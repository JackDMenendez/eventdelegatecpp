# CMakeList.txt : CMake project for eventdelegatecpp, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.16)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project (eventdelegatecpp
         VERSION 0.0.2
         LANGUAGES CXX)

include(CTest)
include(cmake/eventdelegateOptions.cmake)

# ---- Warning guard ----

# Protect dependents from this project's warnings if the guard isn't disabled
set(eventdelegatecpp_warning_guard "SYSTEM")
if(eventdelegatecpp_INCLUDE_WITHOUT_SYSTEM)
  set(eventdelegatecpp_warning_guard "")
endif()

# tell CMake to use CTest extension
enable_testing()

# ---- Library ----

set(eventdelegatecpp_sources
        "source/default_sender.cpp"
) 

set(eventdelegatecpp_headers
        "include/event_delegate.h"
        "include/senders/default_sender.h" 
        "include/util/ed.h" 
        "include/util/ed_defs.h" 
        "include/util/forward.h"
)

# ---- Test Exec ----

set(eventdelegatecpp_test_source
        "test/source/tests/SubscriptionTest.cpp"
 "test/include/util/ghelper.h")

set(eventdeletatecpp_test_headers
        "test/include/util/unit_test.h"
)

add_library(eventdelegatecpp ${eventdelegatecpp_headers} ${eventdelegatecpp_sources})
add_library(eventdelegatecpp::eventdelegatecpp ALIAS eventdelegatecpp)

target_include_directories(eventdelegatecpp
        ${eventdelegatecpp_warning_guard}
        PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>")

target_compile_features(eventdelegatecpp PUBLIC cxx_std_23)

set_target_properties(eventdelegatecpp PROPERTIES
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN ON
        VERSION "${PROJECT_VERSION}"
        SOVERSION "${PROJECT_VERSION_MAJOR}"
)

# ----
# This is the place to insert required packages and libraries.
# ----
# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# Now simply link against gtest or gtest_main as needed. Eg
add_executable(EDUnitGTest
     ${eventdelegatecpp_test_source} 
     ${eventdelegatecpp_test_headers} 
     ${eventdelegatecpp_headers}
)
target_include_directories(EDUnitGTest ${eventdelegatecpp_warning_guard}
        PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)
target_include_directories(EDUnitGTest ${eventdelegatecpp_warning_guard}
        PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/test/include>"
)
target_compile_features(EDUnitGTest PUBLIC cxx_std_23)
target_link_libraries(EDUnitGTest gtest_main)
add_test(NAME unit_test COMMAND EDUnitGTest)

target_compile_definitions(eventdelegatecpp
        PRIVATE $<$<STREQUAL:$<TARGET_PROPERTY:eventdelegatecpp,TYPE>,SHARED_LIBRARY>:CRCPP_EXPORT_API>
        INTERFACE $<$<STREQUAL:$<TARGET_PROPERTY:eventdelegatecpp,TYPE>,SHARED_LIBRARY>:CRCPP_IMPORT_API>
)

# ---- Install ----

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

set(eventdelegatecpp_directory "eventdelegatecpp-${PROJECT_VERSION}")
set(eventdelegatecpp_include_directory "${CMAKE_INSTALL_INCLUDEDIR}/${eventdelegatecpp_directory}")

install(
  TARGETS eventdelegatecpp
  EXPORT eventdelegatecppTargets
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
          COMPONENT eventdelegatecpp_Development
  INCLUDES
  DESTINATION "${eventdelegatecpp_include_directory}"
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
          COMPONENT eventdelegatecpp_Runtime
          NAMELINK_COMPONENT eventdelegatecpp_Development
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT eventdelegatecpp_Runtime)

  set(eventdelegatecpp_install_cmakedir
        "${CMAKE_INSTALL_LIBDIR}/cmake/${eventdelegatecpp_directory}")

write_basic_package_version_file(
        eventdelegatecppConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMinorVersion
        ARCH_INDEPENDENT)

install(
  EXPORT eventdelegatecppTargets
  NAMESPACE eventdelegatecpp::
  DESTINATION "${eventdelegatecpp_install_cmakedir}"
  COMPONENT eventdelegatecpp_Development)

install(
  FILES "${PROJECT_SOURCE_DIR}/cmake/eventdelegatecppConfig.cmake"
        "${PROJECT_BINARY_DIR}/eventdelegatecppConfigVersion.cmake"
  DESTINATION "${eventdelegatecpp_install_cmakedir}"
  COMPONENT eventdelegatecpp_Development)

install(
  DIRECTORY "${PROJECT_SOURCE_DIR}/include/"
  DESTINATION "${eventdelegatecpp_include_directory}"
  COMPONENT eventdelegatecpp_Development)
