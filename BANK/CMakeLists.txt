cmake_minimum_required(VERSION 3.18)
project(bank)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_WIN32_WINNT=0x0601")
set (CMAKE_CXX_STANDARD 17)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_RELEASE_LIBS ON)

if(MSVC)
    set(wxWidgets_ROOT_DIR  "C:/tools/wxWidgets")
    set(CMAKE_PREFIX_PATH  C:/tools/wxWidgets)
    add_compile_options($<$<CONFIG:Release>:/MT> # Runtime library: Multi-threaded
                      $<$<CONFIG:Debug>:/MTd> # Runtime library: Multi-threaded Debug
                      )
endif (MSVC)

find_package(wxWidgets REQUIRED COMPONENTS net core base)
include(${wxWidgets_USE_FILE})



if(MSVC)
    set_target_properties(bank PROPERTIES LINK_FLAGS /SUBSYSTEM:WINDOWS)
    set_property(TARGET bank PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif (MSVC)
find_package(Boost 1.80.0 COMPONENTS system thread regex serialization REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})
target_link_libraries(bank ${wxWidgets_LIBRARIES})

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_RELEASE_LIBS ON)
set(Boost_ROOT "C:/tools/Boost")
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

if(MSVC)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    set(Boost_USE_STATIC_RUNTIME ON)
    add_compile_options($<$<CONFIG:Release>:/MT> # Runtime library: Multi-threaded
                      $<$<CONFIG:Debug>:/MTd> # Runtime library: Multi-threaded Debug
                      )
    set(Boost_DIR "C:/tools/Boost/lib/cmake/Boost-1.80.0")
elseif(MSVC)
    set(Boost_DIR /usr/local/lib/cmake/Boost-1.80.0)
endif (MSVC)

add_executable(serveur "serveur.cpp")
add_executable(berlin "berlin.cpp")
add_executable(paris "paris.cpp")
add_executable(londre "londre.cpp")
add_executable(bank "bank.cpp")

target_link_libraries(bank ${Boost_LIBRARIES})
target_link_libraries(serveur ${Boost_LIBRARIES})
target_link_libraries(berlin ${Boost_LIBRARIES})
target_link_libraries(paris ${Boost_LIBRARIES})
target_link_libraries(londre ${Boost_LIBRARIES})