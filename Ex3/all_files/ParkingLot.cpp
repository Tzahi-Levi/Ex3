#include "ParkingLot.h"
#include "ParkingLotPrinter.h"
#include "UniqueArray.h"

namespace MtmParkingLot {

	ParkingLot::ParkingLot(unsigned int parkingBlockSizes[]) :
		bikeLot(parkingBlockSizes[0]), handicappedLot(parkingBlockSizes[1]), carLot(parkingBlockSizes[2])
	{
		std::cout << "the amount of spots in bike lot is: " << bikeLot.getSize() << std::endl;
		std::cout << "the amount of spots in car lot is: " << carLot.getSize() << std::endl;
		std::cout << "the amount of spots in Handicapped lot is: " << handicappedLot.getSize() << std::endl;
	}

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
		for (unsigned int index = 0; index < carLot.getSize(); index++) {
			Vehicle* vehicleAtIndexPointer = (carLot.getElementAtIndex(index));
			if (vehicleAtIndexPointer != NULL) {//there is a car at that index
				if ((inspectionTime - (*vehicleAtIndexPointer).getEntryTime()).toHours() >= 24 and !(*vehicleAtIndexPointer).isFined()) {
					(*vehicleAtIndexPointer).fine();
					numberOfFined++;
				}
			}
		}
		for (unsigned int index = 0; index < bikeLot.getSize(); index++) {
			Vehicle* vehicleAtIndexPointer = (bikeLot.getElementAtIndex(index));
			if (vehicleAtIndexPointer != NULL) {//there is a bike at that index
				if ((inspectionTime - (*vehicleAtIndexPointer).getEntryTime()).toHours() >= 24 and !(*vehicleAtIndexPointer).isFined()) {
					(*vehicleAtIndexPointer).fine();
					numberOfFined++;
				}
			}
		}
		for (unsigned int index = 0; index < handicappedLot.getSize(); index++) {
			Vehicle* vehicleAtIndexPointer = (handicappedLot.getElementAtIndex(index));
			if (vehicleAtIndexPointer != NULL) {//there is a handicapped car at that index
				if ((inspectionTime - (*vehicleAtIndexPointer).getEntryTime()).toHours() >= 24 and !(*vehicleAtIndexPointer).isFined()) {
					(*vehicleAtIndexPointer).fine();
					numberOfFined++;
				}
			}
		}
		ParkingLotPrinter::printInspectionResult(std::cout, inspectionTime, numberOfFined);
	}

	ParkingResult ParkingLot::exitParking(LicensePlate licensePlate, Time exitTime) {
		Time tempEntrance = Time();
		Vehicle tempVehicle = Vehicle(licensePlate, tempEntrance, CAR);
		const Vehicle* tempCarPointer = carLot[tempVehicle];
		unsigned int parkingIndex;
		if (tempCarPointer) {
			carLot.getIndex(*tempCarPointer, parkingIndex);
			if (carLot.remove(*tempCarPointer)) {
				ParkingLotPrinter::printVehicle(std::cout, CAR, licensePlate, (*tempCarPointer).getEntryTime());
				ParkingLotPrinter::printExitSuccess(std::cout, ParkingSpot(CAR, parkingIndex), exitTime,
					(*tempCarPointer).getPrice(exitTime));
				return SUCCESS;
			}
		}
		const Vehicle* tempBikePointer = bikeLot[tempVehicle];
		if (tempBikePointer) {
			bikeLot.getIndex(*tempBikePointer, parkingIndex);
			if (bikeLot.remove(*tempBikePointer)) {
				ParkingLotPrinter::printVehicle(std::cout, MOTORBIKE, licensePlate, (*tempBikePointer).getEntryTime());
				ParkingLotPrinter::printExitSuccess(std::cout, ParkingSpot(MOTORBIKE, parkingIndex), exitTime,
					(*tempBikePointer).getPrice(exitTime));
				return SUCCESS;
			}
		}
		const Vehicle* tempHandiPointer = handicappedLot[tempVehicle];
		if (tempHandiPointer) {
			handicappedLot.getIndex(*tempHandiPointer, parkingIndex);
			if (handicappedLot.remove(*tempHandiPointer)) {
				ParkingLotPrinter::printVehicle(std::cout, HANDICAPPED, licensePlate, (*tempHandiPointer).getEntryTime());
				ParkingLotPrinter::printExitSuccess(std::cout, ParkingSpot(HANDICAPPED, parkingIndex), exitTime,
					(*tempHandiPointer).getPrice(exitTime));
				return SUCCESS;
			}
		}
		ParkingLotPrinter::printExitFailure(std::cout, licensePlate);
		return VEHICLE_NOT_FOUND;
	}

	ostream& operator<<(ostream& os, const ParkingLot& parkingLot) {
		ParkingLotPrinter::printParkingLotTitle(os);
		for (unsigned int i = 0; i < parkingLot.bikeLot.getSize(); ++i) {
			Vehicle* tempBikePointer = parkingLot.bikeLot.getElementAtIndex(i);
			if (tempBikePointer) {
				Vehicle tempBike = *tempBikePointer;
				ParkingSpot tempParkingSpot = ParkingSpot(MOTORBIKE, i);
				ParkingLotPrinter::printVehicle(os, MOTORBIKE, tempBike.getPlate(),
					tempBike.getEntryTime());
				os << ", ";
				ParkingLotPrinter::printParkingSpot(os, tempParkingSpot);
			}
		}
		for (unsigned int i = 0; i < parkingLot.handicappedLot.getSize(); ++i) {

			Vehicle* tempHandiCappedPointer = parkingLot.handicappedLot.getElementAtIndex(i);
			if (tempHandiCappedPointer) {
				Vehicle tempHandiCapped = *tempHandiCappedPointer;
				ParkingSpot tempParkingSpot = ParkingSpot(HANDICAPPED, i);
				ParkingLotPrinter::printVehicle(os, HANDICAPPED, tempHandiCapped.getPlate(),
					tempHandiCapped.getEntryTime());
				os << ", ";
				ParkingLotPrinter::printParkingSpot(os, tempParkingSpot);
			}
		}
		for (unsigned int i = 0; i < parkingLot.carLot.getSize(); ++i) {
			Vehicle* tempCarPointer = parkingLot.carLot.getElementAtIndex(i);
			if (tempCarPointer) {
				Vehicle tempCar = *tempCarPointer;
				ParkingSpot tempParkingSpot = ParkingSpot(CAR, i);
				ParkingLotPrinter::printVehicle(os, CAR, tempCar.getPlate(),
					tempCar.getEntryTime());
				os << ", ";
				ParkingLotPrinter::printParkingSpot(os, tempParkingSpot);
			}
		}
		return os;
	}

	

}