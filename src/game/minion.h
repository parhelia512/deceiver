#pragma once

#include "data/entity.h"
#include "ai.h"

namespace VI
{


struct TargetEvent;
struct PlayerManager;

#define MINION_HEAD_RADIUS 0.4f
#define MINION_ATTACK_TIME 2.0f

struct MinionEntity : public Entity
{
	MinionEntity(const Vec3&, const Quat&, AI::Team, PlayerManager* = nullptr);
};

struct Minion : public ComponentType<Minion>
{
	struct Goal
	{
		enum class Type : s8
		{
			Position,
			Target,
			count,
		};

		Vec3 pos;
		Ref<Entity> entity;
		Type type;
	};

	enum class PathRequest : s8
	{
		None,
		Random,
		Position,
		Target,
		Repath,
		PointQuery,
		count,
	};

	static r32 particle_accumulator;

	static Minion* closest(AI::TeamMask, const Vec3&, r32* = nullptr);
	static s32 count(AI::TeamMask);
	static void update_client_all(const Update&);
	static Vec3 goal_position(const Goal&, const Vec3&);

	PathRequest path_request;
	Goal goal;
	AI::Path path;
	r32 attack_timer;
	r32 path_timer;
	r32 target_timer;
	r32 target_scan_timer;
	Ref<PlayerManager> owner;
	s8 path_index;

	void awake();
	Vec3 head_pos() const;
	Vec3 hand_pos() const;
	Vec3 aim_pos() const;
	b8 headshot_test(const Vec3&, const Vec3&);
	void hit_by(const TargetEvent& e);
	void fire(const Vec3&);
	void melee_damage();
	void killed(Entity*);
	void footstep();
	void update_server(const Update&);
	void team(AI::Team);

	b8 can_see(Entity*, b8 = false) const;

	void new_goal(const Vec3& = Vec3::zero, b8 = true);
	void set_path(const AI::Result&);
	void turn_to(const Vec3&);
};


}