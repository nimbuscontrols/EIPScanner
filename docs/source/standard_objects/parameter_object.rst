Parameter Object (0x0f)
=======================

*Parameter Object* provides a standard way to access to data and configuration of the **CIP** device.

**EIPScanner** has special class *ParameterObject* to read an parameter, but before use it you should
know:

    1. *Instance ID* of the parameter or how many parameters the device has to read all of them
    2. If the device supports full attributes of Paramter Object (sting descriptions, scaling, etc.) or not

The following example shows how you can get the necessary information from Parameter Object class and read all the
parameters from **EIP** device:

.. literalinclude:: ../../../examples/ParameterObjectExample.cpp

The example is pretty big and we need to look at it closer:

Below we read the number of parameters in the device by reading *Max Instance* attribute of *Parameter Object* class.
For an example, if the number equals 5, we have 5 *Parameter Object* instances with IDs from 1 to 5:

.. code-block:: cpp

    MessageRouter messageRouter;
    auto response = messageRouter.sendRequest(si
            , ServiceCodes::GET_ATTRIBUTE_SINGLE
            , EPath(ParameterObject::CLASS_ID, 0, MAX_INSTANCE));

    if (response.getGeneralStatusCode() != GeneralStatusCodes::SUCCESS) {
        Logger(LogLevel::ERROR) << "Failed to read the count of the parameters";
        logGeneralAndAdditionalStatus(response);
        return -1;
    }

    Buffer buffer(response.getData());
    CipUint paramsCount;
    buffer >> paramsCount;


But to know the number of the parameters is not enough. We need to figure out if the parameters support full attributes.
This information is stored in the second bit of *Parameter Classe Descriptor* and we have to read it:

.. code-block:: cpp

    response = messageRouter.sendRequest(si
            , ServiceCodes::GET_ATTRIBUTE_SINGLE
            , EPath(ParameterObject::CLASS_ID, 0, CLASS_DESCRIPTOR));

    if (response.getGeneralStatusCode() != GeneralStatusCodes::SUCCESS) {
        Logger(LogLevel::ERROR) << "Failed to read the class descriptor";
        logGeneralAndAdditionalStatus(response);
        return -1;
    }

    buffer = Buffer(response.getData());
    CipUint descriptor;
    buffer >> descriptor;

    Logger(LogLevel::INFO) << "Read the class descriptor=0x" << std::hex << (int)descriptor;
    bool allAttributes = descriptor & SUPPORTS_FULL_ATTRIBUTES;

Now we know all that we need and we are able to read the parameters and save them in a vector:

.. code-block:: cpp

    std::vector<ParameterObject> parameters;
    parameters.reserve(paramsCount);
    for (int i = 0; i < paramsCount; ++i) {
        parameters.emplace_back(i+1, allAttributes, si);
    }

When the parameters are read in its constructors, you can access to its attributes by using the getters:

.. code-block:: cpp

    if (!parameters.empty()) {
        parameters[0].getType(); // Read type
        parameters[0].getActualValue<CipUint>(); // 2040
        parameters[0].getEngValue<CipUint>(); // 20.4
        parameters[0].getName();	// Freq
        parameters[0].getUnits(); 	// Hz
        // .. etc

        parameters[0].updateValue(si);
        parameters[0].getActualValue<CipUint>(); // updated value
    }

.. note::
    After an instance of *ParamterObject* is created it doesn't update its attributes. You can update only the actual
    value with method *ParamterObject::updateValue*