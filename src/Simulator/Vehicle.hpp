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
#include "Settings.hpp"
#include "../UI/Widgets/DataBox.hpp"

using namespace std;
using namespace sf;

enum State{STOP, DRIVE, TURN, DELETE};

typedef struct
{
    VehicleTypeOptions Type;
    string VehicleTypeName;
    string ImageDir;
    int ImageCount;
    Vector2f Size;
    vector<Texture> * Textures;
}
VehicleType;

class Vehicle : RectangleShape
{

public:

    Vehicle(VehicleTypeOptions vehicleType, int vehicleNumber, queue<Lane*> * instructionSet, Map * map);
    ~Vehicle(){if(Settings::DrawDelete)cout << "Vehicle " << vehicle_number_ << " deleted" << endl;}

    void Draw(RenderWindow * window);
    void Update(float elapsedTime);

    Lane * GetSourceLane(){return source_lane_;}
    Lane * GetTargetLane(){return dest_lane_;}
    State  GetState(){return state_;}
    int    GetVehicleNumber(){return vehicle_number_;}
    bool   GetIsActive(){return active_;}
    Vector2f GetFrontPosition(){return front_position_;}
    Vector2f GetRearPosition(){return rear_position_;}

    static VehicleType * GetVehicleTypeByOption(VehicleTypeOptions vehicleTypeOptions);
    static Vehicle *     GetVehicle(int vehicleNumber);
    static Vehicle *     AddVehicle(queue<Lane*> * instructionSet, Map * map, VehicleTypeOptions vehicleType = CAR, int vehicleNumber = VehicleCount + 1);

    static void DeleteAllVehicles();
    static void ClearVehicles();
    static void TransferVehicle(Vehicle * vehicle, Lane * toLane, Lane * fromLane = nullptr);
    static bool LoadVehicleTextures(VehicleType * vehicleType);

    static list<Vehicle*> ActiveVehicles;
    static int VehicleCount;

private:

    static int to_be_deleted_;

    static VehicleType Car;
    static VehicleType Truck;
    static VehicleType Motorcycle;

    int      vehicle_number_;
    VehicleType * vehicle_type_;

    Vector2f movement_vec_;
    Vector2f front_position_;
    Vector2f rear_position_;

    float    speed_;
    float    acc_;
    float    max_speed_;
    float    max_acc_;
    float    min_acc_;
    float    angular_vel_;
    bool     turning_;
    bool     active_;

    Vehicle * vehicle_in_front_;

    queue<Lane *> * instruction_set_;

    Map  *   curr_map_;
    Lane *   source_lane_;
    Lane *   dest_lane_;
    Intersection * curr_intersection_;
    Intersection * prev_intersection_;

    State    state_;

    DataBox * data_box_;

    State drive();
    void  apply_changes(float elapsedTime);
};

#endif /* Vehicle_hpp */

