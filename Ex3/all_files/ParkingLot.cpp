#include "ParkingLot.h"
#include "ParkingLotPrinter.h"
#include "UniqueArray.h"
using namespace MtmParkingLot;

ParkingLot::ParkingLot(unsigned int parkingBlockSizes[]) :
	finedCars(parkingBlockSizes[0] + parkingBlockSizes[1] + parkingBlockSizes[2]),
	bikeLot(parkingBlockSizes[0]), handicappedLot(parkingBlockSizes[2]), carLot(parkingBlockSizes[2])
{}

ParkingLot::~ParkingLot() {
	
}
ParkingResult ParkingLot::enterParking(VehicleType vehicleType, LicensePlate licensePlate, Time entranceTime) {
	
	if (vehicleType == HANDICAPPED) {
		return enterHandicapped(licensePlate, entranceTime);
	}
	if (vehicleType == CAR) {
		return enterNormalCar(licensePlate, entranceTime);
	}
	return enterMotorbike(licensePlate, entranceTime);
}

ParkingResult ParkingLot::enterHandicapped(LicensePlate licensePlate, Time entranceTime) {
	unsigned int index;
	HandicappedCar newCar = HandicappedCar(licensePlate, entranceTime);
	if (handicappedLot.getIndex(newCar, index) == true) {
		ParkingSpot spot = ParkingSpot(HANDICAPPED, index);
		ParkingLotPrinter::printEntryFailureAlreadyParked(std::cout, spot);
		(*handicappedLot[newCar]).printVehicle(std::cout);
		return VEHICLE_ALREADY_PARKED;
	}
	if (carLot.getIndex(newCar, index) == true) {
		ParkingSpot spot = ParkingSpot(CAR, index);
		ParkingLotPrinter::printEntryFailureAlreadyParked(std::cout, spot);
		*(carLot[newCar]).printVehicle(std::cout);
		
		return VEHICLE_ALREADY_PARKED;
	}
	if (!handicappedLot.isFull()) {
		index = handicappedLot.insert(newCar);
		ParkingSpot spot = ParkingSpot(HANDICAPPED, index);
		newCar.printVehicle(std::cout);
		ParkingLotPrinter::printEntrySuccess(std::cout, spot);
		return SUCCESS;
	}
	if (!carLot.isFull()) {
		index = carLot.insert(newCar);
		ParkingSpot spot = ParkingSpot(CAR, index);
		newCar.printVehicle(std::cout);
		ParkingLotPrinter::printEntrySuccess(std::cout, spot);
		return SUCCESS;
	}
	newCar.printVehicle(std::cout);
	ParkingLotPrinter::printEntryFailureNoSpot(std::cout);
	return NO_EMPTY_SPOT;
}

ParkingResult ParkingLot::enterNormalCar(LicensePlate licensePlate, Time entranceTime) {
	unsigned int index;
	Car newCar = Car(licensePlate, entranceTime);
	if (carLot.getIndex(newCar, index) == true) {
		ParkingSpot spot = ParkingSpot(CAR, index);
		(*carLot[newCar]).printVehicle(std::cout);
		ParkingLotPrinter::printEntryFailureAlreadyParked(std::cout, spot);
		return VEHICLE_ALREADY_PARKED;
	}
	if (carLot.isFull()) {
		newCar.printVehicle(std::cout);
		ParkingLotPrinter::printEntryFailureNoSpot(std::cout);
		return NO_EMPTY_SPOT;
	}
	index = carLot.insert(newCar);
	newCar.printVehicle(std::cout);
	ParkingSpot spot = ParkingSpot(CAR, index);
	ParkingLotPrinter::printEntrySuccess(std::cout, spot);
	return SUCCESS;

}

ParkingResult ParkingLot::enterMotorbike(LicensePlate licensePlate, Time entranceTime) {
	unsigned int index;
	MotorBike newCar = MotorBike(licensePlate, entranceTime);
	if (bikeLot.getIndex(newCar, index) == true) {
		ParkingSpot spot = ParkingSpot(MOTORBIKE, index);
		(*bikeLot[newCar]).printVehicle(std::cout);
		ParkingLotPrinter::printEntryFailureAlreadyParked(std::cout, spot);
		return VEHICLE_ALREADY_PARKED;
	}
	if (bikeLot.isFull()) {
		newCar.printVehicle(std::cout);
		ParkingLotPrinter::printEntryFailureNoSpot(std::cout);
		return NO_EMPTY_SPOT;
	}
	index = bikeLot.insert(newCar);
	newCar.printVehicle(std::cout);
	ParkingSpot spot = ParkingSpot(MOTORBIKE, index);
	ParkingLotPrinter::printEntrySuccess(std::cout, spot);
	return SUCCESS;

}

ParkingResult ParkingLot::exitParking(LicensePlate licensePlate, Time exitTime) {
    Time tempEntrance = Time(0,0,0);
    Vehicle tempVehicle = Vehicle(licensePlate, tempEntrance, CAR);
    Vehicle* tempCar = carLot[tempVehicle];
    unsigned int parkingSpot;
    carLot.getIndex(*tempCar, parkingSpot);
    if(tempCar) {
        if (carLot.remove(*tempCar)) {
            ParkingLotPrinter::printVehicle(std::cout, CAR, licensePlate, *tempCar->getEntryTime());
            ParkingLotPrinter::printExitSuccess(std::cout, ParkingSpot(CAR, parkingSpot), exitTime,
                                                *tempCar->getPrice(exitTime))
            return SUCCESS;
        }
    }
    Vehicle* tempBike = bikeLot[tempVehicle];
    bikeLot.getIndex(*tempBike, parkingSpot);
    if (tempBike) {
        if (bikeLot.remove(*tempBike)) {
            ParkingLotPrinter::printVehicle(std::cout, MOTORBIKE, licensePlate, *tempBike->getEntryTime());
            ParkingLotPrinter::printExitSuccess(std::cout, ParkingSpot(MOTORBIKE, parkingSpot), exitTime,
                    *tempBike->getPrice(exitTime))
            return SUCCESS;
            }
        }
    Vehicle* tempHandi = handicappedLot[tempVehicle];
    handicappedLot.getIndex(*tempHandi, parkingSpot);
    if (tempHandi) {
        if (handicappedLot.remove(*tempHandi)) {
            ParkingLotPrinter::printVehicle(std::cout, HANDICAPPED, licensePlate, *tempHandi->getEntryTime());
            ParkingLotPrinter::printExitSuccess(std::cout, ParkingSpot(HANDICAPPED, parkingSpot), exitTime,
                                                *tempHandi->getPrice(exitTime))
            return SUCCESS;
        }
    }
    ParkingLotPrinter::printExitFailure(std::cout, licensePlate);
    return VEHICLE_NOT_FOUND;
    }
}
