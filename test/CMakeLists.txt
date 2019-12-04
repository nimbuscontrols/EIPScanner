include_directories("${CMAKE_SOURCE_DIR}/src")

add_executable(cli_test CLITest.cpp)
target_link_libraries(cli_test EIPScanner)

add_executable(file_object_example FileObjectExample.cpp)
target_link_libraries(file_object_example EIPScanner)

enable_testing()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(GTest REQUIRED)
find_package(GMock REQUIRED)

add_executable(test_eipscanner
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

        test.cpp
        )
message(STATUS ${GTEST_BOTH_LIBRARIES})
message(STATUS ${GMOCK_BOTH_LIBRARIES})
target_link_libraries(test_eipscanner
        ${GTEST_BOTH_LIBRARIES}
        ${GMOCK_BOTH_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        EIPScanner)

add_test(TestEipScanner test_eipscanner)