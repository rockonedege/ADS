# set compile flags for GCC

add_definitions(-std=c++11 -pedantic -wall -lpthread)

# copy from https://arkeon.dyndns.org/svn-scol/trunk/dependencies/aruco/CMakeLists.txt
if(MINGW)
    if(${CMAKE_SYSTEM_PROCESSOR} MATCHES amd64* OR ${CMAKE_SYSTEM_PROCESSOR} MATCHES x86_64*)
        add_definitions(-fPIC)
    endif()
endif()
