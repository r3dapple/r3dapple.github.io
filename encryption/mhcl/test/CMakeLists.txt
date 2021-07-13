find_package (Boost COMPONENTS system filesystem unit_test_framework REQUIRED)
include_directories (${MHCL_SOURCE_DIR}/src ${Boost_INCLUDE_DIRS})
add_definitions (-DBOOST_TEST_DYN_LINK)

add_executable (test_biginteger test_biginteger.cpp)
target_link_libraries (test_biginteger biginteger ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

#add_executable (test_aes test_aes.cpp)
#target_link_libraries (test_aes aes biginteger ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})