add_executable( CliffordAlgebra_test "CliffordAlgebra_test.cpp" )
add_test( CliffordAlgebra_testAlgebra CliffordAlgebra_test )
set_tests_properties( CliffordAlgebra_testAlgebra PROPERTIES PASS_REGULAR_EXPRESSION  "3 matrices squared to unity." )
set_tests_properties( CliffordAlgebra_testAlgebra PROPERTIES PASS_REGULAR_EXPRESSION  "3  correct products." )
set_tests_properties( CliffordAlgebra_testAlgebra PROPERTIES PASS_REGULAR_EXPRESSION  "3 commutator relations worked." )

add_executable( Lattice_test "Lattice_test.cpp" )
target_link_libraries( Lattice_test DataCommon )

add_executable( Field_test "Field_test.cpp" )
target_link_libraries( Field_test DataCommon)
add_test( Field_testRuns Field_test )
set_tests_properties (Field_testRuns PROPERTIES PASS_REGULAR_EXPRESSION "Initialization with 0 and 1 works!")
set_tests_properties (Field_testRuns PROPERTIES PASS_REGULAR_EXPRESSION "Initialization by copy works!")
set_tests_properties (Field_testRuns PROPERTIES PASS_REGULAR_EXPRESSION "Copy assignment works!")

add_executable( libconfig_test "libconfig_test.cpp" )
target_link_libraries( libconfig_test config++ )