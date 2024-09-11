set(SOURCE_FILES 
        CipRevision.cpp
        EPath.cpp
        MessageRouterRequest.cpp
        MessageRouterResponse.cpp
        CipString.cpp)

add_subdirectory(connectionManager)

add_library(EIPScannerCip SHARED ${SOURCE_FILES})
add_library(EIPScannerCipS STATIC ${SOURCE_FILES})

target_link_libraries(EIPScannerCip EIPScanner)
target_link_libraries(EIPScannerCipS EIPScannerS)