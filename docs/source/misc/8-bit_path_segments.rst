8-bit Path Segments
===================

Some devices only support 8-bit path segments. In order to set up
**EIPScanner** to use 8-bit path segments, specify the **use_8_bit_path_segments**
parameter when creating the *SessionInfo* object for the adapter.

.. code-block:: cpp

    #include <cip/connectionManager/NetworkConnectionParams.h>
    #include "ConnectionManager.h"
    #include "MessageRouter.h"
    #include "SessionInfo.h"

    using eipScanner::cip::connectionManager::ConnectionParameters;
    using eipScanner::ConnectionManager;
    using eipScanner::MessageRouter;
    using eipScanner::SessionInfo;

    int main()
    {
        auto si = std::make_shared<SessionInfo>("172.28.1.3", 0xAF12, true);

        /* The connection now uses 8-bit path segments for the forward open*/
        ConnectionManager connectionManager;
        ConnectionParameters parameters;
        auto io = connectionManager.forwardOpen(si, parameters);
        connectionManager.forwardClose(si, io);
   
        return 0;
    }


