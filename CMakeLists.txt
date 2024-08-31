cmake_minimum_required(VERSION 3.28)
project(MicroMagPostProcessingTools)

set(CMAKE_CXX_STANDARD 20)

set(MMPPT_CMAKE_DIR "${CMAKE_SOURCE_DIR}/cmake")
set(MMPPT_DATA_DIR "${CMAKE_SOURCE_DIR}/data")
set(MMPPT_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(MMPPT_SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(MMPPT_SRC_TEST_DIR "${CMAKE_SOURCE_DIR}/src-test")
set(MMPPT_THIRD_PARTY_DIR "${CMAKE_SOURCE_DIR}/third-party")
set(MMPPT_TOY_DIR "${CMAKE_SOURCE_DIR}/toy")

set(CMAKE_CXX_STANDARD 20)

###############################################################################
# Find VTK                                                                    #
###############################################################################

find_package(VTK REQUIRED)

###############################################################################
# Find Qt                                                                     #
###############################################################################

find_package(Qt6 COMPONENTS Widgets REQUIRED)
qt_standard_project_setup()

###############################################################################
# Find Boost                                                                  #
###############################################################################

find_package(Boost COMPONENTS python REQUIRED)

###############################################################################
# Find HDF5                                                                   #
###############################################################################

find_package(HDF5 REQUIRED)

###############################################################################
# Find SQLite                                                                 #
###############################################################################

find_package(SQLite3 REQUIRED)

###############################################################################
# Build the toys.                                                             #
###############################################################################

add_subdirectory(${MMPPT_TOY_DIR})
