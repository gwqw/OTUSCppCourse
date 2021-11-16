cmake_minimum_required(VERSION 3.2)

# build number for travis
if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    project(realty VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(realty VERSION 0.0.1)
endif()

set(USE_TEST ON)

# boost dependensies
if (USE_TEST)
    find_package(Boost COMPONENTS unit_test_framework REQUIRED)
endif()

# conan dependencies
if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
endif()

# source
set(SOURCE
    input_parser.cpp input_parser.h
    realty_types.h
    str_utils.cpp str_utils.h
)
set(EXE_SOURCE1 rclst_main.cpp trainer.cpp trainer.h ${SOURCE})
set(EXE_SOURCE2 rclss_main.cpp predictor.cpp predictor.h ${SOURCE})
set(EXE_SOURCE3 rcl_test_main.cpp predictor.cpp predictor.h ${SOURCE})
if (USE_TEST)
    set(TEST_SOURCE test_realty.cpp ${SOURCE})
endif()

# targets and libraries
set(EXE_NAME1 rclst)
set(EXE_NAME2 rclss)
set(EXE_NAME3 rcl_test)
if (USE_TEST)
    set(TEST_NAME test_realty)
endif()
add_executable(${EXE_NAME1} ${EXE_SOURCE1})
add_executable(${EXE_NAME2} ${EXE_SOURCE2})
add_executable(${EXE_NAME3} ${EXE_SOURCE3})
if (USE_TEST)
    add_executable(${TEST_NAME} ${TEST_SOURCE})
endif()

# compiler options
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  set(CMP_OPTIONS "-Wall;-Wextra;-Wshadow;-Wnon-virtual-dtor;-Wold-style-cast;-Wcast-align;-Wunused;-Woverloaded-virtual;-Wconversion;-Wsign-conversion;-Wduplicated-cond")
  if(CMAKE_BUILD_TYPE MATCHES Debug)
      #set(CMP_OPTIONS ${CMP_OPTIONS}";-D_GLIBCXX_DEBUG;-D_GLIBCXX_DEBUG_PEDANTIC")
      set(CMP_OPTIONS ${CMP_OPTIONS}";-fsanitize=address;")
      message("debug")
  endif()
else()
  set(CMP_OPTIONS "/W4")
endif()

# target properties
set_target_properties(${EXE_NAME1} ${EXE_NAME2} ${EXE_NAME3} ${TEST_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS ${CMP_OPTIONS}
)

# add headers
target_include_directories(${EXE_NAME1}
        PRIVATE ${CMAKE_MODULE_PATH}/include
)
target_include_directories(${EXE_NAME2}
        PRIVATE ${CMAKE_MODULE_PATH}/include
)
target_include_directories(${EXE_NAME3}
        PRIVATE ${CMAKE_MODULE_PATH}/include
)

if (USE_TEST)
    if(UNIX)
        set_target_properties(${TEST_NAME} PROPERTIES
            COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
        )
    endif()
    target_include_directories(${TEST_NAME}
            PRIVATE ${Boost_INCLUDE_DIR} ${CMAKE_MODULE_PATH}/include
    )
endif()

# target linking
if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    target_link_libraries(${EXE_NAME1}
        ${CONAN_LIBS} openblas
    )
    target_link_libraries(${EXE_NAME2}
        ${CONAN_LIBS} openblas
    )
    target_link_libraries(${EXE_NAME3}
        ${CONAN_LIBS} openblas
    )
else()
    target_link_libraries(${EXE_NAME1}
        dlib openblas
    )
    target_link_libraries(${EXE_NAME2}
        dlib openblas
    )
    target_link_libraries(${EXE_NAME3}
        dlib openblas
    )
endif()

if (USE_TEST)
    if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
        target_link_libraries(${TEST_NAME}
            ${CONAN_LIBS} openblas ${Boost_LIBRARIES}
        )
    else()
        target_link_libraries(${TEST_NAME}
            dlib openblas ${Boost_LIBRARIES}
        )
    endif()
endif()

# installation
install(TARGETS ${EXE_NAME1} ${EXE_NAME2} RUNTIME DESTINATION bin)

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
    add_test(${TEST_NAME} bin/${TEST_NAME})
endif()
