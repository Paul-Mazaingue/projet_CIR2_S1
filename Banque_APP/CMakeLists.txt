cmake_minimum_required(VERSION 3.18)
project(Banque_APP)

if(MSVC)
    set(wxWidgets_ROOT_DIR  "C:/tools/wxWidgets")
    set(CMAKE_PREFIX_PATH  C:/tools/wxWidgets)
    add_compile_options($<$<CONFIG:Release>:/MT> # Runtime library: Multi-threaded
                      $<$<CONFIG:Debug>:/MTd> # Runtime library: Multi-threaded Debug
                      )
endif (MSVC)

find_package(wxWidgets REQUIRED COMPONENTS net core base)
include(${wxWidgets_USE_FILE})

add_executable(Banque_APP Banque_APP.cpp   )

if(MSVC)
    set_target_properties(Banque_APP PROPERTIES LINK_FLAGS /SUBSYSTEM:WINDOWS)
    set_property(TARGET Banque_APP PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif (MSVC)

target_link_libraries(Banque_APP ${wxWidgets_LIBRARIES})

set(Boost_ROOT "C:/tools/Boost")
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(Banque_APP ${Boost_LIBRARIES})