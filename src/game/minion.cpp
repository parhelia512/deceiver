#include "minion.h"
#include "data/animator.h"
#include "render/skinned_model.h"
#include "walker.h"
#include "asset/armature.h"
#include "asset/animation.h"
#include "asset/shader.h"
#include "asset/mesh.h"
#include "asset/Wwise_IDs.h"
#include "audio.h"
#include "player.h"
#include "mersenne/mersenne-twister.h"
#include "game.h"
#include "render/views.h"
#include "drone.h"
#include "BulletCollision/NarrowPhaseCollision/btRaycastCallback.h"
#include "data/ragdoll.h"
#include "entities.h"
#include "render/particles.h"
#include "net.h"
#include "team.h"
#include "parkour.h"
#include "data/components.h"
#include "common.h"

#define WALK_SPEED 2.5f
#define ROTATION_SPEED 4.0f
#define MINION_HEARING_RANGE 7.0f
#define MINION_VISION_RANGE 20.0f
#define MINION_MELEE_RANGE 2.5f
#define HEALTH 5
#define PATH_RECALC_TIME 1.0f
#define TARGET_SCAN_TIME 0.5f

namespace VI
{

MinionEntity::MinionEntity(const Vec3& pos, const Quat& quat, AI::Team team, PlayerManager* manager)
{
	Transform* transform = create<Transform>();
	transform->pos = pos;

	Animator* animator = create<Animator>();
	SkinnedModel* model = create<SkinnedModel>();

	animator->armature = Asset::Armature::character;
	animator->layers[0].play(Asset::Animation::character_idle);

	model->shader = Asset::Shader::armature;
	model->mesh = Asset::Mesh::character;
	model->team = s8(team);
	model->color.w = MATERIAL_NO_OVERRIDE;

	create<Audio>();

	create<Health>(HEALTH, HEALTH);
	
	Vec3 forward = quat * Vec3(0, 0, 1);

	Walker* walker = create<Walker>(atan2f(forward.x, forward.z));
	walker->max_speed = walker->speed = WALK_SPEED;
	walker->rotation_speed = ROTATION_SPEED;

	Minion* m = create<Minion>();
	m->owner = manager;

	create<AIAgent>()->team = team;

	create<Target>();

}

void Minion::awake()
{
	link_arg<const TargetEvent&, &Minion::hit_by>(get<Target>()->target_hit);
	link_arg<Entity*, &Minion::killed>(get<Health>()->killed);
	target_timer = 100000.0f; // force target recalculation

	Animator* animator = get<Animator>();
	link<&Minion::footstep>(animator->trigger(Asset::Animation::character_walk, 0.0f));
	link<&Minion::footstep>(animator->trigger(Asset::Animation::character_walk, 0.5f));
	link<&Minion::melee_damage>(animator->trigger(Asset::Animation::character_melee, 0.875f));
}

void Minion::team(AI::Team t)
{
	// not synced over the network
	if (t != get<AIAgent>()->team)
	{
		get<AIAgent>()->team = t;
		get<SkinnedModel>()->team = s8(t);
		if (Game::level.local)
			get<Minion>()->new_goal(Vec3::zero, false); // don't allow entity targets; must be a random path
	}
}

Minion* Minion::closest(AI::TeamMask mask, const Vec3& pos, r32* distance)
{
	Minion* closest = nullptr;
	r32 closest_distance = FLT_MAX;
	for (auto i = list.iterator(); !i.is_last(); i.next())
	{
		if (AI::match(i.item()->get<AIAgent>()->team, mask))
		{
			r32 d = (pos - i.item()->get<Transform>()->absolute_pos()).length_squared();
			if (d < closest_distance)
			{
				closest = i.item();
				closest_distance = d;
			}
		}
	}
	if (distance)
		*distance = sqrtf(closest_distance);
	return closest;
}

s32 Minion::count(AI::TeamMask mask)
{
	s32 result = 0;
	for (auto i = list.iterator(); !i.is_last(); i.next())
	{
		if (AI::match(i.item()->get<AIAgent>()->team, mask))
			result++;
	}
	return result;
}

void Minion::footstep()
{
	Audio::post_global_event(AK::EVENTS::PLAY_FOOTSTEP, get<Walker>()->base_pos());
}

void Minion::melee_damage()
{
	Vec3 me = get<Transform>()->absolute_pos();
	Vec3 forward = get<Walker>()->forward();

	b8 did_damage = false;
	for (auto i = PlayerCommon::list.iterator(); !i.is_last(); i.next())
	{
		Vec3 to_target = i.item()->get<Transform>()->absolute_pos() - me;
		r32 distance = to_target.length();
		if (distance < MINION_MELEE_RANGE
			&& forward.dot(to_target / distance) > 0.707f)
		{
			did_damage = true;
			if (i.item()->has<Parkour>())
			{
				Vec3 v = Vec3::normalize(to_target);
				v.y = 0.6f;
				i.item()->get<RigidBody>()->btBody->setLinearVelocity(v * 7.0f);
				Parkour* parkour = i.item()->get<Parkour>();
				parkour->last_support = i.item()->get<Walker>()->support = nullptr;
				parkour->last_support_time = Game::time.total;
				parkour->wall_run_state = Parkour::WallRunState::None;

				Parkour::State state = parkour->fsm.current;
				if (Game::level.local && state != Parkour::State::Roll && state != Parkour::State::Slide)
					i.item()->get<Health>()->damage(entity(), 1);
			}
			else
			{
				// drone
				if (Game::level.local)
					i.item()->get<Health>()->damage(entity(), 1);
			}
		}
	}

	// spark effects
	if (did_damage)
	{
		Vec3 p = hand_pos();
		Quat rot = Quat::look(forward);
		for (s32 i = 0; i < 50; i++)
		{
			Particles::sparks.add
			(
				p,
				rot * Vec3(mersenne::randf_oo() * 2.0f - 1.0f, mersenne::randf_oo() * 2.0f - 1.0f, mersenne::randf_oo()) * 10.0f,
				Vec4(1, 1, 1, 1)
			);
		}
	}
}

Vec3 Minion::head_pos() const
{
	return get<Target>()->absolute_pos();
}

Vec3 Minion::hand_pos() const
{
	Vec3 p(0, 0, 0);
	get<Animator>()->to_world(Asset::Bone::character_hand_R, &p);
	return p;
}

Vec3 Minion::aim_pos() const
{
	Mat4 mat;
	get<Transform>()->mat(&mat);
	mat = get<SkinnedModel>()->offset * mat;
	return (mat * Vec4(-0.188f, 1.600f, -0.516f, 1.0f)).xyz();
}

b8 Minion::headshot_test(const Vec3& ray_start, const Vec3& ray_end)
{
	return LMath::ray_sphere_intersect(ray_start, ray_end, head_pos(), MINION_HEAD_RADIUS);
}

Entity* closest_target(Minion* me, AI::Team team, const Vec3& direction)
{
	if (!Game::level.has_feature(Game::FeatureLevel::TutorialAll)) // in tutorial, don't chase after targets
		return nullptr;

	// if the target is in the wrong direction, add a cost to it
	r32 direction_cost = direction.length_squared() > 0.0f ? 100.0f : 0.0f;

	Vec3 pos = me->get<Transform>()->absolute_pos();
	Entity* closest = nullptr;

	r32 closest_distance = FLT_MAX;

	if (Turret::list.count() > 0)
	{
		for (auto i = Turret::list.iterator(); !i.is_last(); i.next())
		{
			Turret* turret = i.item();
			if (turret->team != team)
			{
				Vec3 item_pos = turret->get<Transform>()->absolute_pos();
				if (me->can_see(turret->entity()))
					return turret->entity();
				Vec3 to_turret = turret->get<Transform>()->absolute_pos() - pos;
				r32 total_distance = to_turret.length_squared() + (to_turret.dot(direction) < 0.0f ? direction_cost : 0.0f);
				if (total_distance < closest_distance)
				{
					closest = turret->entity();
					closest_distance = total_distance;
				}
			}
		}
	}
	else
	{
		for (auto i = CoreModule::list.iterator(); !i.is_last(); i.next())
		{
			CoreModule* core = i.item();
			if (core->team != team)
			{
				Vec3 item_pos = core->get<Transform>()->absolute_pos();
				if (me->can_see(core->entity()))
					return core->entity();
				Vec3 to_core = core->get<Transform>()->absolute_pos() - pos;
				r32 total_distance = to_core.length_squared() + (to_core.dot(direction) < 0.0f ? direction_cost : 0.0f);
				if (total_distance < closest_distance)
				{
					closest = core->entity();
					closest_distance = total_distance;
				}
			}
		}
	}

	if (closest)
		return closest;

	for (auto i = ForceField::list.iterator(); !i.is_last(); i.next())
	{
		ForceField* field = i.item();
		if (field->team != team)
		{
			Vec3 item_pos = field->get<Transform>()->absolute_pos();
			if (me->can_see(field->entity()))
				return field->entity();
			Vec3 to_field = item_pos - pos;
			r32 total_distance = to_field.length_squared() + (to_field.dot(direction) < 0.0f ? direction_cost : 0.0f);
			if (total_distance < closest_distance)
			{
				closest = field->entity();
				closest_distance = total_distance;
			}
		}
	}

	for (auto i = Battery::list.iterator(); !i.is_last(); i.next())
	{
		Battery* pickup = i.item();
		if (pickup->team != team && pickup->team != AI::TeamNone)
		{
			Vec3 item_pos = pickup->get<Transform>()->absolute_pos();
			if (me->can_see(pickup->entity()))
				return pickup->entity();
			Vec3 to_pickup = item_pos - pos;
			r32 total_distance = to_pickup.length_squared() + (to_pickup.dot(direction) < 0.0f ? direction_cost : 0.0f);
			if (total_distance < closest_distance)
			{
				closest = pickup->entity();
				closest_distance = total_distance;
			}
		}
	}

	for (auto i = Sensor::list.iterator(); !i.is_last(); i.next())
	{
		Sensor* sensor = i.item();
		if (sensor->team != team && !sensor->has<Battery>())
		{
			Vec3 item_pos = sensor->get<Transform>()->absolute_pos();
			if (me->can_see(sensor->entity()))
				return sensor->entity();
			Vec3 to_sensor = sensor->get<Transform>()->absolute_pos() - pos;
			r32 total_distance = to_sensor.length_squared() + (to_sensor.dot(direction) < 0.0f ? direction_cost : 0.0f);
			if (total_distance < closest_distance)
			{
				closest = sensor->entity();
				closest_distance = total_distance;
			}
		}
	}

	for (auto i = Minion::list.iterator(); !i.is_last(); i.next())
	{
		Minion* minion = i.item();
		if (minion->get<AIAgent>()->team != team)
		{
			Vec3 item_pos = minion->get<Transform>()->absolute_pos();
			if (me->can_see(minion->entity()))
				return minion->entity();
			Vec3 to_minion = minion->get<Transform>()->absolute_pos() - pos;
			r32 total_distance = to_minion.length_squared() + (to_minion.dot(direction) < 0.0f ? direction_cost : 0.0f);
			if (total_distance < closest_distance)
			{
				closest = minion->entity();
				closest_distance = total_distance;
			}
		}
	}

	for (auto i = Rocket::list.iterator(); !i.is_last(); i.next())
	{
		Rocket* rocket = i.item();
		if (rocket->get<Transform>()->parent.ref() && rocket->team() != team)
		{
			Vec3 item_pos = rocket->get<Transform>()->absolute_pos();
			if (me->can_see(rocket->entity()))
				return rocket->entity();
			Vec3 to_rocket = rocket->get<Transform>()->absolute_pos() - pos;
			r32 total_distance = to_rocket.length_squared() + (to_rocket.dot(direction) < 0.0f ? direction_cost : 0.0f);
			if (total_distance < closest_distance)
			{
				closest = rocket->entity();
				closest_distance = total_distance;
			}
		}
	}

	for (auto i = Grenade::list.iterator(); !i.is_last(); i.next())
	{
		Grenade* grenade = i.item();
		if (grenade->team() != team)
		{
			Vec3 item_pos = grenade->get<Transform>()->absolute_pos();
			if (me->can_see(grenade->entity()))
				return grenade->entity();
			Vec3 to_grenade = grenade->get<Transform>()->absolute_pos() - pos;
			r32 total_distance = to_grenade.length_squared() + (to_grenade.dot(direction) < 0.0f ? direction_cost : 0.0f);
			if (total_distance < closest_distance)
			{
				closest = grenade->entity();
				closest_distance = total_distance;
			}
		}
	}

	return closest;
}

Entity* visible_target(Minion* me, AI::Team team)
{
	for (auto i = Decoy::list.iterator(); !i.is_last(); i.next())
	{
		Decoy* drone = i.item();
		if (drone->get<AIAgent>()->team != team)
		{
			if (me->can_see(drone->entity(), true))
				return drone->entity();
		}
	}

	for (auto i = PlayerCommon::list.iterator(); !i.is_last(); i.next())
	{
		PlayerCommon* player = i.item();
		if (player->get<AIAgent>()->team != team)
		{
			if (me->can_see(player->entity(), true))
				return player->entity();
		}
	}

	for (auto i = Minion::list.iterator(); !i.is_last(); i.next())
	{
		Minion* minion = i.item();
		if (minion->get<AIAgent>()->team != team)
		{
			if (me->can_see(minion->entity()))
				return minion->entity();
		}
	}

	if (Turret::list.count() > 0)
	{
		for (auto i = Turret::list.iterator(); !i.is_last(); i.next())
		{
			Turret* turret = i.item();
			if (turret->team != team)
			{
				if (me->can_see(turret->entity()))
					return turret->entity();
			}
		}
	}
	else
	{
		for (auto i = CoreModule::list.iterator(); !i.is_last(); i.next())
		{
			CoreModule* core = i.item();
			if (core->team != team)
			{
				if (me->can_see(core->entity()))
					return core->entity();
			}
		}
	}

	for (auto i = Grenade::list.iterator(); !i.is_last(); i.next())
	{
		Grenade* grenade = i.item();
		if (grenade->team() != team)
		{
			if (me->can_see(grenade->entity()))
				return grenade->entity();
		}
	}

	for (auto i = ForceField::list.iterator(); !i.is_last(); i.next())
	{
		ForceField* field = i.item();
		if (field->team != team)
		{
			if (me->can_see(field->entity()))
				return field->entity();
		}
	}

	for (auto i = Battery::list.iterator(); !i.is_last(); i.next())
	{
		Battery* pickup = i.item();
		if (pickup->team != team && pickup->team != AI::TeamNone)
		{
			if (me->can_see(pickup->entity()))
				return pickup->entity();
		}
	}

	for (auto i = Sensor::list.iterator(); !i.is_last(); i.next())
	{
		Sensor* sensor = i.item();
		if (sensor->team != team && !sensor->has<Battery>())
		{
			if (me->can_see(sensor->entity()))
				return sensor->entity();
		}
	}

	for (auto i = Rocket::list.iterator(); !i.is_last(); i.next())
	{
		Rocket* rocket = i.item();
		if (rocket->get<Transform>()->parent.ref() && rocket->team() != team)
		{
			if (me->can_see(rocket->entity()))
				return rocket->entity();
		}
	}

	return nullptr;
}

Vec3 Minion::goal_position(const Goal& g, const Vec3& minion_pos)
{
	if (g.type == Goal::Type::Target)
	{
		Entity* e = g.entity.ref();
		vi_assert(e);
		if (e->has<Turret>())
		{
			if (e->get<Turret>()->ingress_points.length > 0)
			{
				r32 closest_distance_sq = FLT_MAX;
				Vec3 closest_point;
				Turret* t = e->get<Turret>();
				for (s32 i = 0; i < t->ingress_points.length; i++)
				{
					const Vec3& pos = t->ingress_points[i];
					r32 distance_sq = (pos - minion_pos).length_squared();
					if (distance_sq < closest_distance_sq)
					{
						closest_distance_sq = distance_sq;
						closest_point = pos;
					}
				}
				return closest_point;
			}
			else
				return e->get<Transform>()->to_world(Vec3(0, 0, -TURRET_HEIGHT));
		}
		else
			return e->get<Transform>()->absolute_pos();
	}
	else
		return g.pos;
}

void Minion::update_server(const Update& u)
{
	// AI
	target_timer += u.time.delta;

	b8 can_attack = false;
	if (get<Minion>()->attack_timer > 0.0f)
	{
		get<Minion>()->attack_timer = vi_max(0.0f, get<Minion>()->attack_timer - u.time.delta);
		can_attack = get<Minion>()->attack_timer == 0.0f;
	}

	Vec3 pos = get<Walker>()->base_pos();

	if (path_request == PathRequest::None)
	{
		target_scan_timer = vi_max(0.0f, target_scan_timer - u.time.delta);
		if (target_scan_timer == 0.0f)
		{
			target_scan_timer = TARGET_SCAN_TIME;

			Entity* target_candidate = visible_target(this, get<AIAgent>()->team);
			if (target_candidate)
			{
				if (target_candidate != goal.entity.ref())
				{
					// look, a shiny!
					path.length = 0;
					goal.type = Goal::Type::Target;
					goal.entity = target_candidate;
					target_timer = 0;
				}
			}
			else
				goal.entity = nullptr; // our current target no longer matches our criteria
		}

		b8 recalc = false;
		path_timer = vi_max(0.0f, path_timer - u.time.delta);
		if (path_timer == 0.0f)
		{
			path_timer = PATH_RECALC_TIME;
			recalc = true;
		}

		switch (goal.type)
		{
			case Goal::Type::Position:
			{
				if (path.length == 0 || (path[path.length - 1] - pos).length_squared() < 1.5f * 1.5f)
				{
					if (target_timer > 2.0f)
						new_goal();
				}
				else
				{
					if (recalc)
					{
						// recalc path
						path_request = PathRequest::Repath;
						AI::pathfind(pos, goal.pos, ObjectLinkEntryArg<Minion, const AI::Result&, &Minion::set_path>(id()));
					}
				}
				break;
			}
			case Goal::Type::Target:
			{
				Entity* g = goal.entity.ref();
				if (g)
				{
					// we're going after the target
					if (can_see(g))
					{
						// turn to and attack the target
						Vec3 hand_pos = get<Minion>()->aim_pos();
						Vec3 aim_pos;
						if (!g->has<Target>() || !g->get<Target>()->predict_intersection(hand_pos, BOLT_SPEED, nullptr, &aim_pos))
							aim_pos = g->get<Transform>()->absolute_pos();
						turn_to(aim_pos);
						path.length = 0;

						Animator::Layer* anim_layer = &get<Animator>()->layers[0];

						if (fabsf(LMath::angle_to(get<Walker>()->target_rotation, get<Walker>()->rotation)) < PI * 0.05f // make sure we're looking at the target
							&& target_timer > MINION_ATTACK_TIME * 0.25f // give some reaction time
							&& anim_layer->animation != Asset::Animation::character_melee
							&& !Team::game_over)
						{
							if (g->has<Parkour>() && (aim_pos - hand_pos).length_squared() < MINION_MELEE_RANGE * MINION_MELEE_RANGE)
							{
								anim_layer->speed = 1.0f;
								anim_layer->behavior = Animator::Behavior::Default;
								anim_layer->play(Asset::Animation::character_melee);
								get<Minion>()->attack_timer = 0.0f;
							}
							else if (can_attack)
								get<Minion>()->fire(aim_pos);
							else if (get<Minion>()->attack_timer == 0.0f)
								get<Minion>()->attack_timer = MINION_ATTACK_TIME;
						}
					}
					else
					{
						if (recalc)
						{
							// recalc path
							path_request = PathRequest::Target;
							AI::pathfind(pos, goal_position(goal, get<Walker>()->base_pos()), ObjectLinkEntryArg<Minion, const AI::Result&, &Minion::set_path>(id()));
						}
					}
				}
				else
					new_goal();
				break;
			}
			default:
			{
				vi_assert(false);
				break;
			}
		}
	}

	// path following

	{
		const Animator::Layer& layer = get<Animator>()->layers[0];
		if (path_index < path.length
			&& layer.animation != Asset::Animation::character_fire
			&& layer.animation != Asset::Animation::character_melee)
		{
			Vec3 flat_pos = pos;
			flat_pos.y = 0.0f;
			Vec3 t = path[path_index];
			t.y = 0.0f;
			Vec3 ray = t - flat_pos;
			while (ray.length_squared() < 0.05f * 0.05f)
			{
				path_index++;
				if (path_index == path.length)
					break;
				t = path[path_index];
				t.y = 0.0f;
				ray = t - flat_pos;
			}

			r32 ray_length = ray.length();
			if (ray_length > 0.1f)
			{
				ray /= ray_length;
				get<Walker>()->dir = Vec2(ray.x, ray.z);
			}
		}
		else
			get<Walker>()->dir = Vec2::zero;
	}

	// update animation
	{
		Animator::Layer* layer = &get<Animator>()->layers[0];

		if (layer->animation != Asset::Animation::character_fire
			&& layer->animation != Asset::Animation::character_melee)
		{
			if (get<Walker>()->support.ref() && get<Walker>()->dir.length_squared() > 0.0f)
			{
				r32 net_speed = vi_max(get<Walker>()->net_speed, WALK_SPEED * 0.5f);
				layer->speed = net_speed / get<Walker>()->speed;
				layer->behavior = Animator::Behavior::Loop;
				layer->play(Asset::Animation::character_walk);
			}
			else if (attack_timer > 0.0f)
			{
				if (layer->animation != Asset::Animation::character_aim)
				{
					layer->speed = 1.0f;
					layer->behavior = Animator::Behavior::Loop;
					layer->animation = Asset::Animation::character_aim;
					layer->time = 0.0f;
				}
			}
			else
			{
				layer->speed = 1.0f;
				layer->behavior = Animator::Behavior::Loop;
				layer->play(Asset::Animation::character_idle);
			}
		}
	}
}

void Minion::fire(const Vec3& target)
{
	vi_assert(Game::level.local);
	Vec3 hand = aim_pos();
	Net::finalize(World::create<BoltEntity>(get<AIAgent>()->team, owner.ref(), Bolt::Type::Normal, hand, target - hand));

	Animator::Layer* layer = &get<Animator>()->layers[0];
	layer->speed = 1.0f;
	layer->behavior = Animator::Behavior::Default;
	layer->play(Asset::Animation::character_fire);
}

r32 Minion::particle_accumulator;
void Minion::update_client_all(const Update& u)
{
	const r32 interval = 0.02f;
	particle_accumulator += u.time.delta;
	while (particle_accumulator > interval)
	{
		particle_accumulator -= interval;
		for (auto i = list.iterator(); !i.is_last(); i.next())
		{
			if (i.item()->get<Animator>()->layers[0].animation == Asset::Animation::character_aim)
			{
				Vec3 pos = i.item()->hand_pos();

				// spawn particle effect
				Vec3 offset = Quat::euler(0.0f, mersenne::randf_co() * PI * 2.0f, (mersenne::randf_co() - 0.5f) * PI) * Vec3(0, 0, 1.0f);
				Particles::fast_tracers.add
				(
					pos + offset,
					offset * -3.5f,
					0
				);
			}
		}
	}

	for (auto i = list.iterator(); !i.is_last(); i.next())
	{
		Minion* m = i.item();
		m->get<SkinnedModel>()->offset.make_transform
		(
			Vec3(0, m->get<Walker>()->capsule_height() * -0.5f - WALKER_SUPPORT_HEIGHT, 0),
			Vec3(1.0f, 1.0f, 1.0f),
			Quat::euler(0, m->get<Walker>()->rotation + PI * 0.5f, 0)
		);

		// update head position
		{
			m->get<Target>()->local_offset = Vec3(0.1f, 0, 0);
			m->get<Animator>()->to_local(Asset::Bone::character_head, &m->get<Target>()->local_offset);
		}
	}
}

void Minion::hit_by(const TargetEvent& e)
{
	get<Health>()->damage(e.hit_by, get<Health>()->hp_max);
}

void Minion::killed(Entity* killer)
{
	PlayerManager::entity_killed_by(entity(), killer);
	get<Audio>()->post_event(AK::EVENTS::STOP);
	Audio::post_global_event(AK::EVENTS::PLAY_HEADSHOT, head_pos());

	if (Game::level.local)
	{
		Entity* ragdoll = World::create<Empty>();
		ragdoll->get<Transform>()->absolute_pos(get<Transform>()->absolute_pos());

		// Apply the SkinnedModel::offset rotation to the ragdoll transform to make everything work
		ragdoll->get<Transform>()->absolute_rot(Quat::euler(0, get<Walker>()->rotation + PI * 0.5f, 0));

		SkinnedModel* new_skin = ragdoll->add<SkinnedModel>();
		SkinnedModel* old_skin = get<SkinnedModel>();
		new_skin->mesh = old_skin->mesh;
		new_skin->mesh_shadow = old_skin->mesh_shadow;
		new_skin->shader = old_skin->shader;
		new_skin->texture = old_skin->texture;
		new_skin->color = old_skin->color;
		new_skin->team = old_skin->team;
		new_skin->mask = old_skin->mask;

		// No rotation
		new_skin->offset.make_transform(
			Vec3(0, -1.1f, 0),
			Vec3(1.0f, 1.0f, 1.0f),
			Quat::identity
		);

		Animator* new_anim = ragdoll->add<Animator>();
		Animator* old_anim = get<Animator>();
		new_anim->armature = old_anim->armature;
		new_anim->bones.resize(old_anim->bones.length);
		for (s32 i = 0; i < old_anim->bones.length; i++)
			new_anim->bones[i] = old_anim->bones[i];

		World::remove_deferred(entity());

		Ragdoll* r = ragdoll->add<Ragdoll>();

		if (killer)
		{
			if (killer->has<Drone>())
				r->apply_impulse(Ragdoll::Impulse::Head, killer->get<Drone>()->velocity * 0.1f);
			else
			{
				Vec3 killer_to_us = get<Transform>()->absolute_pos() - killer->get<Transform>()->absolute_pos();
				r->apply_impulse(killer->has<Parkour>() && killer_to_us.y < get<Walker>()->capsule_height() ? Ragdoll::Impulse::Feet : Ragdoll::Impulse::Head, Vec3::normalize(killer_to_us) * 10.0f);
			}
		}

		Net::finalize(ragdoll);
	}
}

b8 Minion::can_see(Entity* target, b8 limit_vision_cone) const
{
	if (target->has<AIAgent>() && target->get<AIAgent>()->stealth)
		return false;

	Vec3 pos = get<Minion>()->aim_pos();
	Vec3 target_pos = target->get<Transform>()->absolute_pos();
	Vec3 diff = target_pos - pos;
	r32 distance_squared = diff.length_squared();

	// if we're targeting an drone that is flying or just flew recently,
	// then don't limit detection to the minion's vision cone
	// this essentially means the minion can hear the drone flying around
	if (limit_vision_cone)
	{
		if (target->has<Drone>())
		{
			if (distance_squared < MINION_HEARING_RANGE * MINION_HEARING_RANGE && Game::time.total - target->get<Drone>()->attach_time < 1.0f) // we can hear the drone
				limit_vision_cone = false;
			else
			{
				PlayerManager* manager = target->get<PlayerCommon>()->manager.ref();
				if (Team::list[(s32)get<AIAgent>()->team].player_tracks[manager->id()].tracking)
					limit_vision_cone = false;
			}
		}
		else if (target->has<Parkour>())
		{
			if (distance_squared < MINION_HEARING_RANGE * MINION_HEARING_RANGE)
				limit_vision_cone = false;
		}
	}

	// if we're targeting a decoy, always go for the decoy
	if (target->has<Decoy>())
		limit_vision_cone = false;

	if (distance_squared < MINION_VISION_RANGE * MINION_VISION_RANGE)
	{
		r32 distance = diff.length();
		Vec3 dir = diff / distance;
		if (!limit_vision_cone || dir.dot(get<Walker>()->forward()) > 0.707f)
		{
			if (!target->has<Parkour>() || fabsf(diff.y) < MINION_HEARING_RANGE)
			{
				btCollisionWorld::ClosestRayResultCallback ray_callback(pos, target_pos);
				Physics::raycast(&ray_callback, (CollisionStatic | CollisionInaccessible | CollisionElectric | CollisionAllTeamsForceField) & ~Team::force_field_mask(get<AIAgent>()->team));
				if (!ray_callback.hasHit() || target == &Entity::list[ray_callback.m_collisionObject->getUserIndex()])
					return true;
			}
		}
	}
	return false;
}

void Minion::new_goal(const Vec3& direction, b8 allow_entity_target)
{
	Vec3 pos = get<Walker>()->base_pos();
	goal.entity = allow_entity_target ? closest_target(this, get<AIAgent>()->team, direction) : nullptr;
	auto path_callback = ObjectLinkEntryArg<Minion, const AI::Result&, &Minion::set_path>(id());
	if (goal.entity.ref())
	{
		goal.type = Goal::Type::Target;
		if (!can_see(goal.entity.ref()))
		{
			path_request = PathRequest::Target;
			goal.pos = goal_position(goal, pos);
			AI::pathfind(pos, goal.pos, path_callback);
		}
	}
	else
	{
		goal.type = Goal::Type::Position;
		path_request = PathRequest::Random;
		AI::random_path(pos, pos, MINION_VISION_RANGE, path_callback);
	}
	target_timer = 0.0f;
	path_timer = PATH_RECALC_TIME;
}

Vec3 goal_pos(const Minion::Goal& g)
{
	if (g.type == Minion::Goal::Type::Position || !g.entity.ref())
		return g.pos;
	else
		return g.entity.ref()->get<Transform>()->absolute_pos();
}

void Minion::set_path(const AI::Result& result)
{
	get<Minion>()->attack_timer = 0.0f; // we're no longer attacking

	path_request = PathRequest::None;
	path = result.path;
	path_index = 0;
	if (path.length > 1)
	{
		// sometimes the system returns an extra path point at the beginning, which actually puts us farther from the goal
		// if we're close enough to the second path point, then skip that first one.
		if ((path[1] - get<Walker>()->base_pos()).length_squared() < 0.3f * 0.3f)
			path_index = 1;
	}
	if (path_request != PathRequest::Repath)
	{
		if (path.length > 0)
			goal.pos = path[path.length - 1];
		else
			goal.pos = get<Transform>()->absolute_pos();
	}
}

void Minion::turn_to(const Vec3& target)
{
	Vec3 forward = Vec3::normalize(target - get<Transform>()->absolute_pos());
	get<Walker>()->target_rotation = atan2f(forward.x, forward.z);
}


}