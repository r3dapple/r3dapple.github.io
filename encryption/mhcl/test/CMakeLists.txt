IF(CMAKE_BUILD_TYPE MATCHES Debug)
    find_package (Boost COMPONENTS system filesystem unit_test_framework REQUIRED) # sudo apt-get install libboost-all-dev
    include_directories (${MHCL_SOURCE_DIR}/src ${Boost_INCLUDE_DIRS})

    add_executable (test_biginteger_constructor test_biginteger_constructor.cpp)
    target_link_libraries (test_biginteger_constructor biginteger ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

    add_executable (test_biginteger_add test_biginteger_add.cpp)
    target_link_libraries (test_biginteger_add biginteger ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

    add_executable (test_biginteger_subtract test_biginteger_subtract.cpp)
    target_link_libraries (test_biginteger_subtract biginteger ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

    add_executable (test_biginteger_multiply test_biginteger_multiply.cpp)
    target_link_libraries (test_biginteger_multiply biginteger ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

    add_executable (test_biginteger_divide test_biginteger_divide.cpp)
    target_link_libraries (test_biginteger_divide biginteger ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

    add_executable (test_biginteger_bitand test_biginteger_bitand.cpp)
    target_link_libraries (test_biginteger_bitand biginteger ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})


    include(CTest)
    enable_testing()

    add_test(NAME test_biginteger_constructor COMMAND test_biginteger_constructor)
    add_test(NAME test_biginteger_add COMMAND test_biginteger_add)
    add_test(NAME test_biginteger_subtract COMMAND test_biginteger_subtract)
    add_test(NAME test_biginteger_multiply COMMAND test_biginteger_multiply)
    add_test(NAME test_biginteger_divide COMMAND test_biginteger_divide)
    add_test(NAME test_biginteger_bitand COMMAND test_biginteger_bitand)

    include(CodeCoverage.cmake)
    APPEND_COVERAGE_COMPILER_FLAGS()
    setup_target_for_coverage_gcovr_html(
        NAME coverage 
        EXECUTABLE ctest --schedule-random -j 4 --test-dir test 
        EXCLUDE "/usr/*" "/mnt/v/r3dapple.github.io/encryption/mhcl/test/*" "/mnt/v/r3dapple.github.io/encryption/mhcl/src/main.cpp" 
        DEPENDENCIES test_biginteger_constructor test_biginteger_add test_biginteger_multiply test_biginteger_divide test_biginteger_bitand)
ENDIF()
