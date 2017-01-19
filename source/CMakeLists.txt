########################################################
# cmake file for building Marlin example Package
# @author Jan Engels, Desy IT
#CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)
########################################################

### LIBRARY #################################################################

# definitions to pass to the compiler

# ADD_DEFINITIONS( "-pedantic -Wunused-value -O2" )
# ADD_DEFINITIONS("-Wno-long-long -Wreturn-type -Werror")

ADD_DEFINITIONS( "-Wall -ansi -pedantic" )
ADD_DEFINITIONS( "-Wno-long-long" )

# include directories
INCLUDE_DIRECTORIES( ./include )
INSTALL_DIRECTORY( ./include DESTINATION . FILES_MATCHING PATTERN "*.h" )

FILE(GLOB_RECURSE toto "include/*.h")
add_custom_target(headers SOURCES ${toto})

# add library
AUX_SOURCE_DIRECTORY( ./src/CaloObject library_sources )
AUX_SOURCE_DIRECTORY( ./src/Algorithm library_sources )
ADD_SHARED_LIBRARY( ${PROJECT_NAME} ${library_sources} )
INSTALL_SHARED_LIBRARY( ${PROJECT_NAME} DESTINATION lib )
