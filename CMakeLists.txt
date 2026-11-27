cmake_minimum_required(VERSION 3.24)
project(SensorTelemetry VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(ENABLE_SANITIZERS "Build with AddressSanitizer and UndefinedBehaviorSanitizer" OFF)

if(ENABLE_SANITIZERS)
  # Applied globally so GoogleTest and project code share the same instrumentation.
  add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer -fno-sanitize-recover=all)
  add_link_options(-fsanitize=address,undefined)
endif()

# Project warnings (not applied to third-party code).
add_library(telemetry_warnings INTERFACE)
target_compile_options(telemetry_warnings INTERFACE -Wall -Wextra -Werror)

# Header-only core library.
add_library(telemetry_core INTERFACE)
target_include_directories(telemetry_core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(telemetry_core INTERFACE telemetry_warnings)

# --- Tests -------------------------------------------------------------------
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.tar.gz
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
include(GoogleTest)

add_executable(ring_buffer_test tests/ring_buffer_test.cpp)
target_link_libraries(ring_buffer_test PRIVATE telemetry_core GTest::gtest_main)
# PRE_TEST: discover tests at ctest time instead of build time.
gtest_discover_tests(ring_buffer_test DISCOVERY_MODE PRE_TEST)
