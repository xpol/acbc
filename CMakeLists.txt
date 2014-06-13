cmake_minimum_required (VERSION 2.6)
project (acbc)
enable_testing()

if (MSVC)
	add_definitions(/D "_CRT_SECURE_NO_WARNINGS")
else (MSVC)
	set(CMAKE_CXX_FLAGS "-std=c++0x")
endif (MSVC)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/lib")

find_package(FreeImage REQUIRED)
include_directories(${FREEIMAGE_INCLUDE_PATH})
set(LIBS ${LIBS} ${FREEIMAGE_LIBRARY})

add_executable(acbc ${CMAKE_SOURCE_DIR}/src/acbc.cpp)
target_link_libraries(acbc ${LIBS})

if (MSVC)
	add_custom_command(TARGET acbc POST_BUILD        # Adds a post-build event to MyTest
    	COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
    	    "${PROJECT_SOURCE_DIR}/lib/win32/FreeImage.dll"      # <--this is in-file
        	$<TARGET_FILE_DIR:acbc>)
endif (MSVC)


# Tests
add_test(BIT0 acbc ${CMAKE_SOURCE_DIR}/test/0.png)
set_tests_properties(BIT0 PROPERTIES PASS_REGULAR_EXPRESSION "0")

add_test(BIT1 acbc ${CMAKE_SOURCE_DIR}/test/1.png)
add_test(BIT01 acbc ${CMAKE_SOURCE_DIR}/test/0.png ${CMAKE_SOURCE_DIR}/test/1.png)
set_tests_properties(BIT1 BIT01 PROPERTIES PASS_REGULAR_EXPRESSION "1")

add_test(BIT8 acbc ${CMAKE_SOURCE_DIR}/test/8.png)
add_test(BIT018 acbc ${CMAKE_SOURCE_DIR}/test/0.png ${CMAKE_SOURCE_DIR}/test/1.png ${CMAKE_SOURCE_DIR}/test/8.png)
add_test(BIT18 acbc ${CMAKE_SOURCE_DIR}/test/1.png ${CMAKE_SOURCE_DIR}/test/8.png)
set_tests_properties(BIT8 BIT018 BIT18 PROPERTIES PASS_REGULAR_EXPRESSION "8")
