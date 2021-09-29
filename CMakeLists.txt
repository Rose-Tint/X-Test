cmake_minimum_required(VERSION 3.10)
project(X-Test LANGUAGES CXX VERSION 1.0.0 DESCRIPTION "X-Test: Logger for C++")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

install(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/errors.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/format.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/handler.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/logger.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/stl_includes.hpp
    DESTINATION include ${CMAKE_INSTALL_INCLUDEDIR}
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

message(STATUS "X-Test: Building Library")
add_library(X-Test SHARED X-Test.hpp)
set_target_properties(X-Test PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(X-Test PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/inc")

install(TARGETS X-Test
    EXPORT X-TestTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(EXPORT X-TestTargets
    FILE X-TestTargets.cmake
    NAMESPACE xtst::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/X-Test
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/X-TestConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/X-Test.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/X-Test
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/X-TestConfig.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/X-Test
)
