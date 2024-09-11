set(SOURCE_FILES 
        BaseSocket.cpp
        EndPoint.cpp
        TCPSocket.cpp
        UDPBoundSocket.cpp
        UDPSocket.cpp
        Platform.cpp)

add_library(EIPScannerSocket SHARED ${SOURCE_FILES})
add_library(EIPScannerSocketS STATIC ${SOURCE_FILES})

target_link_libraries(EIPScannerSocket EIPScanner)
target_link_libraries(EIPScannerSocketS EIPScannerS)