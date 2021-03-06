#pragma once

#include "data/entity.h"
#if SERVER
#include <AK/SoundEngine/Common/AkTypes.h>
#else
#if _WIN32
#include <AK/StreamManager/Win32/AkDefaultIOHookBlocking.h>
#elif defined(__APPLE__)
#include <AK/StreamManager/POSIX/AkDefaultIOHookBlocking.h>
#elif defined(__ORBIS__)
#include <AkDefaultIOHookBlocking.h>
#else // linux
#include <AK/StreamManager/POSIX/AkDefaultIOHookBlocking.h>
#endif
#include <AK/SoundEngine/Common/AkCommonDefs.h>
#include <AK/SoundEngine/Common/AkCallback.h>
#endif

namespace VI
{

struct Vec3;
struct Quat;

struct Audio : ComponentType<Audio>
{
#if !SERVER
	static CAkDefaultIOHookBlocking wwise_io;
	static void dialogue_volume_callback(AK::IAkMetering*, AkChannelConfig, AkMeteringFlags);
	static void dialogue_done_callback(AkCallbackType, AkCallbackInfo*);
#endif

	static r32 dialogue_volume;
	static StaticArray<ID, 32> dialogue_callbacks; // poll this and empty it every frame; ID is entity ID
	static b8 init();
	static void term();
	static void update();
	static void post_global_event(AkUniqueID);
	static void post_global_event(AkUniqueID, const Vec3&);
	static void post_global_event(AkUniqueID, const Vec3&, const Quat&);
	static void global_param(AkRtpcID, AkRtpcValue);
	static void listener_enable(u32);
	static void listener_disable(u32);
	static void listener_update(u32, const Vec3&, const Quat&);
	static AkUniqueID get_id(const char*);

	b8 registered;

	void awake();
	~Audio();
	void post_event(AkUniqueID);
	b8 post_dialogue_event(AkUniqueID);
	void param(AkRtpcID, AkRtpcValue);
};

}
