include_directories(${CMAKE_SOURCE_DIR}/src)
add_library(EIPScanner SHARED
        eip/EncapsPacket.cpp
        eip/EncapsPacketFactory.cpp

        utils/Logger.cpp
        utils/Buffer.cpp

        SessionInfo.cpp)