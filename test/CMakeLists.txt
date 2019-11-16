include_directories("${CMAKE_SOURCE_DIR}/src")

add_executable(cli_test CLITest.cpp)
target_link_libraries(cli_test EIPScanner)

enable_testing()
find_package(GTest REQUIRED)

add_executable(test_eipscanner
        eip/TestEncapsPacket.cpp
        eip/TestEncapsPacketFactory.cpp
        utils/TestBuffer.cpp

        test.cpp
        )

target_link_libraries(test_eipscanner
        ${GTEST_BOTH_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        EIPScanner)

add_test(TestEipScanner test_eipscanner)