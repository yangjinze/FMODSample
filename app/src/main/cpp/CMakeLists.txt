# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

find_library( log-lib
        log )


#set(my_lib_path ${PROJECT_SOURCE_DIR}/libs)     # PROJECT_SOURCE_DIR等都是为当前cmakelists.txtx的路径
set(my_lib_path E:/MyGit/NDK/FMODSample/app/libs) # 怎么简单表示？？？,避免需要输入绝对路径
message(${my_lib_path}) #打印出来查看

# 添加三方的so库
add_library(libfmod
        SHARED
        IMPORTED )
# 指名第三方库的绝对路径
set_target_properties( libfmod
        PROPERTIES IMPORTED_LOCATION
        ${my_lib_path}/${ANDROID_ABI}/libfmod.so )

add_library(libfmodL
        SHARED
        IMPORTED )

set_target_properties( libfmodL
        PROPERTIES IMPORTED_LOCATION
        ${my_lib_path}/${ANDROID_ABI}/libfmodL.so )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

# 添加我们需要写代码的路径
add_library(fmodtools
        SHARED
        fmodtools.cpp )

# 导入路径，为了让编译时能够寻找到这个文件夹
include_directories(src/main/cpp/inc)

# 链接好三个路径
target_link_libraries( fmodtools
        libfmod
        libfmodL
        ${log-lib} )











## Creates and names a library, sets it as either STATIC
## or SHARED, and provides the relative paths to its source code.
## You can define multiple libraries, and CMake builds them for you.
## Gradle automatically packages shared libraries with your APK.
#
#add_library( # Sets the name of the library.
#        fmodtools-lib
#        # Sets the library as a shared library.
#        SHARED
#        # Provides a relative path to your source file(s).
#        fmodtools.cpp)
#
## Searches for a specified prebuilt library and stores the path as a
## variable. Because CMake includes system libraries in the search path by
## default, you only need to specify the name of the public NDK library
## you want to add. CMake verifies that the library exists before
## completing its build.
#
#find_library( # Sets the name of the path variable.
#        log-lib
#
#        # Specifies the name of the NDK library that
#        # you want CMake to locate.
#        log)
#
#
#set(my_lib_path ${CMAKE_SOURCE_DIR}/libs)
## 添加三方的so库
#add_library(libfmod
#        SHARED
#        IMPORTED )
## 指名第三方库的绝对路径
#set_target_properties( libfmod
#        PROPERTIES IMPORTED_LOCATION
#        ${my_lib_path}/${ANDROID_ABI}/libfmod.so )
#
#add_library(libfmodL
#        SHARED
#        IMPORTED )
#
#set_target_properties( libfmodL
#        PROPERTIES IMPORTED_LOCATION
#        ${my_lib_path}/${ANDROID_ABI}/libfmodL.so )
#
## 导入路径，为了让编译时能够寻找到这个文件夹
#include_directories(src/main/cpp/inc)
##include_directories(inc) TODO 无效？
#
## Specifies libraries CMake should link to your target library. You
## can link multiple libraries, such as libraries you define in this
## build script, prebuilt third-party libraries, or system libraries.
#
#target_link_libraries( # Specifies the target library.
#        fmodtools-lib
#        libfmod
#        libfmodL
#        # Links the target library to the log library
#        # included in the NDK.
#        ${log-lib})

