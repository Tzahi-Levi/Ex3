cmake_minimum_required(VERSION 3.15)
project(Ex3)

set(CMAKE_CXX_STANDARD 14)

add_executable(Ex3 UniqueArray.h UniqueArrayImp.h all_files/MtmParkingLot.cpp all_files/ParkingLot.cpp
        all_files/ParkingLot.h all_files/ParkingLotPrinter.h all_files/ParkingSpot.h all_files/ParkingSpot.cpp
        all_files/ParkingLotTypes.h all_files/Time.h all_files/Time.cpp all_files/ParkingLotPrinter.cpp all_files/Vehicle.h)