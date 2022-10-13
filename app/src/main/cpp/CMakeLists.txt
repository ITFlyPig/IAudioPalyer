# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.18.1)

# Declares and names the project.
project("iaudiopalyer")

set(THIRDPARTY_DIR ${CMAKE_SOURCE_DIR}/thirdparty)
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

set(FFMPEG_DIR ${THIRDPARTY_DIR}/ffmpeg)
set(FFMPEG_INCLUDE_DIR ${FFMPEG_DIR}/include)
set(FFMPEG_LIB_DIR ${FFMPEG_DIR}/lib/${ANDROID_ABI})

# 添加头文件
include_directories(${THIRDPARTY_DIR}/ffmpeg/include)
include_directories(${SOURCE_DIR})
include_directories(${SOURCE_DIR}/jni)


# 添加CPP文件
file(GLOB SOURCE_FILES
        ${SOURCE_DIR}/jni/*.cpp
        ${SOURCE_DIR}/*.cpp
        )


add_library(
        iaudiopalyer
        SHARED
        native-lib.cpp  ${SOURCE_FILES} src/PcmFrame.cpp src/PcmFrame.cpp src/Releaser.cpp)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library(log-lib log)

find_package(oboe REQUIRED CONFIG)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        iaudiopalyer
        ${log-lib}
        oboe::oboe
        # 引入ffmpeg相关静态库
        -lz
        ${FFMPEG_LIB_DIR}/libavformat.a
        ${FFMPEG_LIB_DIR}/libavfilter.a
        ${FFMPEG_LIB_DIR}/libavcodec.a
        ${FFMPEG_LIB_DIR}/libpostproc.a
        ${FFMPEG_LIB_DIR}/libavutil.a
        ${FFMPEG_LIB_DIR}/libswresample.a
        ${FFMPEG_LIB_DIR}/libswscale.a
        ${FFMPEG_LIB_DIR}/libpostproc.a
        ${FFMPEG_LIB_DIR}/libfdk-aac.a
        ${FFMPEG_LIB_DIR}/libvo-aacenc.a
        ${FFMPEG_LIB_DIR}/libx264.a
        )