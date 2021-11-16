cmake_minimum_required(VERSION 3.2)

if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    project(ip_filter VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(ip_filter VERSION 0.0.1)
endif()

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

add_executable(ip_filter main.cpp)
add_library(ip_filter_lib ip_filter.cpp ip_filter.h iterator_range.h 
    str_utils.cpp str_utils.h)
add_executable(test_ip_filter test_filter.cpp)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  set(CMP_OPTIONS "-Wpedantic;-Wall;-Wextra")
else()
  set(CMP_OPTIONS "/W4")
endif()

set_target_properties(ip_filter ip_filter_lib test_ip_filter PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS ${CMP_OPTIONS}
)

if(UNIX)
set_target_properties(test_ip_filter PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
)
endif()

set_target_properties(test_ip_filter PROPERTIES
    INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)

target_link_libraries(ip_filter
    ip_filter_lib
)

target_link_libraries(test_ip_filter
    ${Boost_LIBRARIES}
    ip_filter_lib
)

install(TARGETS ip_filter RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT gwrass@mail.com)

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)

enable_testing()

add_test(ip_tests test_ip_filter)    
