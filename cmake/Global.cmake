# define a set of string with may-be useful readable name
# this file is meant to be included in a CMakeLists.txt
# not as a standalone CMake script

if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    MESSAGE(STATUS "64bit compiler detected" )
    SET( TARGET_PLATFORM_BITS 64 )
    SET( TARGET_PLATFORM_NAME "x64" )
else() 
    MESSAGE(STATUS "32bit compiler detected" )
    SET( TARGET_PLATFORM_BITS 32 )
    SET( TARGET_PLATFORM_NAME "x86" )
endif()


if(MSVC)
	include( ${CMAKE_HOME_DIRECTORY}/cmake/MSVC.cmake )
endif(MSVC)

if(CMAKE_COMPILER_IS_GNUCXX)
	include( ${CMAKE_HOME_DIRECTORY}/cmake/GCC.cmake )
endif(CMAKE_COMPILER_IS_GNUCXX)