cmake_minimum_required(VERSION 3.2)
project(Ads VERSION 1.1.5.0)

set(CMAKE_VERBOSE_MAKEFILE ON)

# load local scripts
include (cmake/Global.cmake)       
 
include_directories(./AdsLib) 
include_directories(./3rd) 

# for AdsLib
set(adslib_root ./AdsLib)
set(lib_hpp ${adslib_root}/AdsDef.h ${adslib_root}/AdsLib.h ${adslib_root}/AdsNotification.h ${adslib_root}/AmsConnection.h ${adslib_root}/AmsHeader.h ${adslib_root}/AmsPort.h ${adslib_root}/AmsRouter.h ${adslib_root}/Frame.h ${adslib_root}/Log.h ${adslib_root}/NotificationDispatcher.h ${adslib_root}/RingBuffer.h ${adslib_root}/Router.h ${adslib_root}/Semaphore.h ${adslib_root}/Sockets.h ${adslib_root}/wrap_endian.h ${adslib_root}/wrap_socket.h)

set(lib_cpp ${adslib_root}/AdsDef.cpp ${adslib_root}/AdsLib.cpp ${adslib_root}/AmsConnection.cpp ${adslib_root}/AmsPort.cpp ${adslib_root}/AmsRouter.cpp ${adslib_root}/Frame.cpp ${adslib_root}/Log.cpp ${adslib_root}/NotificationDispatcher.cpp ${adslib_root}/Sockets.cpp)

# AdsLib
add_library(AdsLib ${lib_cpp})

# Examples
add_executable(example example/example.cpp)
target_link_libraries (example AdsLib ws2_32)

# AdsLibTest
add_executable(AdsLibTest AdsLibTest/main.cpp)
target_link_libraries (AdsLibTest AdsLib ws2_32)

# AdsLibTestRef
set(TWinCAT_TcAdsDll C:/TwinCAT/AdsApi/TcAdsDll)
include_directories(${TWinCAT_TcAdsDll}/Include)
if( ${TARGET_PLATFORM_BITS} EQUAL 64 )
    link_directories(${TWinCAT_TcAdsDll}/x64/Lib)
else()
    link_directories(${TWinCAT_TcAdsDll}/Lib)
endif()
add_executable(AdsLibTestRef AdsLibTestRef/main.cpp)
target_link_libraries (AdsLibTestRef TcAdsDll ws2_32)