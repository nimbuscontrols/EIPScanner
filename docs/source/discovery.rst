Discovery
=========

**EtherNet/IP** allows to discover **EIP** devices in the network
by using UDP broadcast messages.

**EIPScanner** provides *DiscoveryManager* class for this purpose:

.. literalinclude:: ../../examples/DiscoveryManagerExample.cpp

Method *DiscoveryManager::discover* sends broadcast UDP request in the network and
waits for the responses from the devices. It returns a vector of structures for
each discovered device that contain the IP addresses, ports and :ref:`identity_object`\ s.
If there is no device in the network it returns an empty vector.