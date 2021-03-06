#pragma once
#include "types.h"

struct cJSON;

namespace VI
{

struct EntityFinder;
struct RenderParams;
struct Camera;

namespace Net
{
	struct StreamRead;
}

namespace Overworld
{

struct ResourceInfo
{
	AssetID icon;
	AssetID description;
	s16 cost;
};

enum class State : s8
{
	Hidden,
	SplitscreenSelectOptions,
	SplitscreenSelectTeams,
	SplitscreenSelectZone,
	SplitscreenDeploying,
	StoryMode,
	StoryModeOverlay,
	Deploying,
	count,
};

enum class Tab : s8
{
	Map,
	Inventory,
	count,
};

extern ResourceInfo resource_info[s32(Resource::count)];

b8 net_msg(Net::StreamRead*, Net::MessageSource);
void init(cJSON*);
void update(const Update&);
void draw_opaque(const RenderParams&);
void draw_hollow(const RenderParams&);
void draw_override(const RenderParams&);
void draw_ui(const RenderParams&);
void show(Camera*, State, Tab = Tab::Map);
void clear();
void execute(const char*);
void zone_done(AssetID);
void zone_change(AssetID, ZoneState);
b8 active(); // true if the overworld UI is being shown in any way
b8 modal(); // true if the entire overworld scene is being shown
AssetID zone_under_attack();
r32 zone_under_attack_timer();
void resource_change(Resource, s16);
r32 resource_change_time(Resource);

}

}
