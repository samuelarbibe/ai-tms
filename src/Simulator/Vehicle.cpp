//
//  MovableObject.cpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 22/11/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#include "Vehicle.hpp"

int Vehicle::to_be_deleted_ = 0;
int Vehicle::ActiveVehiclesCount = 0;
int Vehicle::VehicleCount = 0;
int Vehicle::VehiclesToDeploy = 0;
list<Vehicle *> Vehicle::ActiveVehicles;
Vehicle *Vehicle::SelectedVehicle = nullptr;

VehicleType Vehicle::SmallCar {
	SMALL_CAR,
	"SmallCar",
	"../../resources/Cars/car_image_",
	3,
	Vector2f(1.6 * 100 / Settings::Scale, 3 * 100 / Settings::Scale)
};
VehicleType Vehicle::MediumCar{
	MEDIUM_CAR,
	"MediumCar",
	"../../resources/Cars/car_image_",
	3,
	Vector2f(1.8 * 100 / Settings::Scale, 4 * 100 / Settings::Scale)
};
VehicleType Vehicle::LongCar  {
	LONG_CAR,
	"LongCar",
	"../../resources/Cars/car_image_",
	3,
	Vector2f(2 * 100 / Settings::Scale, 5 * 100 / Settings::Scale)
};
VehicleType Vehicle::Truck    {
	TRUCK,
	"Truck",
	"../../resources/Cars/car_image_",
	3,
	Vector2f(2.1f * 100 / Settings::Scale, 10 * 100 / Settings::Scale)
};

Vehicle::Vehicle(VehicleTypeOptions vehicleType, int vehicleNumber, list<Lane *> *instructionSet, Map *map) {
	// set initial values for the movable object
	vehicle_type_ = GetVehicleTypeByOption(vehicleType);
	vehicle_number_ = vehicleNumber;
	max_speed_ = Settings::MaxSpeeds[vehicle_type_->Type];
	acceleration = Settings::Acceleration[vehicle_type_->Type];
	deceleration = Settings::Deceleration[vehicle_type_->Type];
	speed_ = 0;
	acc_ = 0;
	state_ = DRIVE;
	curr_map_ = map;
	instruction_set_ = instructionSet;
	source_lane_ = instruction_set_->front();
	instruction_set_->pop_front();
	dest_lane_ = instruction_set_->front();
	active_ = false;

	// get a pointer to the current intersection
	// current intersection is the intersection that the lane leads to
	curr_intersection_ = map->GetIntersection(source_lane_->GetIntersectionNumber());
	// the previous intersection, or the intersection of the source lane
	prev_intersection_ = nullptr;



	angular_vel_ = 0;
	turning_ = false;
	vehicle_in_front_ = nullptr;

	this->setSize(vehicle_type_->Size);
	this->setRotation(source_lane_->GetDirection());
	this->setPosition(source_lane_->GetStartPosition());
	this->setOutlineColor(Color::Blue);
	this->setOrigin(this->getSize().x / 2, this->getSize().y / 2);

	// if vehicle texture hasn't been loaded yet, load it
	if (Settings::DrawTextures && Vehicle::LoadVehicleTextures(vehicle_type_))
	{

		// set up sprite
		int textureNumber;
		if (Settings::MultiColor)
		{
			textureNumber = (vehicle_number_ % vehicle_type_->ImageCount);
		} else
		{
			textureNumber = 1;
		}

		this->setTexture(&(vehicle_type_->Textures->at(textureNumber)));
	} else
	{
		this->setOutlineThickness(10.f);
		this->setOutlineColor(Color::Blue);
		this->setFillColor(Color::Transparent);
	}

	data_box_ = new DataBox(this->getPosition());
	data_box_->AddData("Speed", speed_);
	data_box_->AddData("ID", vehicle_number_);
}

Vehicle::~Vehicle() {
	if (Vehicle::SelectedVehicle == this)
	{
		Vehicle::SelectedVehicle = nullptr;
	}
	if (Settings::DrawDelete)
		cout << "Vehicle " << vehicle_number_ << " deleted" << endl;
}

/// delete all active vehicles
void Vehicle::DeleteAllVehicles() {
	for (Vehicle *v : Vehicle::ActiveVehicles)
	{
		v->state_ = DELETE;
		to_be_deleted_++;
	}
	VehicleCount = 0;
	VehiclesToDeploy = 0;
	ClearVehicles();
}

/// clear the 'to be deleted' vehicles
void Vehicle::ClearVehicles() {
	auto it = ActiveVehicles.begin();

	// while there are cars to delete;
	while (to_be_deleted_ != 0 && it != ActiveVehicles.end())
	{
		// if is to be deleted
		if ((*it)->GetState() == DELETE)
		{
			Vehicle *temp = (*it);
			it = ActiveVehicles.erase(it);

			delete temp;

			to_be_deleted_--;
			ActiveVehiclesCount = ActiveVehicles.size();
			if (Settings::DrawActive)
				cout << "active vehicles : " << ActiveVehiclesCount << endl;
		} else
		{
			it++;
		}
	}
}

/// add a vehicle with an instruction set
Vehicle *Vehicle::AddVehicle(list<Lane *> *instructionSet,
                             Map *map,
                             VehicleTypeOptions vehicleType,
                             int vehicleNumber) {
	auto *temp = new Vehicle(vehicleType, vehicleNumber, instructionSet, map);
	ActiveVehicles.push_back(temp);

	temp->vehicle_in_front_ = (temp->source_lane_->GetBackVehicleId()) ? GetVehicle(temp->source_lane_->GetBackVehicleId())
	                                                             : nullptr;

	//set this car as the last car that entered the lane
	temp->source_lane_->PushVehicleInLane(vehicleNumber);
	ActiveVehiclesCount++;
	VehicleCount++;

	if (Settings::DrawAdded)
		cout << "car " << vehicleNumber << " added to lane " << temp->source_lane_->GetLaneNumber() << endl;

	return temp;
}

/// check if the click point is on a vehicle
Vehicle *Vehicle::CheckSelection(Vector2f position) {

	for (Vehicle *v : Vehicle::ActiveVehicles)
	{
		// only check for active vehicles
		if (v->active_)
		{
			if (v->getGlobalBounds().contains(position))
			{
				Vehicle::SelectedVehicle = v;
				Vehicle::SelectedVehicle->Select();
				return v;
			}
		}
	}
	return nullptr;
}

/// select a vehicle
void Vehicle::Select() {
	this->setOutlineColor(Color::Red);
	this->setFillColor(Color::Red);
}

/// unselect a vehicle
void Vehicle::Unselect() {
	this->setOutlineColor(Color::Blue);

	if (Settings::DrawTextures)
	{
		this->setFillColor(Color::White);
	} else
	{
		this->setFillColor(Color::Transparent);
	}
}

/// load textures as required
bool Vehicle::LoadVehicleTextures(VehicleType *vehicleType) {

	if (vehicleType->Textures == nullptr)
	{
		vehicleType->Textures = new vector<Texture>();

		string directory;
		Texture tempTexture;
		for (int i = 1; i <= vehicleType->ImageCount; ++i)
		{
			directory = vehicleType->ImageDir + to_string(i) + ".png";

			if (tempTexture.loadFromFile(directory))
			{
				tempTexture.setSmooth(true);
				vehicleType->Textures->push_back(tempTexture);
			} else
			{
				cerr << "loading texture no." << i << " for " << vehicleType->VehicleTypeName << " failed" << endl;
			}
		}

		cout << "------------------------------------------------------------------" << endl;
		cout << vehicleType->Textures->size() << "/" << vehicleType->ImageCount << " Textures successfully added for "
		     << vehicleType->VehicleTypeName
		     << endl;
		cout << "------------------------------------------------------------------" << endl;
		vehicleType->ImageCount = vehicleType->Textures->size();
	}
	return bool(vehicleType->ImageCount > 0);
}

/// convert vehicleTypeOption to VehicleType struct
VehicleType *Vehicle::GetVehicleTypeByOption(VehicleTypeOptions vehicleTypeOptions) {
	switch (vehicleTypeOptions)
	{
	case MEDIUM_CAR:return &(Vehicle::MediumCar);
	case LONG_CAR:return &(Vehicle::LongCar);
	case SMALL_CAR:return &(Vehicle::SmallCar);
	case TRUCK:return &(Vehicle::Truck);
	default:return &(Vehicle::MediumCar);
	}
}

/// get vehicle by vehicleNumber
Vehicle *Vehicle::GetVehicle(int vehicleNumber) {
	for (Vehicle *v : ActiveVehicles)
	{
		if (v->vehicle_number_ == vehicleNumber)
		{
			return v;
		}
	}

	return nullptr;
}

/// transfer a vehicle from a lane to another lane
void Vehicle::TransferVehicle(Vehicle *vehicle, Lane *toLane, Lane *fromLane) {
	if (vehicle == nullptr)
	{
		cout << "vehicle not found" << endl;
		return;
	}

	if (fromLane != nullptr)
	{
		if (vehicle->source_lane_->GetLaneNumber() != fromLane->GetLaneNumber())
		{
			cout << "vehicle is not in the given lane" << endl;
			return;
		}
		fromLane->PopVehicleFromLane();
	}

	vehicle->source_lane_ = toLane;
	vehicle->setRotation(vehicle->source_lane_->GetDirection());
	vehicle->angular_vel_ = 0;
	vehicle->setPosition(vehicle->source_lane_->GetStartPosition());
	vehicle->curr_intersection_ = vehicle->curr_map_->GetIntersection(vehicle->source_lane_->GetIntersectionNumber());
	vehicle->vehicle_in_front_ = (vehicle->source_lane_->GetBackVehicleId()) ? GetVehicle(vehicle->source_lane_->GetBackVehicleId())
	                                                                   : nullptr;
	vehicle->source_lane_->PushVehicleInLane(vehicle->vehicle_number_);

	vehicle->instruction_set_->pop_front();
	// if there are instructions left, transfer them to vehicle
	if (!vehicle->instruction_set_->empty())
	{
		vehicle->dest_lane_ = vehicle->instruction_set_->front();
	} else
	{
		vehicle->dest_lane_ = nullptr;
	}
}

/// do drive cycle
State Vehicle::drive() {
	// upon creation, all cars are stacked on each other.
	// while cars dont have a min distance, they wont start driving

	// check for distance with car in front
	if (vehicle_in_front_ != nullptr && vehicle_in_front_->state_ != DELETE && dest_lane_ != nullptr)
	{
		float distanceFromNextCar = Settings::CalculateDistance(this->getPosition(), vehicle_in_front_->getPosition())
			- this->getSize().y/2 - vehicle_in_front_->getSize().y/2;
		float brakingDistance = -(speed_ * speed_) / (2 * deceleration);

		if (distanceFromNextCar < brakingDistance + Settings::MinDistanceFromNextCar ||
			distanceFromNextCar < Settings::MinDistanceFromNextCar)
		{
			turning_ = false;
			state_ = STOP;
			acc_ = deceleration;
			return STOP;
		}
	}

	// check if car is in between lanes (inside an intersection)
	if (curr_intersection_->getGlobalBounds().contains(this->getPosition()) &&
		source_lane_ != nullptr &&
		dest_lane_ != nullptr)
	{
		if (!turning_)
		{
			float distanceSourceTarget = Settings::CalculateDistance(source_lane_->GetEndPosition(),
			                                                         dest_lane_->GetStartPosition());
			float angle = Settings::CalculateAngle(source_lane_->GetDirection(), dest_lane_->GetDirection());

			// if going in a straight line
			if (angle < 1.f && angle > -1.f)
			{
				angular_vel_ = 0;
			} else
			{
				float turningRadius = (distanceSourceTarget / 2.f) / (sin(angle * M_PI / 360.f));
				float turningParameter = 2.f * M_PI * turningRadius;
				float turningDistance = (angle / 360.f) * turningParameter;

				angular_vel_ = angle / turningDistance;
			}

			turning_ = true;
		}

		if (source_lane_ != nullptr)
		{
			prev_intersection_ = curr_map_->GetIntersection(source_lane_->GetIntersectionNumber());
			prev_intersection_->AddVehicleCount();
			source_lane_->PopVehicleFromLane();
			source_lane_ = nullptr;
		}

		state_ = TURN;
		//set rotation
		acc_ = (Settings::AccWhileTurning) ? acceleration / 2.f : 0;
		return TURN;
	}

	// check distance from stop (if lane is blocked)
	if (source_lane_ != nullptr && source_lane_ != dest_lane_ && source_lane_->GetIsBlocked() &&
		!this->getGlobalBounds().contains(source_lane_->GetEndPosition()))
	{
		float distanceFromStop = Settings::CalculateDistance(this->getPosition(), source_lane_->GetEndPosition())
			- this->getSize().y/2;
		float brakingDistance = -(speed_ * speed_) / (2 * deceleration);

		if (distanceFromStop < brakingDistance + Settings::MinDistanceFromStop)
		{
			turning_ = false;
			state_ = STOP;
			acc_ = deceleration;
			return STOP;
		}
	}

	// check if car has left intersection and is now in targetLane
	if (dest_lane_ != nullptr && dest_lane_->getGlobalBounds().contains(this->getPosition()))
	{
		// remove count from previous lane, and set in to nullptr
		prev_intersection_->RemoveVehicleCount();
		prev_intersection_ = nullptr;

		// we need to transfer vehicle to target lane
		TransferVehicle(this, dest_lane_, source_lane_);

		turning_ = false;
		acc_ = acceleration;
		state_ = DRIVE;
		return DRIVE;
	}

	// check if car is no longer in intersection
	if (dest_lane_ == nullptr && !source_lane_->getGlobalBounds().contains(this->getPosition()))
	{
		source_lane_->PopVehicleFromLane();

		turning_ = false;
		++to_be_deleted_;
		state_ = DELETE;
		return DELETE;
	}

	// default = just drive
	turning_ = false;
	acc_ = acceleration;
	state_ = DRIVE;
	return DRIVE;
}

/// update a vehicle's location
void Vehicle::Update(float elapsedTime) {

	if (Settings::DrawVehicleDataBoxes)
	{
		data_box_->Update(this->getPosition());
		data_box_->SetData("Speed", Settings::ConvertVelocity(PXS, KMH, speed_));
	}

	if(state_ != DELETE)
	{
		drive();

		// activate car
		if (! active_ && state_ == DRIVE)
			active_ = true;

		apply_changes(elapsedTime);
	}
}

/// apply the calculated next position
void Vehicle::apply_changes(float elapsed_time) {
	// apply acceleration
	speed_ += acc_ * elapsed_time * Settings::Speed;

	// apply max Speed limit
	if (speed_ > max_speed_)
		speed_ = max_speed_;

	// apply min Speed limit
	if (speed_ < 0)
		speed_ = 0;

	// set rotation relative to currentSpeed, to create a constant turning radius
	Transform t;
	this->rotate(angular_vel_ * elapsed_time * speed_ * Settings::Speed);

	t.rotate(this->getRotation());

	// rotate the movement vector in relation to the forward vector (0,1)
	movement_vec_ = t.transformPoint(Settings::BaseVec);

	// apply movement vector on position, relative to elapsed time to ensure
	// a constant Speed at any FPS

	this->move(movement_vec_ * speed_ * elapsed_time * Settings::Speed);
}

/// render the vehicle
void Vehicle::Draw(RenderWindow *window) {
	window->draw(*this);

	if (Settings::DrawVehicleDataBoxes)
		data_box_->Draw(window);
}



