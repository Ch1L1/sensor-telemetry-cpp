if(EXISTS "/Users/tomashomola/projects/sensor-telemetry-cpp/build/ring_buffer_test")
  if(NOT EXISTS "/Users/tomashomola/projects/sensor-telemetry-cpp/build/ring_buffer_test[1]_tests.cmake" OR
     NOT "/Users/tomashomola/projects/sensor-telemetry-cpp/build/ring_buffer_test[1]_tests.cmake" IS_NEWER_THAN "/Users/tomashomola/projects/sensor-telemetry-cpp/build/ring_buffer_test" OR
     NOT "/Users/tomashomola/projects/sensor-telemetry-cpp/build/ring_buffer_test[1]_tests.cmake" IS_NEWER_THAN "${CMAKE_CURRENT_LIST_FILE}")
    include("/opt/homebrew/share/cmake/Modules/GoogleTestAddTests.cmake")
    gtest_discover_tests_impl(
      TEST_EXECUTABLE [==[/Users/tomashomola/projects/sensor-telemetry-cpp/build/ring_buffer_test]==]
      TEST_EXECUTOR [==[]==]
      TEST_WORKING_DIR [==[/Users/tomashomola/projects/sensor-telemetry-cpp/build]==]
      TEST_EXTRA_ARGS [==[]==]
      TEST_PROPERTIES [==[]==]
      TEST_PREFIX [==[]==]
      TEST_SUFFIX [==[]==]
      TEST_FILTER [==[]==]
      NO_PRETTY_TYPES [==[FALSE]==]
      NO_PRETTY_VALUES [==[FALSE]==]
      TEST_LIST [==[ring_buffer_test_TESTS]==]
      CTEST_FILE [==[/Users/tomashomola/projects/sensor-telemetry-cpp/build/ring_buffer_test[1]_tests.cmake]==]
      TEST_DISCOVERY_TIMEOUT [==[5]==]
      TEST_DISCOVERY_EXTRA_ARGS [==[]==]
      TEST_XML_OUTPUT_DIR [==[]==]
    )
  endif()
  include("/Users/tomashomola/projects/sensor-telemetry-cpp/build/ring_buffer_test[1]_tests.cmake")
else()
  add_test(ring_buffer_test_NOT_BUILT ring_buffer_test_NOT_BUILT)
endif()
