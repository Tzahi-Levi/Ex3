#ifndef MTMPARKINGLOT_PARKINGLOT_H
#define MTMPARKINGLOT_PARKINGLOT_H

#include "ParkingLotTypes.h"
#include "Time.h"
#include "ParkingSpot.h"
#include "../UniqueArray.h"
#include "Vehicle.h"

namespace MtmParkingLot {
	
    using namespace ParkingLotUtils;
    using std::ostream;
	typedef UniqueArray<Vehicle, std::equal_to<Vehicle>> CarArray;
    class ParkingLot {
    public:
		
        ParkingLot(unsigned int parkingBlockSizes[]);
        ~ParkingLot();
        ParkingResult enterParking(VehicleType vehicleType, LicensePlate licensePlate, Time entranceTime);
        ParkingResult exitParking(LicensePlate licensePlate, Time exitTime);
        ParkingResult getParkingSpot(LicensePlate licensePlate, ParkingSpot& parkingSpot) const;
        void inspectParkingLot(Time inspectionTime);
        friend ostream& operator<<(ostream& os, const ParkingLot& parkingLot);

	private:
		CarArray carLot;
		CarArray bikeLot;
		CarArray handicappedLot;

		ParkingResult enterHandicapped(Vehicle newCar, Time entranceTime);
		ParkingResult enterNormalCar(Vehicle newCar, Time entranceTime);
		ParkingResult enterMotorbike(Vehicle newCar, Time entranceTime);
    };
	
}

#endif //MTMPARKINGLOT_PARKINGLOT_H
