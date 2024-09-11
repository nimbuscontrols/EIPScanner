set(SOURCE_FILES 
        Logger.cpp
        Buffer.cpp)

add_library(EIPScannerUtil SHARED ${SOURCE_FILES})
add_library(EIPScannerUtilS STATIC ${SOURCE_FILES})

target_link_libraries(EIPScannerUtil EIPScanner)
target_link_libraries(EIPScannerUtilS EIPScannerS)