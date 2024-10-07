set(SOURCE_FILES 
        ${CMAKE_CURRENT_LIST_DIR}/powerFlex525/DPIFaultManager.cpp
        ${CMAKE_CURRENT_LIST_DIR}/powerFlex525/DPIFaultObject.cpp
        ${CMAKE_CURRENT_LIST_DIR}/powerFlex525/DPIFaultCode.cpp
        ${CMAKE_CURRENT_LIST_DIR}/powerFlex525/DPIFaultParameter.cpp)
        
target_sources(EIPScanner PUBLIC ${SOURCE_FILES})
target_sources(EIPScanner PUBLIC ${SOURCE_FILES})