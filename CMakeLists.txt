cmake_minimum_required(VERSION 3.5)

set(EIPSCANNER_MAJOR_VERSION 1)
set(EIPSCANNER_MINOR_VERSION 1)
set(EIPSCANNER_PATCH_VERSION 0)
set(EIPSCANNER_FULL_VERSION ${EIPSCANNER_MAJOR_VERSION}.${EIPSCANNER_MINOR_VERSION}.${EIPSCANNER_PATCH_VERSION})

project(eip_scanner VERSION ${EIPSCANNER_FULL_VERSION})

find_package(catkin REQUIRED)

set(EIP_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(EIP_INCLUDE_DIR ${CATKIN_DEVEL_PREFIX}/include)
file(MAKE_DIRECTORY ${EIP_INCLUDE_DIR})

catkin_package(
    INCLUDE_DIRS ${EIP_INCLUDE_DIR} ${EIP_INCLUDE_DIR}/eip_scanner
    LIBRARIES ${PROJECT_NAME}
)

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

file(GLOB_RECURSE EIP_HEADER_FILES RELATIVE ${EIP_SOURCE_DIR} ${EIP_SOURCE_DIR}/*.h**)
list(FILTER EIP_HEADER_FILES EXCLUDE REGEX ^vendor) # remove vendor files

add_custom_target(copyIncludeFiles)
foreach(file ${EIP_HEADER_FILES})
    set(sourceFile ${EIP_SOURCE_DIR}/${file})
    set(destinationFile ${EIP_INCLUDE_DIR}/${PROJECT_NAME}/${file})
    add_custom_command(TARGET copyIncludeFiles PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${sourceFile} ${destinationFile}
        BYPRODUCTS ${destinationFile} VERBATIM)
endforeach()

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${EIP_SOURCE_DIR})
add_dependencies(${PROJECT_NAME} copyIncludeFiles)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
    VERSION ${EIPSCANNER_FULL_VERSION}
    SOVERSION ${EIPSCANNER_MAJOR_VERSION}
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS OFF)

add_library(${PROJECT_NAME}S STATIC ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME}S PRIVATE ${EIP_SOURCE_DIR})
add_dependencies(${PROJECT_NAME}S copyIncludeFiles)

set_target_properties(
    ${PROJECT_NAME}S
    PROPERTIES
    VERSION ${EIPSCANNER_FULL_VERSION}
    SOVERSION ${EIPSCANNER_MAJOR_VERSION}
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS OFF)

install(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}S
    ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
    LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
    RUNTIME DESTINATION ${CATKIN_GLOBAL_BIN_DESTINATION})

install(DIRECTORY ${EIP_INCLUDE_DIR}/${PROJECT_NAME}/
    DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION})
