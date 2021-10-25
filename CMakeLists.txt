cmake_minimum_required(VERSION 3.10.2)
project(X-Test
    LANGUAGES CXX
    VERSION 1.0.0
    DESCRIPTION "X-Test: Lightweight testing framework for C++"
)

####################################################################################################
########################################### REQUIREMENTS ###########################################
####################################################################################################


# C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)


####################################################################################################
########################################## PATH VARIABLES ##########################################
####################################################################################################


# set directories
include(GNUInstallDirs)
set(BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(${PROJECT_NAME}_DIR ${BUILD_DIR} PARENT_SCOPE)
set(TARGETS_FNAME "${PROJECT_NAME}Targets")
set(TARGETS_FPATH ${BUILD_DIR}/${TARGETS_FNAME}.cmake)

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/inc)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# searching paths for `find_package()` are different for unix and windows
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(INSTALL_DIR cmake/${PROJECT_NAME})
else()
    set(INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
endif()

set(INS_LIB_DIR ${INSTALL_DIR}/lib)
set(INS_INC_DIR ${INSTALL_DIR}/include)
set(INS_BIN_DIR ${INSTALL_DIR}/bin)

# set files
set(PRIVATE_INCLUDES ${INCLUDE_DIR}/stl_includes.hpp ${INCLUDE_DIR}/TestBase.hpp)
set(PUBLIC_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.hpp )
set(SOURCE_FILES ${SOURCE_DIR}/TestBase.cpp)


####################################################################################################
############################################# BUILDING #############################################
####################################################################################################


message(STATUS "${PROJECT_NAME}: Building")

add_library(${PROJECT_NAME} ${SOURCE_FILES} ${PRIVATE_INCLUDES} ${PUBLIC_INCLUDES})

target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${INCLUDE_DIR}>
    $<BUILD_INTERFACE:${SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    ${INS_INC_DIR}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    LINKER_LANGUAGE CXX
    PUBLIC_HEADER ${PUBLIC_INCLUDES}
    PRIVATE_HEADER ${PRIVATE_INCLUDES} ${SOURCE_FILES}
)


####################################################################################################
########################################### INSTALLATION ###########################################
####################################################################################################


message(STATUS "${PROJECT_NAME}: Installing")

# creates export to hold compilation requirements
install(TARGETS ${PROJECT_NAME}
    EXPORT ${TARGETS_FNAME}
    RUNTIME DESTINATION ${INS_BIN_DIR}
    LIBRARY DESTINATION ${INS_LIB_DIR}
    ARCHIVE DESTINATION ${INS_LIB_DIR}
    PUBLIC_HEADER DESTINATION ${INS_INC_DIR}
    PRIVATE_HEADER DESTINATION ${INS_INC_DIR}
)

message(STATUS "TARGETS_FPATH=${TARGETS_FPATH}")
message(STATUS "INSTALL_DIR=${CMAKE_INSTALL_PREFIX}")

# install the export into INSTALL_DIR
install(EXPORT ${TARGETS_FNAME}
    DESTINATION ${TARGETS_FPATH}
    NAMESPACE xtst::
)


####################################################################################################
########################################## PACKAGE CONFIG ##########################################
####################################################################################################


include(CMakePackageConfigHelpers)
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in"
    "${BUILD_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION ${INS_LIB_DIR}
    PATH_VARS INCLUDE_DIR SOURCE_DIR TARGETS_FPATH
)
write_basic_package_version_file(
    "${BUILD_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION "${PROJECT_VERSION}"
    COMPATIBILITY AnyNewerVersion
)

install(FILES
    "${BUILD_DIR}/${PROJECT_NAME}Config.cmake"
    "${BUILD_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION ${INSTALL_DIR}
)

message(STATUS "${PROJECT_NAME}: Finished building")

