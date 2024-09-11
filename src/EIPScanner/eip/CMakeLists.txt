set(SOURCE_FILES 
        CommonPacket.cpp
        CommonPacketItem.cpp
        CommonPacketItemFactory.cpp
        EncapsPacket.cpp
        EncapsPacketFactory.cpp)

add_library(EIPScannerEip SHARED ${SOURCE_FILES})
add_library(EIPScannerEipS STATIC ${SOURCE_FILES})

target_link_libraries(EIPScannerEip EIPScanner)
target_link_libraries(EIPScannerEipS EIPScannerS)