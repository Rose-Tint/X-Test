set(X_TEST_VERSION @VERSION@)

@PACKAGE_INIT@

set_and_check(X_TEST_INCLUDE_DIR "@PACKAGE_INCLUDE_INSTALL_DIR@")
set_and_check(X_TEST_LIBRARY_DIR "@PACKAGE_LIBRARY_INSTALL_DIR@")
set_and_check(X_TEST_LIBRARY "@PACKAGE_LIBRARY_INSTALL_DIR@/libX_TEST.so")
set_and_check(X_TEST_STATIC_LIBRARY @PACKAGE_LIBRARY_INSTALL_DIR@/libX_TEST.a")
include("${CMAKE_CURRENT_LIST_DIR}/X_TESTLibTargets.cmake")

message(STATUS "X_TEST version: ${X_TEST_VERSION}")
message(STATUS "X_TEST include location: ${X_TEST_INCLUDE_DIR}")
message(STATUS "X_TEST library location: ${X_TEST_LIBRARY_DIR}")

check_required_components(X-Test)
