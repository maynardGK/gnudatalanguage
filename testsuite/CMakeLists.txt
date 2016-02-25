set(BASE_SOURCE ${CMAKE_SOURCE_DIR})
set(BASE_BINARY ${CMAKE_BINARY_DIR})
add_executable(launchtest EXCLUDE_FROM_ALL ${CMAKE_SOURCE_DIR}/testsuite/launchtest.c)
add_library(test_ce SHARED EXCLUDE_FROM_ALL libtest_ce.cpp)
add_custom_target(check COMMAND ${CMAKE_BUILD_TOOL} test DEPENDS launchtest test_ce gdl)
get_target_property(LAUNCHTESTLOCATION launchtest LOCATION)
if(WIN32 AND NOT CYGWIN)
  file(READ "${BASE_SOURCE}/testsuite/Makefile.am" MAKEFILEAM)
  string(REGEX MATCHALL "test[_a-zA-Z0-9]+\\.pro" TESTS ${MAKEFILEAM})
else(WIN32 AND NOT CYGWIN)
  execute_process(
    COMMAND "fgrep" ".pro" "${BASE_SOURCE}/testsuite/Makefile.am" 
    COMMAND "awk" "{printf(\"%s;\",$1)}" 
    OUTPUT_VARIABLE TESTS)
endif(WIN32 AND NOT CYGWIN)
foreach(TEST ${TESTS})
	add_test(${TEST} ${LAUNCHTESTLOCATION} ${TEST})
	set_tests_properties(${TEST} PROPERTIES SKIP_RETURN_CODE 77) # autoconf's setting
endforeach(TEST TESTS)
