cmake_minimum_required(VERSION 3.2)
project(Ads VERSION 0.0.0.1)

set(CMAKE_VERBOSE_MAKEFILE OFF)

option(use_local_twincat "set has_twincat_dll to ON if compiled on a workstation with TWinCAT installed" ON)

# load local scripts
include (cmake/Global.cmake)      

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/app_config.hpp.in"
  "${PROJECT_SOURCE_DIR}/app_config.hpp"
  ) 

SET(config_hpp app_config.hpp.in app_config.hpp CMakeLists.txt)

# header files  
include_directories(.) 
include_directories(./3rd) 

# for AdsLib
set(adslib_root ./AdsLib)
set(lib_hpp ${adslib_root}/AdsDef.h ${adslib_root}/AdsLib.h ${adslib_root}/AdsNotification.h ${adslib_root}/AmsConnection.h ${adslib_root}/AmsHeader.h ${adslib_root}/AmsPort.h ${adslib_root}/AmsRouter.h ${adslib_root}/Frame.h ${adslib_root}/Log.h ${adslib_root}/NotificationDispatcher.h ${adslib_root}/RingBuffer.h ${adslib_root}/Router.h ${adslib_root}/Semaphore.h ${adslib_root}/Sockets.h ${adslib_root}/wrap_endian.h ${adslib_root}/wrap_socket.h)

set(lib_cpp ${adslib_root}/AdsDef.cpp ${adslib_root}/AdsLib.cpp ${adslib_root}/AmsConnection.cpp ${adslib_root}/AmsPort.cpp ${adslib_root}/AmsRouter.cpp ${adslib_root}/Frame.cpp ${adslib_root}/Log.cpp ${adslib_root}/NotificationDispatcher.cpp ${adslib_root}/Sockets.cpp)

# AdsLib
add_library(AdsLib ${lib_cpp})

if(use_local_twincat)
    # AdsLibTestRef
    set(TWinCAT_TcAdsDll C:/TwinCAT/AdsApi/TcAdsDll)
    include_directories(${TWinCAT_TcAdsDll}/Include)

    if( ${TARGET_PLATFORM_BITS} EQUAL 64 )
        link_directories(${TWinCAT_TcAdsDll}/x64/Lib)
    else()
        link_directories(${TWinCAT_TcAdsDll}/Lib)
    endif()

    add_executable(AdsLibTestRef AdsLibTestRef/main.cpp ${config_hpp})
    target_link_libraries (AdsLibTestRef TcAdsDll)

	# Examples
	add_executable(exampleRef exampleRef/example.cpp ${config_hpp})
	target_link_libraries (exampleRef TcAdsDll)
	
else()
	include_directories(./AdsLib) 

	# AdsLibTestls
	add_executable(AdsLibTest AdsLibTest/main.cpp ${config_hpp})
	target_link_libraries (AdsLibTest AdsLib ws2_32)

	# Examples
	add_executable(example example/example.cpp ${config_hpp})
	target_link_libraries (example AdsLib ws2_32)

endif()