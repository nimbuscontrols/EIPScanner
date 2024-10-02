set(SOURCE_FILES 
        ${CMAKE_CURRENT_LIST_DIR}/mp3300iec/Yaskawa_EPath.cpp
        ${CMAKE_CURRENT_LIST_DIR}/mp3300iec/Yaskawa_MessageRouter.cpp
        ${CMAKE_CURRENT_LIST_DIR}/mp3300iec/Yaskawa_MessageRouterRequest.cpp)

target_sources(EIPScanner PUBLIC ${SOURCE_FILES})
target_sources(EIPScanner PUBLIC ${SOURCE_FILES})