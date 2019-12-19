.. _identity_object::

Identity Object (0x01)
======================

*Identity Object* provides identification and general information about the *CIP* devices.  It presents in all
*CIP* products.

You can read this information with *IdentityObject* class:

.. literalinclude:: ../../../examples/IdentityObjectExample.cpp

The constructor takes *instanceID* and *SessionInfo* as arguments to read data via *EtherNet/IP*.
