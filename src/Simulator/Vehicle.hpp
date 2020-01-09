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
#include <queue>
#include <SFML/Graphics.hpp>
#include "Map/Map.hpp"
#include "Flags.hpp"

using namespace std;
using namespace sf;

typedef enum {STOP, DRIVE, TURN, DELETE} State;
typedef enum {CAR, TRUCK, MOTORCYCLE} VehicleTypeOptions;

typedef struct
{
    VehicleTypeOptions Type;
    string VehicleTypeName;
    string ImageDir;
    int ImageCount;
    Vector2f Scale;
    float MaxSpeed{100};
    float MaxAcceleration{};
    float MinAcceleration{};
    vector<Texture> * Textures;
}
VehicleType;

class Vehicle
{

public:

    static list<Vehicle*> ActiveVehicles;
    static int VehicleCount;

    static void ClearVehicles();
    static void SetMaxSpeed(VehicleTypeOptions vehicleType, float max_speed, float max_acceleration);
    static void TransferVehicle(Vehicle * vehicle, Lane * toLane, Lane * fromLane = nullptr);
    static bool LoadVehicleTextures(VehicleType * vehicleType);
    static VehicleType * GetVehicleTypeByOption(VehicleTypeOptions vehicleTypeOptions);
    static Vehicle * GetVehicle(int vehicleNumber);
    static Vehicle * AddVehicle(queue<Lane*> * instructionSet, Map * map, VehicleTypeOptions vehicleType = CAR, int vehicleNumber = VehicleCount + 1);

    Vehicle(VehicleTypeOptions vehicleType, int vehicleNumber, queue<Lane*> * instructionSet, Map * map);
    ~Vehicle(){if(DRAW_DELETE)cout << "Vehicle " << m_vehicleNumber << " deleted" << endl;}

    void Draw(RenderWindow * window);
    void Update(float elapsedTime);

    Lane * GetSourceLane(){return m_sourceLane;}
    Lane * GetTargetLane(){return m_targetLane;}
    State       GetState(){return m_state;}
    int GetVehicleNumber(){return m_vehicleNumber;}

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
    float    m_minAcceleration;
    float    m_acceleration;
    float    m_rotation;
    float    m_angularV;

    Vehicle *m_vehicleInFront;

    int      m_vehicleNumber;
    queue<Lane *> * m_instructionSet;

    Map  *   m_currentMap;
    Lane *   m_sourceLane;
    Lane *   m_targetLane;
    //Lane *   m_currentLane;
    Intersection * m_currentIntersection;

    State    m_state;

    State drive();
    void  applyChanges(float elapsedTime);
    static float calculateDistance(Vector2f a, Vector2f b);

};



#endif /* Vehicle_hpp */

