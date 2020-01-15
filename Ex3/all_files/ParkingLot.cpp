#include "ParkingLot.h"
#include "ParkingLotPrinter.h"
#include "UniqueArray.h"
using namespace MtmParkingLot;

ParkingLot::ParkingLot(unsigned int parkingBlockSizes[]) :
	bikeLot(parkingBlockSizes[0]), handicappedLot(parkingBlockSizes[2]), carLot(parkingBlockSizes[2])
{}

ParkingLot::~ParkingLot() {
	
}
ParkingResult ParkingLot::getParkingSpot(LicensePlate licensePlate, ParkingSpot& parkingSpot) const {
	Vehicle newCar = Vehicle(licensePlate, Time(),CAR);
	unsigned int index = 0;
	if (carLot.getIndex(newCar, index) == true) {
		parkingSpot = ParkingSpot(CAR, index);
		return SUCCESS;
	}
	if (handicappedLot.getIndex(newCar, index) == true) {
		parkingSpot = ParkingSpot(HANDICAPPED, index);
		return SUCCESS;
	}
	if (bikeLot.getIndex(newCar, index) == true) {
		parkingSpot = ParkingSpot(MOTORBIKE, index);
		return SUCCESS;
	}
	return VEHICLE_NOT_FOUND;
}

ParkingResult ParkingLot::enterParking(VehicleType vehicleType, LicensePlate licensePlate, Time entranceTime) {
	Vehicle newCar = Vehicle(licensePlate, entranceTime, vehicleType);
	ParkingSpot tempSpot = ParkingSpot();
	if (getParkingSpot(licensePlate, tempSpot) == SUCCESS) {
		if (tempSpot.getParkingBlock() == CAR) {
			Vehicle oldCar = *(carLot[newCar]);
			oldCar.printVehicle(std::cout);
			ParkingLotPrinter::printEntryFailureAlreadyParked(std::cout, tempSpot);
			return VEHICLE_ALREADY_PARKED;
		}
		if (tempSpot.getParkingBlock() == MOTORBIKE) {
			Vehicle oldCar = *(bikeLot[newCar]);
			oldCar.printVehicle(std::cout);
			ParkingLotPrinter::printEntryFailureAlreadyParked(std::cout, tempSpot);
			return VEHICLE_ALREADY_PARKED;
		}
		Vehicle oldCar = *(handicappedLot[newCar]);
		oldCar.printVehicle(std::cout);
		ParkingLotPrinter::printEntryFailureAlreadyParked(std::cout, tempSpot);
		return VEHICLE_ALREADY_PARKED;
	}
	if (vehicleType == HANDICAPPED) {
		return enterHandicapped(newCar, entranceTime);
	}
	if (vehicleType == CAR) {
		return enterNormalCar(newCar, entranceTime);
	}
	return enterMotorbike(newCar, entranceTime);
}

ParkingResult ParkingLot::enterHandicapped(Vehicle newCar, Time entranceTime) {
	unsigned int index;
	ParkingSpot tempSpot = ParkingSpot();
	
	if (!handicappedLot.isFull()) {
		index = handicappedLot.insert(newCar);
		ParkingSpot spot = ParkingSpot(HANDICAPPED, index);
		newCar.printVehicle(std::cout);
		ParkingLotPrinter::printEntrySuccess(std::cout, spot);
		return SUCCESS;
	}
	
	return enterNormalCar(newCar, entranceTime);
}

ParkingResult ParkingLot::enterNormalCar(Vehicle newCar, Time entranceTime) {
	unsigned int index;
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

ParkingResult ParkingLot::enterMotorbike(Vehicle newCar, Time entranceTime) {
	unsigned int index;
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

void ParkingLot::inspectParkingLot(Time inspectionTime) {
	unsigned int numberOfFined = 0;
	for (unsigned int index = 0; index < carLot.getSize(); index++) {
		Vehicle* vehicleAtIndexPointer = (carLot.getElementAtIndex(index));
		if (vehicleAtIndexPointer != NULL) {//there is a car at that index
			Vehicle vehicleAtIndex = *vehicleAtIndexPointer;
			if ((inspectionTime - vehicleAtIndex.getEntryTime()).toHours() >= 24) {
					vehicleAtIndex.fine();
					numberOfFined++;
			}
		}
	}
	for (unsigned int index = 0; index < bikeLot.getSize(); index++) {
		Vehicle* vehicleAtIndexPointer = (bikeLot.getElementAtIndex(index));
		if (vehicleAtIndexPointer != NULL) {//there is a bike at that index
			Vehicle vehicleAtIndex = *vehicleAtIndexPointer;
			if ((inspectionTime - vehicleAtIndex.getEntryTime()).toHours() >= 24) {
				vehicleAtIndex.fine();
			}
		}
	}
	for (unsigned int index = 0; index < handicappedLot.getSize(); index++) {
		Vehicle* vehicleAtIndexPointer = (handicappedLot.getElementAtIndex(index));
		if (vehicleAtIndexPointer != NULL) {//there is a handicapped car at that index
			Vehicle vehicleAtIndex = *vehicleAtIndexPointer;
			if ((inspectionTime - vehicleAtIndex.getEntryTime()).toHours() >= 24) {
				vehicleAtIndex.fine();
			}
		}
	}
	ParkingLotPrinter::printInspectionResult(std::cout, inspectionTime, numberOfFined);
}

