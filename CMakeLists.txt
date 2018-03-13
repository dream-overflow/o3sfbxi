cmake_minimum_required (VERSION 3.4)
project (O3SFBXI)

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
   message(STATUS "Setting build type to 'Debug' as none was specified.")
   set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
   # Set the possible values of build type for cmake-gui
   set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
     "MinSizeRel" "RelWithDebInfo")
endif()

if(NOT MINGW AND ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    # compiler options
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/O2 /Zi /D_DEBUG /EHsc /MP /MDd /DO3D_DISABLE_RAM_MEMORY_MANAGER /DO3D_DISABLE_FAST_MEMORY_MANAGER")
    set(CMAKE_CXX_FLAGS_RELEASE        "/O2 /DNDEBUG /EHsc /MP /MD /DO3D_DISABLE_RAM_MEMORY_MANAGER /DO3D_DISABLE_FAST_MEMORY_MANAGER /DO3D_DISABLE_GPU_MEMORY_MANAGER /DO3D_DISABLE_SPU_MEMORY_MANAGER")
    set(CMAKE_CXX_FLAGS_DEBUG          "/D_DEBUG /Zi /EHsc /MP /MDd /DO3D_DISABLE_RAM_MEMORY_MANAGER /DO3D_DISABLE_FAST_MEMORY_MANAGER")
    set(CMAKE_CXX_FLAGS_DEBUGFULL      "/D_DEBUG /Zi /EHsc /MP /MDd")
    set(CMAKE_C_FLAGS_RELWITHDEBINFO   "/O2 /Zi /D_DEBUG /EHsc /MP /MD /DO3D_DISABLE_RAM_MEMORY_MANAGER /DO3D_DISABLE_FAST_MEMORY_MANAGER")
    set(CMAKE_C_FLAGS_RELEASE          "/O2 /DNDEBUG /EHsc /MP /MD /DO3D_DISABLE_RAM_MEMORY_MANAGER /DO3D_DISABLE_FAST_MEMORY_MANAGER /DO3D_DISABLE_GPU_MEMORY_MANAGER /DO3D_DISABLE_SPU_MEMORY_MANAGER")
    set(CMAKE_C_FLAGS_DEBUG            "/D_DEBUG /Zi /EHsc /MP /MDd /DO3D_DISABLE_RAM_MEMORY_MANAGER /DO3D_DISABLE_FAST_MEMORY_MANAGER")
    set(CMAKE_C_FLAGS_DEBUGFULL        "/D_DEBUG /Zi /EHsc /MP /MDd")
else()
    # compiler options
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -fvisibility=hidden -Wall -std=c++14")  # -fexceptions -Wextra -Wno-unused-parameter

    # until we have fixed any missing override keyword waited by c++17
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter -Wno-inconsistent-missing-override")

    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(CMAKE_CXX_FLAGS_RELEASE        "-O2 -DNDEBUG -ffunction-sections -DO3D_DISABLE_RAM_MEMORY_MANAGER -DO3D_DISABLE_FAST_MEMORY_MANAGER -DO3D_DISABLE_GPU_MEMORY_MANAGER -DO3D_DISABLE_SPU_MEMORY_MANAGER")
        set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -glldb -D_DEBUG -DO3D_DISABLE_RAM_MEMORY_MANAGER -DO3D_DISABLE_FAST_MEMORY_MANAGER")
        set(CMAKE_CXX_FLAGS_DEBUG          "-O0 -g -glldb -D_DEBUG -DO3D_DISABLE_RAM_MEMORY_MANAGER -DO3D_DISABLE_FAST_MEMORY_MANAGER")
        set(CMAKE_CXX_FLAGS_DEBUGFULL      "-O0 -g -glldb -D_DEBUG")
        set(CMAKE_C_FLAGS_RELEASE          "-O2 -DNDEBUG -ffunction-sections -DO3D_DISABLE_RAM_MEMORY_MANAGER -DO3D_DISABLE_FAST_MEMORY_MANAGER -DO3D_DISABLE_GPU_MEMORY_MANAGER -DO3D_DISABLE_SPU_MEMORY_MANAGER")
        set(CMAKE_C_FLAGS_RELWITHDEBINFO   "-O2 -g -glldb -D_DEBUG -DO3D_DISABLE_RAM_MEMORY_MANAGER -DO3D_DISABLE_FAST_MEMORY_MANAGER")
        set(CMAKE_C_FLAGS_DEBUG            "-O0 -g -glldb -D_DEBUG -DO3D_DISABLE_RAM_MEMORY_MANAGER -DO3D_DISABLE_FAST_MEMORY_MANAGER")
        set(CMAKE_C_FLAGS_DEBUGFULL        "-O0 -g -glldb -D_DEBUG")
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        set(CMAKE_CXX_FLAGS_RELEASE        "-O2 -DNDEBUG -ffunction-sections -DO3D_DISABLE_RAM_MEMORY_MANAGER -DO3D_DISABLE_FAST_MEMORY_MANAGER -DO3D_DISABLE_GPU_MEMORY_MANAGER -DO3D_DISABLE_SPU_MEMORY_MANAGER")
        set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -ggdb -D_DEBUG -DO3D_DISABLE_RAM_MEMORY_MANAGER -DO3D_DISABLE_FAST_MEMORY_MANAGER")
        set(CMAKE_CXX_FLAGS_DEBUG          "-O0 -g -ggdb -D_DEBUG -DO3D_DISABLE_RAM_MEMORY_MANAGER -DO3D_DISABLE_FAST_MEMORY_MANAGER")
        set(CMAKE_CXX_FLAGS_DEBUGFULL      "-O0 -g -ggdb -D_DEBUG")
        set(CMAKE_C_FLAGS_RELEASE          "-O2 -DNDEBUG -ffunction-sections -DO3D_DISABLE_RAM_MEMORY_MANAGER -DO3D_DISABLE_FAST_MEMORY_MANAGER -DO3D_DISABLE_GPU_MEMORY_MANAGER -DO3D_DISABLE_SPU_MEMORY_MANAGER")
        set(CMAKE_C_FLAGS_RELWITHDEBINFO   "-O2 -g -ggdb -D_DEBUG -DO3D_DISABLE_RAM_MEMORY_MANAGER -DO3D_DISABLE_FAST_MEMORY_MANAGER")
        set(CMAKE_C_FLAGS_DEBUG            "-O0 -g -ggdb -D_DEBUG -DO3D_DISABLE_RAM_MEMORY_MANAGER -DO3D_DISABLE_FAST_MEMORY_MANAGER")
        set(CMAKE_C_FLAGS_DEBUGFULL        "-O0 -g -ggdb -D_DEBUG")
    endif ()

    if(${CMAKE_SYSTEM_NAME} MATCHES "Android")
        set(CMAKE_CXX_FLAGS_RELEASE "-g0 -Os -DNDEBUG -ffunction-sections")
    endif()

    # linker options
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE  "-Wl,--gc-sections")
    set(CMAKE_MODULE_LINKER_FLAGS_RELEASE  "-Wl,--gc-sections")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE  "-Wl,--gc-sections")
#    if(NOT MINGW)
#        set(CMAKE_SHARED_LINKER_FLAGS_RELEASE  "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} -Wl,-fuse-ld=gold")
#        set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS_RELEASE} -Wl,-fuse-ld=gold")
#        set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wl,-fuse-ld=gold")
#    endif()
endif()

#----------------------------------------------------------
# externals libraries
#----------------------------------------------------------

include(dreamoverflow)

set (CMAKE_FIND_ROOT_PATH ${CMAKE_FIND_ROOT_PATH} $ENV{PREFIX})

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(CMAKE_PREFIX_PATH $ENV{PREFIX} ${CMAKE_PREFIX_PATH})
	set(CMAKE_INCLUDE_PATH $ENV{PREFIX}/include ${CMAKE_INCLUDE_PATH})
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

if(!${O3D_TARGET_ANDROID})
    find_package(OpenGL REQUIRED)
endif()

find_package(Objective3D REQUIRED)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

set(LIBRARY_OUTPUT_PATH ${CMAKE_BUILD_TYPE}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BUILD_TYPE}/bin)

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    set(OBJECTIVE3D_STUDIO_LIBRARY_DIR ${PROJECT_SOURCE_DIR}/../o3dstudio/build-amd64-dbg/bin)
elseif(${CMAKE_BUILD_TYPE} MATCHES "RelWithDebInfo")
    set(OBJECTIVE3D_STUDIO_LIBRARY_DIR ${PROJECT_SOURCE_DIR}/../o3dstudio/build-amd64-odbg/bin)
elseif(${CMAKE_BUILD_TYPE} MATCHES "Release")
    set(OBJECTIVE3D_STUDIO_LIBRARY_DIR ${PROJECT_SOURCE_DIR}/../o3dstudio/build-amd64/bin)
endif()

set(OBJECTIVE3D_STUDIO_PLUGIN_DIR ${OBJECTIVE3D_STUDIO_LIBRARY_DIR}/plugins)
set(OBJECTIVE3D_STUDIO_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/../o3dstudio/common/include)

include_directories(${Qt5Widgets_INCLUDE_DIRS})

#----------------------------------------------------------
# options
#----------------------------------------------------------

include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories(${OBJECTIVE3D_INCLUDE_DIR})
include_directories(${OBJECTIVE3D_INCLUDE_DIR_objective3dconfig})
include_directories(${OBJECTIVE3D_STUDIO_INCLUDE_DIR})

link_directories(${OBJECTIVE3D_LIBRARY_DIR})
link_directories(${OBJECTIVE3D_STUDIO_LIBRARY_DIR})

if(${CMAKE_SYSTEM_NAME} MATCHES "Android")
    if (O3D_USE_SSE2)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse2")
    endif (O3D_USE_SSE2)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Android")

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	if (O3D_USE_SSE2)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse2")
	endif (O3D_USE_SSE2)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	if (O3D_USE_SSE2)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse2")
	endif (O3D_USE_SSE2)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	option(O3D_USE_SSE2 "Compile SSE2 optimizations" OFF)
    if(O3D_USE_SSE2)
		if(MINGW)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse2")
		else()
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:SSE2")
		endif()
    endif (O3D_USE_SSE2)

	# Windows subsystem
	if(MSVC)
		set(CMAKE_CREATE_WIN32_EXE "/SUBSYSTEM:WINDOWS /ENTRY:\"mainCRTStartup\"")
	endif(MSVC)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

#----------------------------------------------------------
# targets
#----------------------------------------------------------

file(GLOB_RECURSE SOURCES_CXX *.cpp src/o3s/plugins/fbxi)

add_library(o3sfbxi SHARED ${SOURCES_CXX})

target_link_libraries(o3sfbxi ${OBJECTIVE3D_LIBRARY} o3scommon)

#----------------------------------------------------------
# install to o3dstudio same build mode
#----------------------------------------------------------

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    install (FILES ${CMAKE_BINARY_DIR}/libo3sfbxi.dll DESTINATION ${OBJECTIVE3D_STUDIO_PLUGIN_DIR})
else()
    install (FILES ${CMAKE_BINARY_DIR}/libo3sfbxi.so DESTINATION ${OBJECTIVE3D_STUDIO_PLUGIN_DIR})
endif()

