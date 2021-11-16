cmake_minimum_required(VERSION 3.2)

# build number for travis
if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    project(allocator VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(allocator VERSION 0.0.1)
endif()

set(USE_TEST ON)

# boost dependensies
if (USE_TEST)
    find_package(Boost COMPONENTS unit_test_framework REQUIRED)
endif()

# source
set(LIB_SOURCE flat_allocator.h simple_list.h chunk_allocator.h
        simple_math.h simple_math.cpp
        )
set(ALLOCATOR_SOURCE main.cpp hard.h)

# targets and libraries
set(EXE_NAME allocator)
set(LIB_NAME allocator_lib)
if (USE_TEST)
    set(TEST_NAME test_allocator)
endif()
add_executable(${EXE_NAME} ${ALLOCATOR_SOURCE})
add_library(${LIB_NAME} ${LIB_SOURCE})
if (USE_TEST)
    add_executable(${TEST_NAME} test_allocator.cpp non_assignable_type.h)
endif()

# compiler options
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  set(CMP_OPTIONS "-Wall;-Wextra;-Wshadow;-Wnon-virtual-dtor;-Wold-style-cast;-Wcast-align;-Wunused;-Woverloaded-virtual;-Wconversion;-Wsign-conversion;-Wduplicated-cond")
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
target_link_libraries(${EXE_NAME}
    ${LIB_NAME}
)
if (USE_TEST)
    target_link_libraries(${TEST_NAME}
        ${Boost_LIBRARIES}
        ${LIB_NAME}
    )
endif()

# installation
install(TARGETS allocator RUNTIME DESTINATION bin)

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
