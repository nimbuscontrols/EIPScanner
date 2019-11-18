include_directories(${CMAKE_SOURCE_DIR}/src)
add_library(EIPScanner SHARED

        cip/connectionManager/ForwardOpenRequest.cpp
        cip/connectionManager/ForwardOpenResponse.cpp
        cip/EPath.cpp
        cip/MessageRouterRequest.cpp
        cip/MessageRouterResponse.cpp

        eip/CommonPacket.cpp
        eip/CommonPacketItem.cpp
        eip/CommonPacketItemFactory.cpp
        eip/EncapsPacket.cpp
        eip/EncapsPacketFactory.cpp

        sockets/TCPSocket.cpp

        utils/Logger.cpp
        utils/Buffer.cpp

        ConnectionManager.cpp
        IOConnection.cpp
        MessageRouter.cpp
        SessionInfo.cpp)