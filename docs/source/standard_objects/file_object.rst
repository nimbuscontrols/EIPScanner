File Object (0x37)
==================

*File Object* allows to transfer files between a scanner and an adapter. It might be helpful if you want to read *EDS*
file from the **EIP** device.

**EIPScanner** implements only reading files with *FileObject* class.
Below you can see how to read *EDS* file from the device:

.. literalinclude:: ../../../examples/FileObjectExample.cpp

*File Object* sends data split into chunks (max 255), so we need to receive all of them after we've begun uploading.
Method *FileObject::handleTransfers* receives all the chunks and calls a handler, which we set in method *FileObject::beginUpload*,
where we save the received data as a file.