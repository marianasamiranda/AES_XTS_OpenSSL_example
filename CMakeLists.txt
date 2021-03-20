# ---------------------------------------------------------------------------- #

cmake_minimum_required(VERSION 3.11)
include(FetchContent)

project(aesxts VERSION 1.0.0 DESCRIPTION "aes-xts: ...")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Setup the options that CMake can take in
option(AESXTS_INSTALL "Install AES XTS's header and library" ON)
option(AESXTS_BUILD_TESTS "Build AES XTS's unit tests" ON)

# Setup the basic C++ Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -lssl -lcrypto")

# Test whether -Wthread-safety is available. See
# https://clang.llvm.org/docs/ThreadSafetyAnalysis.html
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag(-Wthread-safety HAVE_CLANG_THREAD_SAFETY)

# ---------------------------------------------------------------------------- #
# aes-xts

add_library(aesxts SHARED "")

set_target_properties(
        aesxts
        PROPERTIES
        # CXX_VISIBILITY_PRESET hidden
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
)

target_include_directories(aesxts PUBLIC include)

if (HAVE_CLANG_THREAD_SAFETY)
    target_compile_options(aesxts PUBLIC -Wthread-safety)
endif (HAVE_CLANG_THREAD_SAFETY)

find_package(Threads REQUIRED)
target_link_libraries(aesxts Threads::Threads)

find_package(OpenSSL REQUIRED)
target_link_libraries(aesxts OpenSSL::SSL)
target_link_libraries(aesxts OpenSSL::Crypto)

target_sources(
        aesxts
        PUBLIC
        ${PROJECT_SOURCE_DIR}/include/aesxts/xts.hpp
)

target_sources(
        aesxts
        PRIVATE
        src/xts.cpp
)

if (AESXTS_BUILD_TESTS)
    enable_testing()

    function(aesxts_test test_file)
        get_filename_component(test_target_name "${test_file}" NAME_WE)

        add_executable("${test_target_name}" "")
        target_sources("${test_target_name}"
                PRIVATE
                "${test_file}"
                )

        target_link_libraries("${test_target_name}" aesxts)

        add_test(NAME "${test_target_name}" COMMAND "${test_target_name}")
    endfunction(aesxts_test)

    aesxts_test("tests/example.cpp")

endif (AESXTS_BUILD_TESTS)

# ---------------------------------------------------------------------------- #
# install

if (AESXTS_INSTALL)
    include(GNUInstallDirs)
    install(
            TARGETS aesxts
            EXPORT aesxtsTargets
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
    install(DIRECTORY include/xts TYPE INCLUDE)
endif (AESXTS_INSTALL)

# ---------------------------------------------------------------------------- #
