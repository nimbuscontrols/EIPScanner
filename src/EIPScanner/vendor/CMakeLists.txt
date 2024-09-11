include_directories(ra/powerFlex525)
include_directories(yaskawa/mp3300iec)

set( VENDOR_FILES
        # ra
        vendor/ra/powerFlex525/DPIFaultManager.cpp
        vendor/ra/powerFlex525/DPIFaultObject.cpp
        vendor/ra/powerFlex525/DPIFaultCode.cpp
        vendor/ra/powerFlex525/DPIFaultParameter.cpp

        # yaskawa
        vendor/yaskawa/mp3300iec/Yaskawa_EPath.cpp
        vendor/yaskawa/mp3300iec/Yaskawa_MessageRouter.cpp
        vendor/yaskawa/mp3300iec/Yaskawa_MessageRouterRequest.cpp

        PARENT_SCOPE
)