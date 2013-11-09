cmake_minimum_required(VERSION 2.8)

set(APPLICATION_NAME "LowLevelOgre")

project(${APPLICATION_NAME})

if(WIN32)
	set(CMAKE_MODULE_PATH "$ENV{OGRE_HOME}/CMake/;${CMAKE_MODULE_PATH}")
	set(OGRE_SAMPLES_INCLUDEPATH "$ENV{OGRE_HOME}/Samples/include")
	if(MSVC_VERSION GREATER 1500 OR MSVC_VERSION EQUAL 1500)
		option(FRAMEWORK_MSVC_ENABLE_MP "Set to ON to build the applicatiob with the /MP option (Visual Studio 2008 and above)." ON)
		mark_as_advanced(FRAMEWORK_MSVC_ENABLE_MP)
		if(FRAMEWORK_MSVC_ENABLE_MP)
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
		endif(FRAMEWORK_MSVC_ENABLE_MP)
	endif(MSVC_VERSION GREATER 1500 OR MSVC_VERSION EQUAL 1500)
else()
	if(EXISTS "/usr/local/lib/OGRE/cmake")
		set(CMAKE_MODULE_PATH "/usr/local/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")
		set(OGRE_SAMPLES_INCLUDEPATH "/usr/local/share/OGRE/samples/Common/include/")
	elseif(EXISTS "/usr/lib/OGRE/cmake")
		set(CMAKE_MODULE_PATH "/usr/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")
		set(OGRE_SAMPLES_INCLUDEPATH "/usr/share/OGRE/samples/Common/include/")
	else ()
		message(SEND_ERROR "Failed to find module path.")
	endif(EXISTS "/usr/local/lib/OGRE")
endif(WIN32)

if (CMAKE_BUILD_TYPE STREQUAL "")
	# CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
	# differentiation between debug and release builds.
	set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()

set(CMAKE_DEBUG_POSTFIX "_d")
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/dist")

find_package(OGRE REQUIRED)
find_package(OIS REQUIRED)

# Find Boost
# Prefer static linking in all cases
if (WIN32 OR APPLE)
	set(Boost_USE_STATIC_LIBS TRUE)
else ()
	# Statically linking boost to a dynamic Ogre build doesn't work on Linux 64bit
	set(Boost_USE_STATIC_LIBS ${OGRE_STATIC})
endif ()
if (APPLE AND OGRE_BUILD_PLATFORM_APPLE_IOS)
    set(Boost_USE_MULTITHREADED OFF)
endif()
set(Boost_ADDITIONAL_VERSIONS "1.54" "1.54.0" "1.53" "1.53.0" "1.52" "1.52.0" "1.51" "1.51.0" "1.50" "1.50.0" "1.49" "1.49.0" "1.48" "1.48.0" "1.47" "1.47.0" "1.46" "1.46.0" "1.45" "1.45.0" "1.44" "1.44.0" "1.42" "1.42.0" "1.41.0" "1.41" "1.40.0" "1.40")
# Components that need linking (NB does not include header-only components like bind)
set(OGRE_BOOST_COMPONENTS thread date_time)
find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} QUIET)
if (NOT Boost_FOUND)
	# Try again with the other type of libs
	if(Boost_USE_STATIC_LIBS)
		set(Boost_USE_STATIC_LIBS OFF)
	else()
		set(Boost_USE_STATIC_LIBS ON)
	endif()
	find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} QUIET)
endif()
if(Boost_FOUND AND Boost_VERSION GREATER 104900)
        set(OGRE_BOOST_COMPONENTS thread date_time system chrono)
        find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} QUIET)
endif()
if(Boost_FOUND)
	# Set up referencing of Boost
	include_directories(${Boost_INCLUDE_DIR})
	add_definitions(-DBOOST_ALL_NO_LIB)
	set(OGRE_LIBRARIES ${OGRE_LIBRARIES} ${Boost_LIBRARIES})
endif()

set(HDRS
	./AdvancedOgreFramework/AdvancedOgreFramework.hpp
	./AdvancedOgreFramework/AppState.hpp
	./AdvancedOgreFramework/AppStateManager.hpp
	./AdvancedOgreFramework/DemoApp.hpp
	./AdvancedOgreFramework/DotSceneLoader.hpp
	./AdvancedOgreFramework/GameState.hpp
	./AdvancedOgreFramework/MenuState.hpp
	./AdvancedOgreFramework/PauseState.hpp
	./AdvancedOgreFramework/rapidxml.hpp
)

set(SRCS
	./AdvancedOgreFramework/AdvancedOgreFramework.cpp
	./AdvancedOgreFramework/AppStateManager.cpp
	./AdvancedOgreFramework/DemoApp.cpp
	./AdvancedOgreFramework/DotSceneLoader.cpp
	./AdvancedOgreFramework/GameState.cpp
	./AdvancedOgreFramework/main.cpp
	./AdvancedOgreFramework/MenuState.cpp
	./AdvancedOgreFramework/PauseState.cpp
)

include_directories(
	${OIS_INCLUDE_DIRS}
	${OGRE_INCLUDE_DIRS}
	${OGRE_SAMPLES_INCLUDEPATH}
)

add_executable(${APPLICATION_NAME} WIN32 ${HDRS} ${SRCS})

set_target_properties(${APPLICATION_NAME} PROPERTIES DEBUG_POSTFIX _d)
target_link_libraries(${APPLICATION_NAME} ${OGRE_LIBRARIES} ${OIS_LIBRARIES})

if ((${OGRE_VERSION} VERSION_EQUAL "1.9.0") OR (${OGRE_VERSION} VERSION_GREATER "1.9.0"))
	include_directories(${OGRE_Overlay_INCLUDE_DIR})
	target_link_libraries(${APPLICATION_NAME} ${OGRE_Overlay_LIBRARIES})
endif()

install(TARGETS ${APPLICATION_NAME}
	RUNTIME DESTINATION bin
	CONFIGURATIONS Debug Release RelWithDebInfo MinSizeRel)

if(EXISTS ${OGRE_Overlay_BINARY_REL})
	install(FILES ${OGRE_Overlay_BINARY_REL} DESTINATION bin CONFIGURATIONS Release RelWithDebInfo MinSizeRel)
endif()
if(EXISTS ${OGRE_RenderSystem_Direct3D11_REL})
	install(FILES ${OGRE_RenderSystem_Direct3D11_REL} DESTINATION bin CONFIGURATIONS Release RelWithDebInfo MinSizeRel)
endif()
if(EXISTS ${OGRE_RenderSystem_Direct3D9_REL})
	install(FILES ${OGRE_RenderSystem_Direct3D9_REL} DESTINATION bin CONFIGURATIONS Release RelWithDebInfo MinSizeRel)
endif()
if(EXISTS ${OGRE_RenderSystem_GL3Plus_REL})
	install(FILES ${OGRE_RenderSystem_GL3Plus_REL} DESTINATION bin CONFIGURATIONS Release RelWithDebInfo MinSizeRel)
endif()
if(EXISTS ${OGRE_RenderSystem_GL_REL})
	install(FILES ${OGRE_RenderSystem_GL_REL} DESTINATION bin CONFIGURATIONS Release RelWithDebInfo MinSizeRel)
endif()
install(FILES ${OGRE_BINARY_REL}
	${OGRE_Paging_BINARY_REL}
	${OGRE_Property_BINARY_REL}
	${OGRE_RTShaderSystem_BINARY_REL}
	${OGRE_Terrain_BINARY_REL}
	${OGRE_Volume_BINARY_REL}
	${OGRE_Plugin_ParticleFX_REL}
	${OGRE_Plugin_BSPSceneManager_REL}
	${OGRE_Plugin_PCZSceneManager_REL}
	${OGRE_Plugin_OctreeZone_REL}
	${OGRE_Plugin_OctreeSceneManager_REL}
	${OGRE_PLUGIN_DIR_REL}/plugins.cfg
	${OIS_BINARY_REL}
	DESTINATION bin
	CONFIGURATIONS Release RelWithDebInfo MinSizeRel)

if(EXISTS ${OGRE_Overlay_BINARY_DBG})
	install(FILES ${OGRE_Overlay_BINARY_DBG} DESTINATION bin CONFIGURATIONS Debug)
endif()
if(EXISTS ${OGRE_RenderSystem_Direct3D11_DBG})
	install(FILES ${OGRE_RenderSystem_Direct3D11_DBG} DESTINATION bin CONFIGURATIONS Debug)
endif()
if(EXISTS ${OGRE_RenderSystem_Direct3D9_DBG})
	install(FILES ${OGRE_RenderSystem_Direct3D9_DBG} DESTINATION bin CONFIGURATIONS Debug)
endif()
if(EXISTS ${OGRE_RenderSystem_GL3Plus_DBG})
	install(FILES ${OGRE_RenderSystem_GL3Plus_DBG} DESTINATION bin CONFIGURATIONS Debug)
endif()
if(EXISTS ${OGRE_RenderSystem_GL_DBG})
	install(FILES ${OGRE_RenderSystem_GL_DBG} DESTINATION bin CONFIGURATIONS Debug)
endif()
install(FILES ${OGRE_BINARY_DBG}
	${OGRE_Paging_BINARY_DBG}
	${OGRE_Property_BINARY_DBG}
	${OGRE_RTShaderSystem_BINARY_DBG}
	${OGRE_Terrain_BINARY_DBG}
	${OGRE_Volume_BINARY_DBG}
	${OGRE_Plugin_ParticleFX_DBG}
	${OGRE_Plugin_BSPSceneManager_DBG}
	${OGRE_Plugin_PCZSceneManager_DBG}
	${OGRE_Plugin_OctreeZone_DBG}
	${OGRE_Plugin_OctreeSceneManager_DBG}
	${OGRE_PLUGIN_DIR_DBG}/plugins_d.cfg
	${OIS_BINARY_DBG}
	DESTINATION bin
	CONFIGURATIONS Debug)

if(WIN32)
	install(FILES ${OGRE_PLUGIN_DIR_REL}/cg.dll
		${OGRE_Plugin_CgProgramManager_REL}
		DESTINATION bin
		CONFIGURATIONS Release RelWithDebInfo MinSizeRel)
	install(FILES ${OGRE_PLUGIN_DIR_REL}/cg.dll
		${OGRE_Plugin_CgProgramManager_DBG}
		DESTINATION bin
		CONFIGURATIONS Debug)
endif(WIN32)
