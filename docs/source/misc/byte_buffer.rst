.. _buffer:

Byte Buffer
===========

**EtherNet/IP** has a type system and specifies how the types must
be sends over the network. So we have to decode and encode our C++ types.
To make it easy, **EIPScanner** have class **utils::Buffer*.

To decode the data that we've received from the network use operator >>:

.. code-block:: cpp

    Buffer buffer(dataFromNetwork);
    cip::CipUint var1;
    cip::CipReal var2;
    buffer >> var1 >> var2;

To encode the data that we are going to send, use operator <<:

.. code-block:: cpp

    Buffer buffer;
    cip::CipUint var1 = 10;
    cip::CipReal var2 = 2.4;
    buffer << var1 << var2;

    buffer.data(); // => data to send

