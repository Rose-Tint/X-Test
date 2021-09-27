cmake_minimum_required ( VERSION 3.10 )

project ( XTest CXX VERSION 1.0 )

add_definitions(-std=c++17)

set ( CMAKE_CXX_STANDARD 17 )
set ( CMAKE_CXX_STANDARD_REQUIRED True )

if ( USE_X_TEST )
    add_subdirectory ( X-Test )
    list ( APPEND EXTRA_LIBS X-Test )
    list ( APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/X-Test" )
endif ( )

target_link_libraries ( XTest PUBLIC ${EXTRA_LIBS} )

target_include_directories (
    XTest PUBLIC
    "${PROJECT_SOURCE_DIR}/X-Test"
    ${EXTRA_INCLUDES}
)