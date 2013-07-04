cmake_minimum_required (VERSION 2.8) 

# installer related
set(CPACK_COMPONENTS_ALL_IN_ONE_PACKAGE 1)
set(CPACK_PACKAGE_NAME "cclib")
set(CPACK_SYSTEM_NAME "${CMAKE_SYSTEM_NAME}")
set(CPACK_PACKAGE_VENDOR "ART+COM Ag (www.artcom.de)")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Port of the Creative Computing Particle System")
set(CPACK_PACKAGE_VERSION "0.1.0")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "cclib")
# set(CMAKE_OSX_ARCHITECTURES "i386")
enable_testing()
project (cclib) 

set(CCLIB_VERSION_MAJOR 0)
set(CCLIB_VERSION_MINOR 1)

# set(CMAKE_C_FLAGS "-arch i386 -Wno-reorder ${CMAKE_C_FLAGS}")
# set(CMAKE_CXX_FLAGS "-arch i386 -Wno-reorder ${CMAKE_CXX_FLAGS}")

add_subdirectory (src) 

include(CPack)

