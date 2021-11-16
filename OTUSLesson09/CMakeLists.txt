cmake_minimum_required(VERSION 3.2)

# build number for travis
if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    project(print_ip VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(print_ip VERSION 0.0.1)
endif()

set(USE_TEST ON)

# boost dependensies
if (USE_TEST)
    find_package(Boost COMPONENTS unit_test_framework REQUIRED)
endif()

# source
set(EXE_SOURCE main.cpp print_ip.h)
set(TEST_SOURCE test_print_ip.cpp)

# targets and libraries
set(EXE_NAME print_ip)
if (USE_TEST)
    set(TEST_NAME test_print_ip)
endif()
add_executable(${EXE_NAME} ${EXE_SOURCE})
if (USE_TEST)
    add_executable(${TEST_NAME} ${TEST_SOURCE})
endif()

# compiler options
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  set(CMP_OPTIONS "-Wpedantic;-Wall;-Wextra;-D_GLIBCXX_DEBUG;-D_GLIBCXX_DEBUG_PEDANTIC")
else()
  set(CMP_OPTIONS "/W4")
endif()

# target properties
set_target_properties(${EXE_NAME} ${TEST_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS ${CMP_OPTIONS}
)

# add boost headers for test
if (USE_TEST)
    if(UNIX)
        set_target_properties(${TEST_NAME} PROPERTIES
            COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
        )
    endif()

    target_include_directories(${TEST_NAME}
            PRIVATE ${Boost_INCLUDE_DIR}
            )
endif()

# target linking
if (USE_TEST)
    target_link_libraries(${TEST_NAME}
        ${Boost_LIBRARIES}
    )
endif()

# installation
install(TARGETS print_ip RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT gwrass@mail.com)

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)

# tests
if (USE_TEST)
    enable_testing()
    add_test(${TEST_NAME} ${TEST_NAME})
endif()
