cmake_minimum_required(VERSION 3.10)
project(X-Test LANGUAGES CXX VERSION 1.0.0 DESCRIPTION "X-Test: Lightweight testing framework for C++")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(TARGETS_DIR ${CMAKE_BINARY_DIR}/X-Test)
set(INSTALL_DIR ${CMAKE_INSTALL_FULL_LIBDIR}/cmake/X-Test)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/inc)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

message(STATUS "X-Test: Building Library")
add_library(X-Test SHARED "${SOURCE_DIR}/TestBase.cpp")
set_target_properties(X-Test PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(X-Test PROPERTIES PUBLIC_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/X-Test.hpp")
target_include_directories(X-Test
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/inc"
)

install(TARGETS X-Test
    EXPORT X-TestTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
    PUBLIC_HEADER DESTINATION include
)

export(EXPORT X-TestTargets
    FILE ${TARGETS_DIR}/X-TestTargets.cmake
    NAMESPACE xtst::
)

install(EXPORT X-TestTargets
    FILE "X-TestTargets.cmake"
    NAMESPACE xtst::
    DESTINATION ${INSTALL_DIR}
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
    "${TARGETS_DIR}/X-TestConfig.cmake"
    INSTALL_DESTINATION ${INSTALL_DIR}
    PATH_VARS INCLUDE_DIR SOURCE_DIR TARGETS_DIR INSTALL_DIR
)

write_basic_package_version_file(
    "${TARGETS_DIR}/X-TestConfigVersion.cmake"
    VERSION "${PROJECT_VERSION}"
    COMPATIBILITY AnyNewerVersion
)

install(FILES
    "${TARGETS_DIR}/X-TestConfig.cmake"
    "${TARGETS_DIR}/X-TestConfigVersion.cmake"
    DESTINATION ${INSTALL_DIR}
)

message(STATUS "X-Test: Finished building")
