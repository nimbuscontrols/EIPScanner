set(SOURCE_FILES 
        ${CMAKE_CURRENT_LIST_DIR}/FileObjectEmptyState.cpp
        ${CMAKE_CURRENT_LIST_DIR}/FileObjectLoadedState.cpp
        ${CMAKE_CURRENT_LIST_DIR}/FileObjectNonExistentState.cpp
        ${CMAKE_CURRENT_LIST_DIR}/FileObjectState.cpp
        ${CMAKE_CURRENT_LIST_DIR}/FileObjectUploadInProgressState.cpp)
        
target_sources(EIPScanner PUBLIC ${SOURCE_FILES})
target_sources(EIPScannerS PUBLIC ${SOURCE_FILES})