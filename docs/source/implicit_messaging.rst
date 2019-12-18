Implicit Messaging
==================

**EtherNet/IP** protocol allows to connect the scanner and the adapter by using
IO connections, so that they can send data each other periodically or then the
data have changed.

In order to establish and handle IO connections through UDP, **EIPScanner** provides 
*ConnectionManager* class, that has *forwardOpen* method where we can pass all the parameters
of the connections. The method returns an instance of *IOConnection* class, which we can use 
to handle the received data from the adapter and send the our data to it. 


.. code-block:: cpp

   #include <sstream>
   #include <cip/connectionManager/NetworkConnectionParams.h>
   #include "SessionInfo.h"
   #include "ConnectionManager.h"
   #include "utils/Logger.h"
   #include "utils/Buffer.h"

   using namespace eipScanner::cip;
   using eipScanner::SessionInfo;
   using eipScanner::MessageRouter;
   using eipScanner::ConnectionManager;
   using eipScanner::cip::connectionManager::ConnectionParameters;
   using eipScanner::cip::connectionManager::NetworkConnectionParams;
   using eipScanner::utils::Buffer;
   using eipScanner::utils::Logger;
   using eipScanner::utils::LogLevel;

   int main() {
           Logger::setLogLevel(LogLevel::DEBUG);
           auto si = std::make_shared<SessionInfo>("172.28.1.3", 0xAF12);

           // Implicit messaging
           ConnectionManager connectionManager;

           ConnectionParameters parameters;
           parameters.connectionPath = {0x20, 0x04,0x24, 151, 0x2C, 150, 0x2C, 100};  // config Assm151, output Assm150, intput Assm100
           parameters.o2tRealTimeFormat = true;
           parameters.originatorVendorId = 342;
           parameters.originatorSerialNumber = 32423;
           parameters.t2oNetworkConnectionParams |= NetworkConnectionParams::P2P;
           parameters.t2oNetworkConnectionParams |= NetworkConnectionParams::SCHEDULED_PRIORITY;
           parameters.t2oNetworkConnectionParams |= 32; //size of Assm100 =32
           parameters.o2tNetworkConnectionParams |= NetworkConnectionParams::P2P;
           parameters.o2tNetworkConnectionParams |= NetworkConnectionParams::SCHEDULED_PRIORITY;
           parameters.o2tNetworkConnectionParams |= 32; //size of Assm150 = 32

           parameters.originatorSerialNumber = 0x12345;
           parameters.o2tRPI = 1000000;
           parameters.t2oRPI = 1000000;
           parameters.transportTypeTrigger |= NetworkConnectionParams::CLASS1;

           auto io = connectionManager.forwardOpen(si, parameters);
           if (auto ptr = io.lock()) {
                   ptr->setDataToSend(std::vector<uint8_t>(32));

                   ptr->setReceiveDataListener([](auto realTimeHeader, auto sequence, auto data) {
                           std::ostringstream ss;
                           ss << "secNum=" << sequence << " data=";
                           for (auto &byte : data) {
                                   ss << "[" << std::hex << (int) byte << "]";
                           }

                           Logger(LogLevel::INFO) << "Received: " << ss.str();
                   });

                   ptr->setCloseListener([]() {
                           Logger(LogLevel::INFO) << "Closed";
                   });
           }

           int count = 200;
           while (connectionManager.hasOpenConnections() && count-- > 0) {
                   connectionManager.handleConnections(std::chrono::milliseconds(100));
           }

           connectionManager.forwardClose(si, io);

           return 0;
   }

There are many lines of code here. But let's go through it step by step.

.. code-block:: cpp

  auto si = std::make_shared<SessionInfo>("172.28.1.3", 0xAF12);


*ConnectionManager* uses :ref:`explicit_messaging`, so we need to establish **EIP** session before to
call service *Forward_Open*:


.. code-block:: cpp

  ConnectionManager connectionManager;

  ConnectionParameters parameters;
  parameters.connectionPath = {0x20, 0x04,0x24, 151, 0x2C, 150, 0x2C, 100};  // config Assm151, output Assm150, intput Assm100
  parameters.o2tRealTimeFormat = true;
  parameters.originatorVendorId = 342;
  parameters.originatorSerialNumber = 32423;
  parameters.t2oNetworkConnectionParams |= NetworkConnectionParams::P2P;
  parameters.t2oNetworkConnectionParams |= NetworkConnectionParams::SCHEDULED_PRIORITY;
  parameters.t2oNetworkConnectionParams |= 32; //size of Assm100 =32
  parameters.o2tNetworkConnectionParams |= NetworkConnectionParams::P2P;
  parameters.o2tNetworkConnectionParams |= NetworkConnectionParams::SCHEDULED_PRIORITY;
  parameters.o2tNetworkConnectionParams |= 32; //size of Assm150 = 32

  parameters.originatorSerialNumber = 0x12345;
  parameters.o2tRPI = 1000000;
  parameters.t2oRPI = 1000000;
  parameters.transportTypeTrigger |= NetworkConnectionParams::CLASS1;

  auto io = connectionManager.forwardOpen(si, parameters);

As you can see, IO connection has a lot of parameters. This tutorial doesn't aim to give the whole information about all the
options and parameters of the implicit messaging. Use please **CIP** specification for details. Moreover each **EIP** 
device can have its own set of parameters which it uses to establish the IO connection. 
Always see documentation or\\and EDS files to figure out how to tune the parameters.

However, there are some things that need clarifying:

   1. Service *Forward_Open* opens two connections: Originator (Scanner) => Target (Adapter) 
      and Target => Originator. Parameters that start with **o2t** defined for direction Originator => Target, **t2o** for 
      Originator => Target. 
   2. *t2oNetworkConnectionParams* and *t2oNetworkConnectionParams* has last 9 bits for connection size. Use operator
      **|=** to set them
   3. IO connection path must be a vector of byte (as you see it in EDS file or specification): 
      0x20 0x04 CONFIG_ASSEMBLY_ID 0x2C OUTPUT_ASSEMBLY_ID 0x2C INPUT_ASSEMBLY 
   4. *RPI* and *API* in microseconds

If method *ConnectionManager::forwardOpen* has managed to open the connection it returns a weak pointer to it else null pointer:

.. code-block:: cpp

  if (auto ptr = io.lock()) {
          // Set data to send 
          ptr->setDataToSend(std::vector<uint8_t>(32));

          // Set callback for received data 
          ptr->setReceiveDataListener([](auto realTimeHeader, auto sequence, auto data) {
                  std::ostringstream ss;
                  ss << "secNum=" << sequence << " data=";
                  for (auto &byte : data) {
                          ss << "[" << std::hex << (int) byte << "]";
                  }

                  Logger(LogLevel::INFO) << "Received: " << ss.str();
          });

          // Set callback to no
          ptr->setCloseListener([]() {
                  Logger(LogLevel::INFO) << "Closed";
          });
  }


In this snippet, we set the data to send and subscribe on the two events: the data is received and the connection is closed.

.. note::

   Pay attention, that the size of the data is the same as the O=>T connection size if the connection has the fixed size. 
   Some device can ignore this data and close the connection by timeout. 


To open a connection and set the listeners are not enough to make it work. **EIPScanner** is a single thread library and we 
need periodically to handle these connections:

.. code-block:: cpp

        int count = 200;
        while (connectionManager.hasOpenConnections() && count-- > 0) {
                connectionManager.handleConnections(std::chrono::milliseconds(100));
        }


This loop executes 200 times and while there are the open connections to handle. Method *ConnectionManager::handleConnections*
does several things for each IO connection:

   1. Checks if the new data received via UDP and calls the corresponding handler that has been set by 
      *IOConnection::setReceiveDataListener*.
   2. Closes a connection if it hasn't been receiving new data during *IOConnection::t2oAPI* x
      *ConnectionParameters::connectionTimeoutMultiplier* x 4 and calls the corresponding handler.
   3. Sends the data each *IOConnection::o2tAPI*.


.. note::

   You should call method *ConnectionManager::handleConnections* more often than the least API of opened connections. 


The last thing, we should do, is close the connection politely:

.. code-block:: cpp

   connectionManager.forwardClose(si, io)

