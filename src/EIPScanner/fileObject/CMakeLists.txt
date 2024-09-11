set(SOURCE_FILES 
        FileObjectEmptyState.cpp
        FileObjectLoadedState.cpp
        FileObjectNonExistentState.cpp
        FileObjectState.cpp
        FileObjectUploadInProgressState.cpp)

add_library(EIPScannerFileObject SHARED ${SOURCE_FILES})
add_library(EIPScannerFileObjectS STATIC ${SOURCE_FILES})

target_link_libraries(EIPScannerFileObject EIPScanner)
target_link_libraries(EIPScannerFileObjectS EIPScannerS)