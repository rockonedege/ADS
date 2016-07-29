# set compile flags for MSVC

add_definitions(/DUNICODE /D_UNICODE)
add_definitions(/W3)
add_definitions(/EHsc /Bt)
#add_definitions(/wd4819 /wd4345 /wd4251) # disable warnings
add_definitions(/MP)        #enable concurrent build
#add_definitions(/P)        #preprocessor
#add_definitions(/ZI /Gm )  #enanble an incremental rebuild, incompatible with the /MP

#from http://stackoverflow.com/questions/14172856/cmake-compile-with-mt-instead-of-md
set(CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
        )
foreach(CompilerFlag ${CompilerFlags})
  string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
endforeach()
