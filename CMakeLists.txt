cmake_minimum_required(VERSION 3.28)

# if(NOT LINUX) should work, too, if you need that
if(LINUX) 
    message(STATUS ">>> Linux")
    # linux stuff here
    set(CMAKE_C_COMPILER "/usr/bin/gcc")
    set(CMAKE_CXX_COMPILER "/usr/bin/g++")
    #set(CMAKE_CXX_COMPILER "C:/msys64/mingw64/bin/g++.exe")
    #set(CMAKE_PREFIX_PATH "C:/Users/kamilae/vcpkg/installed/x64-windows/share")
    #set(CMAKE_LINKER "/usr/bin/ld")
    #set(CMAKE_TOOLCHAIN_FILE "C:/Users/kamilae/vcpkg/scripts/buildsystems/vcpkg.cmake")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fno-eliminate-unused-debug-symbols -lcurl")
    set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "/usr/lib/x86_64-linux-gnu")
else()
    message(STATUS ">>> Not Linux")
    # stuff that should happen not on Linux 
    set(CMAKE_C_COMPILER "C:/msys64/mingw64/bin/gcc.exe")
    set(CMAKE_CXX_COMPILER "C:/msys64/mingw64/bin/g++.exe")
    #set(CMAKE_PREFIX_PATH "C:/Users/kamilae/vcpkg/installed/x64-windows/share")
    set(CMAKE_TOOLCHAIN_FILE "C:/Users/kamilae/vcpkg/scripts/buildsystems/vcpkg.cmake")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lcurl"),
endif()
#set(CMAKE_C_COMPILER "C:/msys64/mingw64/bin/gcc.exe")
#set(CMAKE_CXX_COMPILER "C:/msys64/mingw64/bin/g++.exe")
#set(CMAKE_PREFIX_PATH "C:/Users/kamilae/vcpkg/installed/x64-windows/share")
#set(CMAKE_TOOLCHAIN_FILE "C:/Users/kamilae/vcpkg/scripts/buildsystems/vcpkg.cmake")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lcurl"),

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined -fsanitize-trap")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=undefined -fsanitize-trap")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=undefined -fsanitize-trap")
#set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -fsanitize=undefined -fsanitize-trap")

project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

if(LINUX)
    find_package(Freetype)
    if(NOT Freetype_FOUND)
        message(FATAL_ERROR "Freetype not found")
    endif()
else()
    # Set the CURL_ROOT environment variable
    set(CURL_ROOT "C:/Users/kamilae/vcpkg/installed/x64-windows")

    # Specify the paths to the cURL library and include directories
    set(CURL_INCLUDE_DIR "${CURL_ROOT}/include")
    set(CURL_LIBRARY "${CURL_ROOT}/lib/libcurl.lib")
endif()

find_package(CURL REQUIRED)

add_executable(main src/main.cpp src/question.cpp src/button.cpp src/scroll_bar.cpp src/drop_down.cpp src/number_text_box.cpp src/radio_button.cpp)
if(LINUX)
    target_link_libraries(main PRIVATE sfml-graphics)
    target_link_libraries(main PUBLIC CURL::libcurl Freetype m)
    #target_link_libraries(main Freetype)
else()
    include_directories(${CURL_INCLUDE_DIR})
    target_link_libraries(main PRIVATE sfml-graphics)
    target_link_libraries(main PUBLIC CURL::libcurl)
endif()


