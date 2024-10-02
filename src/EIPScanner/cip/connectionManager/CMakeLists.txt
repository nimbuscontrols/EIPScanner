set(SOURCE_FILES 
    ${CMAKE_CURRENT_LIST_DIR}/ForwardCloseRequest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ForwardOpenRequest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/LargeForwardOpenRequest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ForwardOpenResponse.cpp
    ${CMAKE_CURRENT_LIST_DIR}/NetworkConnectionParametersBuilder.cpp)

target_sources(EIPScanner PUBLIC ${SOURCE_FILES})
target_sources(EIPScannerS PUBLIC ${SOURCE_FILES})