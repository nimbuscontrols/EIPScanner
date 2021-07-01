cmake_minimum_required(VERSION 3.5)

set(EIPSCANNER_MAJOR_VERSION 1)
set(EIPSCANNER_MINOR_VERSION 1)
set(EIPSCANNER_PATCH_VERSION 0)
set(EIPSCANNER_FULL_VERSION ${EIPSCANNER_MAJOR_VERSION}.${EIPSCANNER_MINOR_VERSION}.${EIPSCANNER_PATCH_VERSION})

project(EIPScanner VERSION ${EIPSCANNER_FULL_VERSION})


set(CMAKE_CXX_STANDARD 14)
option(ENABLE_VENDOR_SRC "Enable vendor source" OFF)

find_package(catkin REQUIRED COMPONENTS
)


include_directories(${CMAKE_SOURCE_DIR}/src)

set(SOURCE_FILES
        src/cip/connectionManager/ForwardCloseRequest.cpp
        src/cip/connectionManager/ForwardOpenRequest.cpp
        src/cip/connectionManager/LargeForwardOpenRequest.cpp
        src/cip/connectionManager/ForwardOpenResponse.cpp
        src/cip/connectionManager/NetworkConnectionParametersBuilder.cpp

        src/cip/CipRevision.cpp
        src/cip/EPath.cpp
        src/cip/MessageRouterRequest.cpp
        src/cip/MessageRouterResponse.cpp
        src/cip/CipString.cpp

        src/eip/CommonPacket.cpp
        src/eip/CommonPacketItem.cpp
        src/eip/CommonPacketItemFactory.cpp
        src/eip/EncapsPacket.cpp
        src/eip/EncapsPacketFactory.cpp

        src/fileObject/FileObjectEmptyState.cpp
        src/fileObject/FileObjectLoadedState.cpp
        src/fileObject/FileObjectNonExistentState.cpp
        src/fileObject/FileObjectState.cpp
        src/fileObject/FileObjectUploadInProgressState.cpp

        src/sockets/BaseSocket.cpp
        src/sockets/EndPoint.cpp
        src/sockets/TCPSocket.cpp
        src/sockets/UDPBoundSocket.cpp
        src/sockets/UDPSocket.cpp

        src/utils/Logger.cpp
        src/utils/Buffer.cpp

        src/BaseObject.cpp
        src/ConnectionManager.cpp
        src/DiscoveryManager.cpp
        src/FileObject.cpp
        src/IdentityObject.cpp
        src/IOConnection.cpp
        src/MessageRouter.cpp
        src/ParameterObject.cpp
        src/SessionInfo.cpp)


catkin_package(
        INCLUDE_DIRS src
        LIBRARIES EIPScanner

  #      DEPENDS EIGEN3
)

add_library(EIPScanner SHARED ${SOURCE_FILES})
add_library(EIPScannerS STATIC ${SOURCE_FILES})

set_target_properties(
        EIPScanner
        PROPERTIES
        VERSION ${EIPSCANNER_FULL_VERSION}
        SOVERSION ${EIPSCANNER_MAJOR_VERSION})

#install(TARGETS EIPScanner EIPScannerS
#        LIBRARY
#            DESTINATION lib
#        ARCHIVE
#            DESTINATION lib)
#
#install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/
#        DESTINATION include/EIPScanner
#        FILES_MATCHING PATTERN "*.h*")


