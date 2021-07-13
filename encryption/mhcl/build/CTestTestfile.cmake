# CMake generated Testfile for 
# Source directory: /mnt/v/r3dapple.github.io/encryption/mhcl
# Build directory: /mnt/v/r3dapple.github.io/encryption/mhcl/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(BITest1 "/mnt/v/r3dapple.github.io/encryption/mhcl/build/test/biginteger_mult_test")
set_tests_properties(BITest1 PROPERTIES  _BACKTRACE_TRIPLES "/mnt/v/r3dapple.github.io/encryption/mhcl/CMakeLists.txt;7;add_test;/mnt/v/r3dapple.github.io/encryption/mhcl/CMakeLists.txt;0;")
subdirs("test")
subdirs("src")
