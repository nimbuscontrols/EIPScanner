enable_testing()
include_directories("${CMAKE_SOURCE_DIR}/src")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(GTest REQUIRED)
find_package(GMock REQUIRED)

add_executable(test_eipscanner
        cip/TestCipRevision.cpp
        cip/TestCipString.cpp
        cip/TestEPath.cpp
        cip/TestMessageRouterResponse.cpp

        eip/TestCommonPacket.cpp
        eip/TestCommonPacketItem.cpp
        eip/TestCommonPacketItemFactory.cpp
        eip/TestEncapsPacket.cpp
        eip/TestEncapsPacketFactory.cpp

        fileObject/TestFileObjectLoadedState.cpp
        fileObject/TestFileObjectUploadInProgressState.cpp

        utils/TestBuffer.cpp

        TestIdentityObject.cpp
        TestParameterObject.cpp
        test.cpp
        )
message(STATUS ${GTEST_BOTH_LIBRARIES})
message(STATUS ${GMOCK_BOTH_LIBRARIES})

include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${GMOCK_INCLUDE_DIRS})

target_link_libraries(test_eipscanner
        ${GTEST_BOTH_LIBRARIES}
        ${GMOCK_BOTH_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        EIPScanner)

add_test(TestEipScanner test_eipscanner)