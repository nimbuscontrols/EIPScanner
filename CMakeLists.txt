cmake_minimum_required(VERSION 3.5)

set(EIPSCANNER_MAJOR_VERSION 1)
set(EIPSCANNER_MINOR_VERSION 1)
set(EIPSCANNER_PATCH_VERSION 0)
set(EIPSCANNER_FULL_VERSION ${EIPSCANNER_MAJOR_VERSION}.${EIPSCANNER_MINOR_VERSION}.${EIPSCANNER_PATCH_VERSION})

project(EIPScanner
    VERSION ${EIPSCANNER_FULL_VERSION}
    DESCRIPTION "Free implementation of EtherNet/IP in C++"
    HOMEPAGE_URL "https://github.com/nimbuscontrols/EIPScanner"
    )

set(CMAKE_CXX_STANDARD 20)
option(ENABLE_VENDOR_SRC "Enable vendor source" ON)
option(TEST_ENABLED "Enable unit test" OFF)
option(EXAMPLE_ENABLED "Build examples" OFF)

if (WIN32)
	# Needed on MinGW with GCC 10 or lower
	add_compile_definitions(_WIN32_WINNT=0x0600)

	# Prevent std::numeric_limits::max collision
	if (MSVC)
		add_compile_definitions(NOMINMAX)
	endif()
endif()

# TODO: Change the file structure so that every folder isn't included in the include directories
include_directories(${PROJECT_SOURCE_DIR})
add_subdirectory(src)

if (EXAMPLE_ENABLED)
    add_subdirectory(examples)
endif()

if (TEST_ENABLED)
    add_subdirectory(test)
endif()


configure_file(EIPScanner.pc.in EIPScanner.pc @ONLY)

install(FILES ${CMAKE_BINARY_DIR}/EIPScanner.pc DESTINATION lib/pkgconfig)
