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

enum State
{
	STOP, DRIVE, TURN, DELETE
};

typedef struct
{
	VehicleTypeOptions Type;
	string VehicleTypeName;
	string ImageDir;
	int ImageCount;
	Vector2f Size;
	vector<Texture> *Textures;
}
	VehicleType;

class Vehicle : RectangleShape
{

  public:

	Vehicle(VehicleTypeOptions vehicleType, int vehicleNumber, list<Lane *> *instructionSet, Map *map);
	~Vehicle();

	void Draw(RenderWindow *window);
	void Update(float elapsedTime);

	// add entities
	static Vehicle *AddVehicle(list<Lane *> *instructionSet,
	                           Map *map,
	                           VehicleTypeOptions vehicleType = CAR,
	                           int vehicleNumber = VehicleCount + 1);

	// get
	int GetVehicleNumber() { return vehicle_number_; }
	bool GetIsActive() { return active_; }
	Lane *GetSourceLane() { return source_lane_; }
	Lane *GetTargetLane() { return dest_lane_; }
	Lane *GetCurrentLane() { return source_lane_; }
	State GetState() { return state_; }
	Vector2f GetFrontPosition() { return front_position_; }
	Vector2f GetRearPosition() { return rear_position_; }
	Vector2f GetPosition() { return getPosition(); }
	list<Lane *> *GetInstructionSet() { return instruction_set_; }
	static VehicleType *GetVehicleTypeByOption(VehicleTypeOptions vehicleTypeOptions);
	static Vehicle *GetVehicle(int vehicleNumber);

	// set
	void Select();
	void Unselect();

	static Vehicle *CheckSelection(Vector2f position);

	static void DeleteAllVehicles();
	static void ClearVehicles();
	static void TransferVehicle(Vehicle *vehicle, Lane *toLane, Lane *fromLane = nullptr);
	static bool LoadVehicleTextures(VehicleType *vehicleType);

	static list<Vehicle *> ActiveVehicles;
	static int GetActiveVehicleCount() { return active_vehicles_count_; }
	static int VehicleCount;
	static Vehicle *SelectedVehicle;

  private:

	static int to_be_deleted_;
	static int active_vehicles_count_;

	static VehicleType Car;
	static VehicleType Truck;
	static VehicleType Motorcycle;

	int vehicle_number_;
	VehicleType *vehicle_type_;

	Vector2f movement_vec_;
	Vector2f front_position_;
	Vector2f rear_position_;

	float speed_;
	float acc_;
	float max_speed_;
	float max_acc_;
	float min_acc_;
	float angular_vel_;
	bool turning_;
	bool active_;

	Vehicle *vehicle_in_front_;

	list<Lane *> *instruction_set_;

	Map *curr_map_;
	Lane *source_lane_;
	Lane *dest_lane_;
	Intersection *curr_intersection_;
	Intersection *prev_intersection_;

	State state_;

	DataBox *data_box_;

	State drive();
	void apply_changes(float elapsedTime);
};

#endif /* Vehicle_hpp */

