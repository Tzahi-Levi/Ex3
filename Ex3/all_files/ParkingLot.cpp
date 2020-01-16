#include "ParkingLot.h"
#include "ParkingLotPrinter.h"
#include "UniqueArray.h"

namespace MtmParkingLot {
	static ostream& printCarArray(ostream& os, const CarArray& arr, VehicleType type) {
		for (unsigned int i = 0; i < arr.getSize(); ++i) {
			Vehicle* tempCarPointer = arr.getElementAtIndex(i);
			if (tempCarPointer) {
				Vehicle tempCar = *tempCarPointer;
				ParkingSpot tempParkingSpot = ParkingSpot(type, i);
				ParkingLotPrinter::printVehicle(os, tempCar.getType(), tempCar.getPlate(),
					tempCar.getEntryTime());
				ParkingLotPrinter::printParkingSpot(os, tempParkingSpot);
			}
		}
		return os;
	}
	
	static int inspectCarArray(CarArray& arr, Time inspectionTime) {
		int numberOfFined = 0;
		for (unsigned int index = 0; index < arr.getSize(); index++) {
			Vehicle* vehicleAtIndexPointer = (arr.getElementAtIndex(index));
			if (vehicleAtIndexPointer != NULL) {//there is a car at that index
				if ((inspectionTime - (*vehicleAtIndexPointer).getEntryTime()).toHours() >= 24 and !(*vehicleAtIndexPointer).isFined()) {
					(*vehicleAtIndexPointer).fine();
					numberOfFined++;
				}
			}
		}
		return numberOfFined;
	}

	static ParkingResult exitFromCarArray(CarArray& arr, LicensePlate plate, VehicleType lot, Time exitTime) {
		Time tempEntrance = Time();
		Vehicle tempVehicle = Vehicle(plate, tempEntrance, lot);
		const Vehicle* tempCarPointer = arr[tempVehicle];
		unsigned int parkingIndex;
		if (tempCarPointer) {
			arr.getIndex(*tempCarPointer, parkingIndex);
			if (arr.remove(*tempCarPointer)) {
				ParkingLotPrinter::printVehicle(std::cout, (*tempCarPointer).getType(), plate, (*tempCarPointer).getEntryTime());
				ParkingLotPrinter::printExitSuccess(std::cout, ParkingSpot(lot, parkingIndex), exitTime,
					(*tempCarPointer).getPrice(exitTime));
				return SUCCESS;
			}
		}
   return VEHICLE_NOT_FOUND;
	}

	ParkingLot::ParkingLot(unsigned int parkingBlockSizes[]) :
		bikeLot(parkingBlockSizes[0]), handicappedLot(parkingBlockSizes[1]), carLot(parkingBlockSizes[2])
	{}

	ParkingResult ParkingLot::getParkingSpot(LicensePlate licensePlate, ParkingSpot& parkingSpot) const {
		Vehicle newCar = Vehicle(licensePlate, Time(), CAR);
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
		numberOfFined += inspectCarArray(carLot, inspectionTime);
		numberOfFined += inspectCarArray(bikeLot, inspectionTime);
		numberOfFined += inspectCarArray(handicappedLot, inspectionTime);
		ParkingLotPrinter::printInspectionResult(std::cout, inspectionTime, numberOfFined);
	}

	ParkingResult ParkingLot::exitParking(LicensePlate licensePlate, Time exitTime) {
		if (exitFromCarArray(carLot, licensePlate, CAR, exitTime) == SUCCESS) {
			return SUCCESS;
		}
		if (exitFromCarArray(bikeLot, licensePlate, MOTORBIKE, exitTime) == SUCCESS) {
			return SUCCESS;
		}
		if (exitFromCarArray(handicappedLot, licensePlate, HANDICAPPED, exitTime) == SUCCESS) {
			return SUCCESS;
		}
		ParkingLotPrinter::printExitFailure(std::cout, licensePlate);
   return VEHICLE_NOT_FOUND;
	}

	ostream& operator<<(ostream& os, const ParkingLot& parkingLot) {
		ParkingLotPrinter::printParkingLotTitle(os);
		printCarArray(os, parkingLot.bikeLot,MOTORBIKE);
		printCarArray(os, parkingLot.handicappedLot,HANDICAPPED);
		printCarArray(os, parkingLot.carLot,CAR);
		return os;
	}
}