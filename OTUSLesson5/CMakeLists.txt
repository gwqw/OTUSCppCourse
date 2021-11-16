cmake_minimum_required(VERSION 3.2)

# build number for travis
if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    project(range VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(range VERSION 0.0.1)
endif()

# boost dependensies
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

# conan dependencies
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# executable target
add_executable(range main.cpp ip_filter.h)
add_library(ip_filter_lib ip_filter.cpp ip_filter.h  str_utils.cpp str_utils.h)
add_executable(test_ip_filter test_filter.cpp ip_filter.h)

# compiler options
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  set(CMP_OPTIONS "-Wpedantic;-Wall;-Wextra")
else()
  set(CMP_OPTIONS "/W4")
endif()

# target properties
set_target_properties(range ip_filter_lib test_ip_filter PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS ${CMP_OPTIONS}
)

if(UNIX)
    set_target_properties(test_ip_filter PROPERTIES
        COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    )
endif()

set_target_properties(range PROPERTIES
    INCLUDE_DIRECTORIES ${CMAKE_MODULE_PATH}/include
)

set_target_properties(test_ip_filter PROPERTIES
    INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
    INCLUDE_DIRECTORIES ${CMAKE_MODULE_PATH}/include
)

# target linking
target_link_libraries(range
    ip_filter_lib
    ${CONAN_LIBS}
)

target_link_libraries(test_ip_filter
    ${Boost_LIBRARIES}
    ${CONAN_LIBS}
    ip_filter_lib
)

# installation
install(TARGETS range RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT gwrass@mail.com)

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)

# tests
enable_testing()

add_test(ip_tests bin/test_ip_filter)
