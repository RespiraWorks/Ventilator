find_package(Threads REQUIRED)
find_package(GTest)

if (NOT GTest_FOUND)
	if (CMAKE_SYSTEM_NAME MATCHES Linux AND NOT WITH_CONAN)

		find_program(LSB_RELEASE lsb_release)
		execute_process(COMMAND ${LSB_RELEASE} -is
			OUTPUT_VARIABLE LINUX_DISTRIBUTION
			OUTPUT_STRIP_TRAILING_WHITESPACE)

		if (LINUX_DISTRIBUTION MATCHES "Debian" OR LINUX_DISTRIBUTION MATCHES "Ubuntu")
			message(STATUS "Attempting to apply Debian fix for googletest")

			include(ExternalProject)

			execute_process(COMMAND ${LSB_RELEASE} -cs
				OUTPUT_VARIABLE LSB_CODENAME
				OUTPUT_STRIP_TRAILING_WHITESPACE)

			if(LSB_CODENAME MATCHES stretch)
				set(GTEST_SOURCE_DIR /usr/src/googletest/googletest)
			elseif(LSB_CODENAME MATCHES jessie OR LSB_CODENAME MATCHES xenial)
				set(GTEST_SOURCE_DIR /usr/src/gtest)
			endif()

			if(EXISTS ${GTEST_SOURCE_DIR} AND IS_DIRECTORY ${GTEST_SOURCE_DIR})
				#if the source directory exists and is indeed a directory we build the
				#gtest code
				set(GTEST_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/googletest-build)
				add_subdirectory(${GTEST_SOURCE_DIR} ${GTEST_BINARY_DIR})
				set(GTEST_INCLUDE_DIRS ${GTEST_SOURCE_DIR}/include)
				set(GTEST_LIBRARIES gtest gtest_main)
				set(GTest_FOUND 1)
			endif()

		else ()
			message(SEND_ERROR "GTest not found and no fix available")
		endif ()

	else ()
		message(SEND_ERROR "GTest not found and no fix available")
	endif ()
endif ()
