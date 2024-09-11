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
        add_subdirectory(vendor)
endif()
