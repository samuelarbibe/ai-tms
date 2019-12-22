//
//  Vehicle.hpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 11/12/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#ifndef Vehicle_hpp
#define Vehicle_hpp

#include <iostream>
#include <cstring>
#include <SFML/Graphics.hpp>
#include "Intersection.hpp"

using namespace std;
using namespace sf;

typedef enum {STOP, DRIVE, TURN, DELETE} State;
typedef enum {CAR, TRUCK, MOTORCYCLE} VehicleTypeOptions;

typedef struct {
    VehicleTypeOptions Type;
    string VehicleTypeName;
    string ImageDir;
    int ImageCount;
    Vector2f Scale;
    float MaxSpeed{170};
    float MaxAcceleration{MaxSpeed/3.f};
    vector<Texture> * Textures;
}VehicleType;

class Vehicle
{

public:

    static list<Vehicle*> ActiveVehicles;
    static int VehicleCount;

    static void ClearVehicles();
    static void SetMaxSpeed(VehicleTypeOptions vehicleType, float max_speed);
    static void LoadVehicleTextures(VehicleType * vehicleType);
    static VehicleType * GetVehicleTypeByOption(VehicleTypeOptions vehicleTypeOptions);
    static Vehicle * GetVehicle(int vehicleNumber);
    static Vehicle * AddVehicle(Lane * sourceLane, Lane * destinationLane, Intersection * currentIntersection, VehicleTypeOptions vehicleType = CAR, int vehicleNumber = VehicleCount + 1);

    Vehicle(VehicleTypeOptions vehicleType, int vehicleNumber, Lane * sourceLane, Lane * destinationLane, Intersection * currentIntersection);
    ~Vehicle(){cout << "Vehicle " << m_vehicleNumber << " deleted" << endl;};

    void TransferVehicle(Vehicle * vehicle, Lane * fromLane, Lane * toLane);

    void Draw(RenderWindow * window);
    void Update(float elapsedTime);

    Lane * GetSourceLane(){return this->m_sourceLane;};
    Lane * GetTargetLane(){return this->m_targetLane;};
    State GetState(){return m_state;};
    int GetVehicleNumber(){return this->m_vehicleNumber;};

private:

    static int toBeDeleted;
    static VehicleType Car;
    static VehicleType Truck;
    static VehicleType Motorcycle;

    VehicleType * m_vehicleType;
    Vector2f m_position;

    Vector2f m_movementVec;
    Texture *m_texture;
    Sprite   m_sprite;

    float    m_speed;
    float    m_maxSpeed;
    float    m_maxAcceleration;
    float    m_acceleration{};
    float    m_rotation;
    float    m_angularV;

    Vehicle *m_vehicleInFront;

    int      m_vehicleNumber;

    Lane *   m_sourceLane;
    Lane *   m_targetLane;
    Lane *   m_currentLane;
    Intersection * m_currentIntersection;

    State    m_state;

    State drive();
    void  applyChanges(float elapsedTime);
    float calculateDistance(Vector2f a, Vector2f b);

};



#endif /* Vehicle_hpp */

