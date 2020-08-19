#include "cip/Types.h"
#include <functional>
#include <sstream>
#include <fstream>
#include <cip/connectionManager/NetworkConnectionParams.h>
#include "ConnectionManager.h"
#include <DiscoveryManager.h>
#include "FileObject.h"
#include "fileObject/FileObjectState.h"
#include "IdentityObject.h"
#include "IOConnection.h"
#include "ParameterObject.h"
#include "SessionInfo.h"
#include "utils/Logger.h"
#include "utils/Buffer.h"

#include "vendor/yaskawa/mp3300iec/Yaskawa_EPath.h"
#include "vendor/yaskawa/mp3300iec/Yaskawa_MessageRouter.h"
#include "vendor/yaskawa/mp3300iec/Yaskawa_MessageRouterRequest.h"

using namespace eipScanner::cip;
using eipScanner::ConnectionManager;
using eipScanner::DiscoveryManager;
using eipScanner::IdentityObject;
using eipScanner::IOConnection;
using eipScanner::Yaskawa_MessageRouter;
using eipScanner::ParameterObject;
using eipScanner::SessionInfo;
using eipScanner::utils::Buffer;
using eipScanner::utils::Logger;
using eipScanner::utils::LogLevel;
using eipScanner::cip::connectionManager::ConnectionParameters;
using eipScanner::cip::connectionManager::NetworkConnectionParams;

#define YASKAWA_IP_ADDR "192.168.1.2"
#define YASKAWA_PORT 0xAF12
#define YASKAWA_VENDOR_ID 0x2c

#define ASSEMBLY_OBJECT 0x04

#define YASKAWA_INPUT_ASSEMBLY_111 0x6F
#define YASKAWA_OUTPUT_ASSEMBLY_101 0x65

#define ASSEMBLY_INSTANCE_SIZE 128

#define DATA_OFFSET 100

/*
 * fillVector - Function to fill up a vector of bytes.
 *      data_offset:                Offset to the first byte you want to start writing
 *      assembly_instance_size:     Size of the instance in bytes
 *
 *      Returns a vector of bytes
 */
std::vector<uint8_t> fillVector(int data_offset, int assembly_instance_size)
{
    std::vector<uint8_t> data;

    for (int i = 0; i < assembly_instance_size; i++)
    {
        if (i < data_offset)
            data.push_back(0x00);
        else
            data.push_back(0xDE);
    }

    return data;
}

/*
 * readBuffer - Function to read a buffer into bytes
 *      buffer:                     Buffer filled with the response
 *      assembly_instance_size:     Size of the instance in bytes
 *
 *      Returns a vector of bytes
 */
std::vector<uint8_t> readBuffer(Buffer buffer, int assembly_instance_size)
{
    uint8_t byte;
    std::vector<uint8_t> data;

    for (int i = 0; i < assembly_instance_size; i++)
    {
        buffer >> byte;
        data.push_back(byte);
    }

    return data;
}

/*
 * writeAssemblyObject - Writes the Assembly Instance Provided (Please note that the entire instance will be written.)
 *  si:                 SessionInfo object holding the connection Information
 *  assembly_instance:  Integer declaring which instance to be written
 *  data:               Vector of Bytes to be written to the Assembly Instance
 *
 *  Returns true if successful.
 */
bool writeAssemblyObject(std::shared_ptr<eipScanner::SessionInfo> si, int assembly_instance, std::vector<uint8_t> data)
{
    Yaskawa_MessageRouter messageRouter;

    auto response = messageRouter.sendRequest(si
            , ServiceCodes::SET_ATTRIBUTE_SINGLE
            , Yaskawa_EPath(ASSEMBLY_OBJECT, assembly_instance, 0x03)
            , data);

    if (response.getGeneralStatusCode() != GeneralStatusCodes::SUCCESS) {
        Logger(LogLevel::ERROR) << "Failed to write assembly object";
        logGeneralAndAdditionalStatus(response);
        return 0;
    }

    Logger(LogLevel::INFO) << "The device has written successfully.";
    return 1;
}

/*
 * readAssemblyObject - Reads the Assembly Instance Provided (Please note that the entire instance will be read.)
 *      si:                         SessionInfo object holding the connection Information
 *      assembly_instance:          Integer declaring which instance to be read
 *
 *      Returns a vector of bytes that contains the response or nothing if there was an error
 */
std::vector<uint8_t> readAssemblyObject(std::shared_ptr<eipScanner::SessionInfo> si, int assembly_instance)
{
    Yaskawa_MessageRouter messageRouter;
    std::vector<uint8_t> data_read;
    uint8_t firstByte;
    int num_bytes;

    auto response = messageRouter.sendRequest(si
            , ServiceCodes::GET_ATTRIBUTE_SINGLE
            , Yaskawa_EPath(ASSEMBLY_OBJECT, assembly_instance, 0x03));

    if (response.getGeneralStatusCode() != GeneralStatusCodes::SUCCESS) {
        Logger(LogLevel::ERROR) << "Failed to read parameters";
        logGeneralAndAdditionalStatus(response);
        return data_read;
    }

    Buffer buffer(response.getData());

    num_bytes = 4 * sizeof (buffer);
    data_read = readBuffer(buffer, num_bytes);
    firstByte = data_read[0];

    // converting the byte to uint16_t to properly print with the Logger
    Logger(LogLevel::INFO) << "The device has " << num_bytes << " bytes. The first byte contains: " << (uint16_t)firstByte;

    return data_read;
}

// Test to read and write assembly objects on the adapter
int main() {

    Logger::setLogLevel(LogLevel::DEBUG);
    std::vector<uint8_t> data_read;
    bool success;

    // Connect to the adapter with the given ip address and port number
    auto si = std::make_shared<SessionInfo>(YASKAWA_IP_ADDR, YASKAWA_PORT);

    // Data to be written -- In this example we just fill it with junk
    std::vector<uint8_t> data_write = fillVector(0x00, ASSEMBLY_INSTANCE_SIZE);

    // Assembly Object - Perform a read on the output assembly, and then a read and a write on the input assembly
    data_read = readAssemblyObject(si, YASKAWA_OUTPUT_ASSEMBLY_101);
    data_read = readAssemblyObject(si, YASKAWA_INPUT_ASSEMBLY_111);
    success = writeAssemblyObject(si, YASKAWA_INPUT_ASSEMBLY_111, data_write);

    return 0;
}

#if 0
// Test to confirm you are connected to a Yaskawa Device
int main() {

    Logger::setLogLevel(LogLevel::DEBUG);

    DiscoveryManager discoveryManager(YASKAWA_IP_ADDR, YASKAWA_PORT, std::chrono::seconds(1));
    auto devices = discoveryManager.discover();

    for (auto& device : devices) {
        if (device.identityObject.getVendorId() == YASKAWA_VENDOR_ID)
        {
            Logger(LogLevel::INFO) << "Discovered YASKAWA device: "
                << device.identityObject.getProductName()
                << " with address " << device.socketAddress.toString();
        }
    }


    return 0;
}
#endif

