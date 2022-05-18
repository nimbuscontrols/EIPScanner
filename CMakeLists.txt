cmake_minimum_required(VERSION 3.5)

set(EIPSCANNER_MAJOR_VERSION 1)
set(EIPSCANNER_MINOR_VERSION 1)
set(EIPSCANNER_PATCH_VERSION 0)
set(EIPSCANNER_FULL_VERSION ${EIPSCANNER_MAJOR_VERSION}.${EIPSCANNER_MINOR_VERSION}.${EIPSCANNER_PATCH_VERSION})

project(EIPScanner VERSION ${EIPSCANNER_FULL_VERSION})

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

set(INSTALL_CMAKE_DIR share/EIPScanner/cmake CACHE PATH
    "Installation directory for CMake files")
# ... for the build tree
set(INSTALL_INCLUDE_DIR include/EIPScanner CACHE PATH
      "Installation directory for header files")
# ... for the build tree
set(INSTALL_LIB_DIR lib CACHE PATH
      "Installation directory for header files")
# Make relative paths absolute (needed later on)
# foreach(p LIB BIN INCLUDE CMAKE)
foreach(p LIB INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()

file(RELATIVE_PATH REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}"
       "${INSTALL_INCLUDE_DIR}")
set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}")
configure_file(EIPScannerConfig.cmake.in
  "${PROJECT_BINARY_DIR}/EIPScannerConfig.cmake" @ONLY)
# ... for the install tree
set(CONF_INCLUDE_DIRS "\${EIPScanner_CMAKE_DIR}/${REL_INCLUDE_DIR}")
configure_file(EIPScannerConfig.cmake.in
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/EIPScannerConfig.cmake" @ONLY)
# ... for both
configure_file(EIPScannerConfigVersion.cmake.in
  "${PROJECT_BINARY_DIR}/EIPScannerConfigVersion.cmake" @ONLY)
# Install the EIPScannerConfig.cmake and EIPScannerConfigVersion.cmake
install(FILES
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/EIPScannerConfig.cmake"
  "${PROJECT_BINARY_DIR}/EIPScannerConfigVersion.cmake"
  DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)

add_subdirectory(src)
if (EXAMPLE_ENABLED)
    add_subdirectory(examples)
endif()

if (TEST_ENABLED)
    add_subdirectory(test)
endif()
