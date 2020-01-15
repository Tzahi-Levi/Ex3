
#ifndef  VEHICLE_H_
#define VEHICLE_H_


#include "Time.h"
#include "ParkingLot.h"
#include "ParkingLotTypes.h"
#include "ParkingLotPrinter.h"
using namespace ParkingLotUtils;
class Vehicle {
protected:
	int firstHourParkingRate;
	int generalParkingRate;
	Time entryTime;
	LicensePlate plate;
	VehicleType type;

public:	
	Vehicle	(LicensePlate plate, Time entrytime, VehicleType carType) : plate(plate),
			firstHourParkingRate(0), generalParkingRate(0), entryTime(entrytime), type(carType)
			{
	            if(type==CAR){
	                this->firstHourParkingRate=20;
	                this->generalParkingRate=10;
	            }
                if(type==HANDICAPPED){
                    this->firstHourParkingRate=15;
                    this->generalParkingRate=0;
                }
                this->firstHourParkingRate=10;
                this->generalParkingRate=5;
			};

	bool operator==(Vehicle& other) {
		return plate == other.plate;
	}

	int  getPrice(Time leaveTime);

	Time getEntryTime() {
		return entryTime;
	}
	Vehicle(const Vehicle& other):
		firstHourParkingRate(other.firstHourParkingRate),
		generalParkingRate(other.generalParkingRate),
		entryTime(other.entryTime),
		plate(other.plate),
		type(other.type)
    {};
	virtual ostream& printVehicle(ostream& os) {
		return (ParkingLotPrinter::printVehicle(os, type, plate, entryTime));
	}
};
int Vehicle::getPrice(Time leaveTime) {
	Time stayTime = leaveTime - entryTime;

	if (stayTime.toHours() == 1) {
		return firstHourParkingRate;
	}
	if (stayTime.toHours() > 5) {// at least 6
		return firstHourParkingRate + 5 * generalParkingRate;
	}
	return firstHourParkingRate + (stayTime.toHours() - 1) * generalParkingRate;
}
#endif //  VEHICLE_H_