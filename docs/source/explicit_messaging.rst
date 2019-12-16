Explicit Messaging
==================

**EtherNet/IP** protocol provides the explicit messaging that is a RPC protocol working via TCP\IP.
To make a "call" we have to know the code of the service and the address of the instance or the class that
provides the wanted service.

**CIP** protocol uses *EPATH* to address objects in the object model. It contains ClassID of the object,
InstanceID of the instance and, optionally, AttributeID of the instance attribute. If we want to
call a class service we should use InstanceID=0.  

Let's say we have a *Widget Object* with ClassID=0xf0 and this object has service *PlusOne* with code 0x1 which
receives a integer as an argument and returns its increment. The service belongs to instance level, so we 
have some instance of *Widget Object* with InstanceID=0x5. 

So we have this request:

+----------------+----------+---------------------------------------+
|                | Type     | Value                                 |
+================+==========+=======================================+
| Service Code   | USINT    | 0x1                                   |
+----------------+----------+---------------------------------------+
| Address        | EPATH    | ClassID=0xf0, InstanceID=0x5          |
+----------------+----------+---------------------------------------+
| Argument       | INT      | 5                                     |
+----------------+----------+---------------------------------------+

And the response should be:

+----------------+----------+---------------------------------------+
|                | Type     | Value                                 |
+================+==========+=======================================+
| Service Code   | USINT    | 0x81  (response has code              |
|                |          | = service code | 0x80)                |
+----------------+----------+---------------------------------------+
| General Status | USINT    | 0 (SUCCESS)                           |
+----------------+----------+---------------------------------------+
| Result         | INT      | 6                                     |
+----------------+----------+---------------------------------------+

But whom should we send the request? In any EIP device, there is a special object for this.
It is *Message Router*. The router is responsible for receiving explicit requests, routing them to 
**CIP** objects and handling errors and results. 

This sounds a bit abstract. Let's see how it will be implemented in code:


.. code-block:: cpp

   #include <EIPScanner/MessageRouter.h>
   #include <EIPScanner/utils/Logger.h>
   #include <EIPScanner/utils/Buffer.h>

   using eipScanner::SessionInfo;
   using eipScanner::MessageRouter;
   using namespace eipScanner::cip;
   using namespace eipScanner::utils;

   int main() {
           try {
                   // Open EIP session with the adapter
                   auto si = std::make_shared<SessionInfo>("172.28.1.3", 0xAF12);

                   // Send Message Router Request
                   MessageRouter messageRouter;

                   const CipUsint PLUS_ON_SERVICE = 0x05;
                   const EPath EPATH_TO_WIDGET_INSTANCE(0xf0, 0x5);
                   Buffer buffer;
                   CipInt arg = 5;
                   buffer << arg;

                   auto response = messageRouter.sendRequest(si, 
                                          PLUS_ON_SERVICE, 
                                          EPATH_TO_WIDGET_INSTANCE, 
                                          buffer.data());

                   if (response.getGeneralStatusCode() == GeneralStatusCodes::SUCCESS) {
                           Buffer buffer(response.getData());
                           CipInt result;
                           buffer >> result;

                           Logger(LogLevel::INFO) << result;
                   }
           } catch (std::exception &e) {
                   Logger(LogLevel::ERROR) << e.what();
                   return -1;
           }

           return 0;
   }


First of all, we have to connect with the EIP adapter and establish EIP session with it.
We do it by using *SessionInfo* object:


.. code-block:: cpp

   auto si = std::make_shared<SessionInfo>("172.28.1.3", 0xAF12);


Here we're connecting with the EIP adapter by IP address 172.28.1.3 and port 0xAF12 (default port for the explicit messaging).

Then we should form and send a request to the *Message Router*:

.. code-block:: cpp

   MessageRouter messageRouter;

   const CipUsint PLUS_ON_SERVICE = 0x05;
   const EPath EPATH_TO_WIDGET_INSTANCE(0xf0, 0x5);
   Buffer buffer;
   CipInt arg = 5;
   buffer << arg;

   auto response = messageRouter.sendRequest(si, 
                        PLUS_ON_SERVICE, 
                        EPATH_TO_WIDGET_INSTANCE, 
                        buffer.data());


Unfortunately, we can't send the service arguments "as is". Instead we should encode them and decode the result according **CIP**
specification. To make it easer, **EIPScanner** provides a special class *utils::Buffer*. In this example, we encode 5 as INT type
and get the result as a byte vector with method *utils::Buffer::data()*. 

The result of the request is stroed in  *Message Router Response*:

.. code-block:: cpp

   if (response.getGeneralStatusCode() == GeneralStatusCodes::SUCCESS) {
         Buffer buffer(response.getData());
         CipInt result;
         buffer >> result;

         Logger(LogLevel::INFO) << result;
    }


First of all, we should check if the request is successful and only then we decode its data by using *Buffer*. In our example,
this is only one number of type INT. 


Reading\\Writing CIP Attributes
------------------------------

The most typical operations in the explicit communication are reading and writing **CIP** attributes. The example that we used above is 
suitable, but we should keep in mind 2 things:

   1. Use *cip::Epath* with *Attribute ID* which you're going to read or write an attribute. For an example EPath(1,2,3), where ClassId=1,
   InstanceId=2, AttributeId=3

   2. Use *cip::ServiceCodes* enum with the common service codes
