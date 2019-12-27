Logging
=======

**EIPScanner** has a very simple embedded logger *utils::Logger* which
prints the messages into stdout. This is a simple usage example:

.. code-block:: cpp

    #include "utils/Logger.h"

    using eipScanner::utils::Logger;
    using eipScanner::utils::LogLevel;

    int main() {
        Logger::setLogLevel(LogLevel::INFO);

        Logger(LogLevel::INFO) << "You will see this message.";
        Logger(LogLevel::DEBUG) << "You won't see this message.";
        return 0;
    }

You can set the lowest log level for all your application by method *Logger::setLogLevel*.
Here it is *INFO*, so that DEBUG level isn't printed.

Perhaps, the embedded logger isn't suitable for your application or you use another one.
No problems. Implement interface *utils::LogAppenderIf* and set it with method *Logger::setAppender*.

.. note::
    The printing messages happens in the destructor of *utils::Logger*. It means if you
    want to see message at once, don't save the logger in a variable or a field.