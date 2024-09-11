set(SOURCE_FILES 
    ForwardCloseRequest.cpp
    ForwardOpenRequest.cpp
    LargeForwardOpenRequest.cpp
    ForwardOpenResponse.cpp
    NetworkConnectionParametersBuilder.cpp)

add_library(EIPScannerCipConnectionManager SHARED ${SOURCE_FILES})
add_library(EIPScannerCipConnectionManagerS STATIC ${SOURCE_FILES})

target_link_libraries(EIPScannerCipConnectionManager EIPScanner)
target_link_libraries(EIPScannerCipConnectionManagerS EIPScannerS)