cmake_minimum_required(VERSION 3.16)


project(sandbox LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(eventdelegatecpp SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/..")
FetchContent_MakeAvailable(eventdelegatecpp)

include(../cmake/eventdelegateOptions.cmake)

add_executable(sandbox main.cpp)

target_compile_features(sandbox PRIVATE cxx_std_20)

target_link_libraries(sandbox PRIVATE eventdelegatecpp::eventdelegatecpp)

target_eventdelegate_options(sandbox)