8-bit Path Segments
===================

Some devices only support 8-bit path segments. In order to set up
**EIPScanner** to use 8-bit path segments, a *MessageRouter* with the
**USE_8_BIT_PATH_SEGMENTS** flag set should be passed to the *ConnectionManager*
upon construction:

.. code-block:: cpp

    MessageRouter::SPtr mr_ptr = std::make_shared<MessageRouter>(MessageRouter::USE_8_BIT_PATH_SEGMENTS);
    ConnectionManager _connectionManager = ConnectionManager(mr_ptr);



