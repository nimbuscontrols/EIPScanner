cmake_minimum_required(VERSION 3.5)

# Remember to update the LIBRARY_PATH and LD_LIBRARY_PATH environment variables
#   to the /usr/local/include/
#   and   /usr/local/lib

add_executable(explicit_messaging ExplicitMessagingExample.cpp)
target_link_libraries(explicit_messaging PUBLIC EIPScanner)

add_executable(file_object_example FileObjectExample.cpp)
target_link_libraries(file_object_example PUBLIC EIPScanner)

add_executable(identity_object_example IdentityObjectExample.cpp)
target_link_libraries(identity_object_example PUBLIC EIPScanner)

add_executable(implicit_messaging ImplicitMessagingExample.cpp)
target_link_libraries(implicit_messaging PUBLIC EIPScanner)

add_executable(parameter_object_example ParameterObjectExample.cpp)
target_link_libraries(parameter_object_example PUBLIC EIPScanner)

add_executable(discovery_example DiscoveryManagerExample.cpp)
target_link_libraries(discovery_example PUBLIC EIPScanner)

add_executable(yaskawa_assembly_object_example vendors/yaskawa/mp3300iec/Yaskawa_AssemblyObjectExample.cpp)
target_link_libraries(yaskawa_assembly_object_example PUBLIC EIPScanner)

if(WIN32)
  target_link_libraries(explicit_messaging ws2_32)
  target_link_libraries(file_object_example ws2_32)
  target_link_libraries(identity_object_example ws2_32)
  target_link_libraries(implicit_messaging ws2_32)
  target_link_libraries(parameter_object_example ws2_32)
  target_link_libraries(discovery_example ws2_32)
  target_link_libraries(yaskawa_assembly_object_example ws2_32)
endif()
