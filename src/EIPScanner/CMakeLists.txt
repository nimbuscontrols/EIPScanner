# Add library target for common source files
set(SOURCE_FILES 
        BaseObject.cpp
        ConnectionManager.cpp
        DiscoveryManager.cpp
        FileObject.cpp
        IdentityObject.cpp
        IOConnection.cpp
        MessageRouter.cpp
        ParameterObject.cpp
        SessionInfo.cpp)
        
add_library(EIPScanner SHARED ${SOURCE_FILES})
add_library(EIPScannerS STATIC ${SOURCE_FILES})


add_subdirectory(cip)
add_subdirectory(eip)
add_subdirectory(fileObject)
add_subdirectory(sockets)
add_subdirectory(utils)


# if vendor scripts are enabled
if(ENABLE_VENDOR_SRC)
        add_subdirectory(vendor/ra)
        add_subdirectory(vendor/yaskawa)
        add_subdirectory(vendor/teknic)
endif()


if(WIN32)
	target_link_libraries(EIPScanner ws2_32)
	target_link_libraries(EIPScannerS ws2_32)
endif()


set_target_properties(
        EIPScanner
        PROPERTIES
        VERSION ${EIPSCANNER_FULL_VERSION}
        SOVERSION ${EIPSCANNER_MAJOR_VERSION})

install(TARGETS EIPScanner EIPScannerS
        LIBRARY
            DESTINATION lib
        ARCHIVE
            DESTINATION lib)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/src/
        DESTINATION include
        FILES_MATCHING PATTERN "*.h*")