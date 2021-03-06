#ifndef  VEHICLE_H_
#define VEHICLE_H_


#include "Time.h"
#include "ParkingLot.h"
#include "ParkingLotTypes.h"
#include "ParkingLotPrinter.h"
namespace ParkingLotUtils {
	class Vehicle {
	private:
    LicensePlate plate;
    Time entryTime;
    VehicleType type;
		int firstHourParkingRate;
		int generalParkingRate;		
		bool fined;

	public:
		static const int FINE_COST = 250;
		static const int MAX_PAYING_HOURS = 6;
		static const int CAR_INITIAL = 20;
		static const int CAR_PER_HOUR = 10;
		static const int HANDICAPPED_INITIAL = 15;
		static const int HANDICAPPED_PER_HOUR = 0;
		static const int MOTORBIKE_INITIAL = 10;
		static const int MOTORBIKE_PER_HOUR = 5;

		Vehicle(LicensePlate plate, Time entrytime, VehicleType carType) : plate(plate), entryTime(entrytime),type(carType),firstHourParkingRate(0), 
                                                                       generalParkingRate(0), fined(false){
			if (type == CAR) {
				this->firstHourParkingRate = CAR_INITIAL;
				this->generalParkingRate = CAR_PER_HOUR;
			}
			if (type == HANDICAPPED) {
				this->firstHourParkingRate = HANDICAPPED_INITIAL;
				this->generalParkingRate = HANDICAPPED_PER_HOUR;
			}

			if (type == MOTORBIKE) {
				this->firstHourParkingRate = MOTORBIKE_INITIAL;
				this->generalParkingRate = MOTORBIKE_PER_HOUR;
			}
		};

		bool operator==(const Vehicle& other)const {
			return plate == other.plate;
		}

		int  getPrice(Time leaveTime) const {
			Time stayTime = leaveTime - entryTime;
			unsigned int fine = 0;
			if (fined) {
				fine = FINE_COST;
			}
			if (stayTime.toHours() == 0) {
				return firstHourParkingRate + fine;
			}
			if (stayTime.toHours() > MAX_PAYING_HOURS - 1) {// at least 6
				return fine + firstHourParkingRate + (5 * generalParkingRate);
			}
			return fine + firstHourParkingRate + (stayTime.toHours() - 1) * generalParkingRate;
		}

		Time getEntryTime() const {
			return entryTime;
		}

		ostream& printVehicle(ostream& os)const {
			return (ParkingLotPrinter::printVehicle(os, type, plate, entryTime));
		}

		void fine() {
			fined = true;
		}

		bool isFined() {
			return fined;
		}
		
		LicensePlate getPlate() const {
			return plate;
		}

		VehicleType getType() const {
			return type;
		}
	};
}
#endif //  VEHICLE_H_ 