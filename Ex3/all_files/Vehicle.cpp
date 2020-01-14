//
// Created by ZachiLoy on 12/01/2020.
//
#include "Time.h"
class Vehicle {
private:
    unsigned int licensePlate;
    double firstHourParkingRate;
    double generalParkingRate;
    Time start;
public:
    Vehicle(unsigned int licensePlate, double first, double general) : licensePlate(licensePlate),
                                                                        firstHourParkingRate(first),
                                                                       generalParkingRate(general){};
    bool operator==(const Vehicle& vehicle) const {
        return (vehicle.licensePlate == this->licensePlate);
    }
    unsigned int getPlate(){
        return this->licensePlate;
    }
};

class Motorbike : public Vehicle{
private:
    unsigned int type;
public:
    explicit Motorbike(unsigned int licensePlate,double first, double general, unsigned int type):
                                                                        Vehicle(licensePlate, 10, 5),
                                                                        type(0){};
    unsigned int getType(){
        return this->type;
    }
};

class HandicappedCar : public Vehicle{
private:
    unsigned int type;
public:
    explicit HandicappedCar(unsigned int licensePlate,double first, double general, unsigned int type):
                                                                        Vehicle(licensePlate, 15, 0),
                                                                        type(1){};
    unsigned int getType(){
        return this->type;
    }
};

class Car : public Vehicle{
private:
    unsigned int type;
public:
    explicit Car(unsigned int licensePlate,double first, double general, unsigned int type):
                                                                        Vehicle(licensePlate, 20, 10),
                                                                        type(2){};
    unsigned int getType(){
        return this->type;
    }
};