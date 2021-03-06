if (WIN32)
	set(WWISE_PLATFORM_INCLUDE_PATHS
		AK/StreamManager/Win32
	)
	set(WWISE_PLATFORM_SOURCES
		AK/StreamManager/Win32/AkDefaultIOHookBlocking.h
		AK/StreamManager/Win32/AkDefaultIOHookBlocking.cpp
		AK/StreamManager/Win32/AkFileHelpers.h
		AK/StreamManager/Win32/AkIOThread.h
		AK/StreamManager/Win32/AkIOThread.cpp
		AK/StreamManager/Win32/AkPlatformStreamingDefaults.h
	)
elseif (APPLE)
	set(WWISE_PLATFORM_INCLUDE_PATHS
		AK/StreamManager/POSIX
		AK/StreamManager/Mac
	)
	set(WWISE_PLATFORM_SOURCES
		AK/StreamManager/POSIX/AkDefaultIOHookBlocking.h
		AK/StreamManager/POSIX/AkDefaultIOHookBlocking.cpp
		AK/StreamManager/POSIX/AkFileHelpers.h
		AK/StreamManager/POSIX/AkIOThread.h
		AK/StreamManager/POSIX/AkIOThread.cpp
		AK/StreamManager/Mac/AkPlatformStreamingDefaults.h
	)
elseif (PLAYSTATION)
	set(WWISE_PLATFORM_INCLUDE_PATHS
	../../src/platform/ps/wwise/StreamManager
	)
	set(WWISE_PLATFORM_SOURCES
		../../src/platform/ps/wwise/StreamManager/AkDefaultIOHookBlocking.h
		../../src/platform/ps/wwise/StreamManager/AkDefaultIOHookBlocking.cpp
		../../src/platform/ps/wwise/StreamManager/AkFileHelpers.h
		../../src/platform/ps/wwise/StreamManager/AkIOThread.h
		../../src/platform/ps/wwise/StreamManager/AkIOThread.cpp
		../../src/platform/ps/wwise/StreamManager/AkPlatformStreamingDefaults.h
	)
else() # Linux
	set(WWISE_PLATFORM_INCLUDE_PATHS
		AK/StreamManager/POSIX
		AK/StreamManager/Linux
	)
	set(WWISE_PLATFORM_SOURCES
		AK/StreamManager/POSIX/AkDefaultIOHookBlocking.h
		AK/StreamManager/POSIX/AkDefaultIOHookBlocking.cpp
		AK/StreamManager/POSIX/AkFileHelpers.h
		AK/StreamManager/POSIX/AkIOThread.h
		AK/StreamManager/POSIX/AkIOThread.cpp
		AK/StreamManager/Linux/AkPlatformStreamingDefaults.h
	)
endif()

add_library(wwise_integration STATIC
	CMakeLists.txt
	AK/StreamManager/Common/AkBuddyBlockPool.h
	AK/StreamManager/Common/AkDefaultLowLevelIODispatcher.h
	AK/StreamManager/Common/AkDefaultLowLevelIODispatcher.cpp
	AK/StreamManager/Common/AkDeviceBase.h
	AK/StreamManager/Common/AkDeviceBase.cpp
	AK/StreamManager/Common/AkDeviceBlocking.h
	AK/StreamManager/Common/AkDeviceBlocking.cpp
	AK/StreamManager/Common/AkDeviceDeferredLinedUp.h
	AK/StreamManager/Common/AkDeviceDeferredLinedUp.cpp
	AK/StreamManager/Common/AkFileLocationBase.h
	AK/StreamManager/Common/AkFileLocationBase.cpp
	AK/StreamManager/Common/AkIOMemMgr.h
	AK/StreamManager/Common/AkIOMemMgr.cpp
	AK/StreamManager/Common/AkMultipleFileLocation.h
	AK/StreamManager/Common/AkMultipleFileLocation.inl
	AK/StreamManager/Common/AkStmDeferredLinedUpBase.h
	AK/StreamManager/Common/AkStmDeferredLinedUpBase.inl
	AK/StreamManager/Common/AkStmMemView.h
	AK/StreamManager/Common/AkStreamingDefaults.h
	AK/StreamManager/Common/AkStreamMgr.h
	AK/StreamManager/Common/AkStreamMgr.cpp
	AK/StreamManager/Common/AkTransferDeferred.h
	AK/StreamManager/Common/AkTransferDeferred.cpp
	${WWISE_PLATFORM_SOURCES}
)

target_include_directories(wwise_integration PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${WWISE_PLATFORM_INCLUDE_PATHS}
)

cmake_policy(SET CMP0008 OLD)

if (WIN32)
	target_link_libraries(wwise_integration
		ws2_32
		debug ${CMAKE_CURRENT_SOURCE_DIR}/Win/Debug/AkMemoryMgr
		optimized ${CMAKE_CURRENT_SOURCE_DIR}/Win/Release/AkMemoryMgr
		debug ${CMAKE_CURRENT_SOURCE_DIR}/Win/Debug/AkMusicEngine
		optimized ${CMAKE_CURRENT_SOURCE_DIR}/Win/Release/AkMusicEngine
		debug ${CMAKE_CURRENT_SOURCE_DIR}/Win/Debug/AkSoundEngine
		optimized ${CMAKE_CURRENT_SOURCE_DIR}/Win/Release/AkSoundEngine
		debug ${CMAKE_CURRENT_SOURCE_DIR}/Win/Debug/AkStreamMgr
		optimized ${CMAKE_CURRENT_SOURCE_DIR}/Win/Release/AkStreamMgr
		debug ${CMAKE_CURRENT_SOURCE_DIR}/Win/Debug/AkVorbisDecoder
		optimized ${CMAKE_CURRENT_SOURCE_DIR}/Win/Release/AkVorbisDecoder
		debug ${CMAKE_CURRENT_SOURCE_DIR}/Win/Debug/CommunicationCentral
	)
elseif (APPLE)
	target_link_libraries(wwise_integration
		debug ${CMAKE_CURRENT_SOURCE_DIR}/Mac/Debug/libAkMemoryMgr.a
		optimized ${CMAKE_CURRENT_SOURCE_DIR}/Mac/Release/libAkMemoryMgr.a
		debug ${CMAKE_CURRENT_SOURCE_DIR}/Mac/Debug/libAkMusicEngine.a
		optimized ${CMAKE_CURRENT_SOURCE_DIR}/Mac/Release/libAkMusicEngine.a
		debug ${CMAKE_CURRENT_SOURCE_DIR}/Mac/Debug/libAkSoundEngine.a
		optimized ${CMAKE_CURRENT_SOURCE_DIR}/Mac/Release/libAkSoundEngine.a
		debug ${CMAKE_CURRENT_SOURCE_DIR}/Mac/Debug/libAkStreamMgr.a
		optimized ${CMAKE_CURRENT_SOURCE_DIR}/Mac/Release/libAkStreamMgr.a
		debug ${CMAKE_CURRENT_SOURCE_DIR}/Mac/Debug/libAkVorbisDecoder.a
		optimized ${CMAKE_CURRENT_SOURCE_DIR}/Mac/Release/libAkVorbisDecoder.a
		debug ${CMAKE_CURRENT_SOURCE_DIR}/Mac/Debug/libCommunicationCentral.a
	)
elseif (PLAYSTATION)
	target_link_libraries(wwise_integration
		debug ${CMAKE_CURRENT_SOURCE_DIR}/../../src/platform/ps/wwise/Debug/libAkMemoryMgr.a
		optimized ${CMAKE_CURRENT_SOURCE_DIR}/../../src/platform/ps/wwise/Release/libAkMemoryMgr.a
		debug ${CMAKE_CURRENT_SOURCE_DIR}/../../src/platform/ps/wwise/Debug/libAkMusicEngine.a
		optimized ${CMAKE_CURRENT_SOURCE_DIR}/../../src/platform/ps/wwise/Release/libAkMusicEngine.a
		debug ${CMAKE_CURRENT_SOURCE_DIR}/../../src/platform/ps/wwise/Debug/libCommunicationCentral.a
		debug ${CMAKE_CURRENT_SOURCE_DIR}/../../src/platform/ps/wwise/Debug/libAkSoundEngine.a
		optimized ${CMAKE_CURRENT_SOURCE_DIR}/../../src/platform/ps/wwise/Release/libAkSoundEngine.a
		debug ${CMAKE_CURRENT_SOURCE_DIR}/../../src/platform/ps/wwise/Debug/libAkStreamMgr.a
		optimized ${CMAKE_CURRENT_SOURCE_DIR}/../../src/platform/ps/wwise/Release/libAkStreamMgr.a
		debug ${CMAKE_CURRENT_SOURCE_DIR}/../../src/platform/ps/wwise/Debug/libAkVorbisDecoder.a
		optimized ${CMAKE_CURRENT_SOURCE_DIR}/../../src/platform/ps/wwise/Release/libAkVorbisDecoder.a
	)
else() # Linux
	target_link_libraries(wwise_integration
		debug ${CMAKE_CURRENT_SOURCE_DIR}/Linux/Debug/libAkMemoryMgr.a
		optimized ${CMAKE_CURRENT_SOURCE_DIR}/Linux/Release/libAkMemoryMgr.a
		debug ${CMAKE_CURRENT_SOURCE_DIR}/Linux/Debug/libAkMusicEngine.a
		optimized ${CMAKE_CURRENT_SOURCE_DIR}/Linux/Release/libAkMusicEngine.a
		debug ${CMAKE_CURRENT_SOURCE_DIR}/Linux/Debug/libAkSoundEngine.a
		optimized ${CMAKE_CURRENT_SOURCE_DIR}/Linux/Release/libAkSoundEngine.a
		debug ${CMAKE_CURRENT_SOURCE_DIR}/Linux/Debug/libAkStreamMgr.a
		optimized ${CMAKE_CURRENT_SOURCE_DIR}/Linux/Release/libAkStreamMgr.a
		debug ${CMAKE_CURRENT_SOURCE_DIR}/Linux/Debug/libAkVorbisDecoder.a
		optimized ${CMAKE_CURRENT_SOURCE_DIR}/Linux/Release/libAkVorbisDecoder.a
		debug ${CMAKE_CURRENT_SOURCE_DIR}/Linux/Debug/libCommunicationCentral.a
	)
endif()
