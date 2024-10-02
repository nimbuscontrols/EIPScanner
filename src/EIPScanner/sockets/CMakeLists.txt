set(SOURCE_FILES 
        ${CMAKE_CURRENT_LIST_DIR}/BaseSocket.cpp
        ${CMAKE_CURRENT_LIST_DIR}/EndPoint.cpp
        ${CMAKE_CURRENT_LIST_DIR}/TCPSocket.cpp
        ${CMAKE_CURRENT_LIST_DIR}/UDPBoundSocket.cpp
        ${CMAKE_CURRENT_LIST_DIR}/UDPSocket.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Platform.cpp)

        
target_sources(EIPScanner PUBLIC ${SOURCE_FILES})
target_sources(EIPScanner PUBLIC ${SOURCE_FILES})