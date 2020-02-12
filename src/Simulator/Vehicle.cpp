//
//  MovableObject.cpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 22/11/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#include "Vehicle.hpp"

const Vector2f m_forwardVec{0.f, -1.f};

int Vehicle::to_be_deleted_{0};
int Vehicle::VehicleCount = 0;
list<Vehicle *> Vehicle::ActiveVehicles;

VehicleType Vehicle::Car{CAR, "Car", "../../resources/Cars/car_image_", 5, Vector2f(0.6f, 0.6f)};
VehicleType Vehicle::Truck{TRUCK, "Truck", "../../resources/Cars/car_image_", 5, Vector2f(0.12f, 0.12f)};
VehicleType Vehicle::Motorcycle{MOTORCYCLE, "Motorcycle", "../resources/Cars/motorcycle_image_", 5,
                                Vector2f(0.12f, 0.12f)};

Vehicle::Vehicle(VehicleTypeOptions vehicleType, int vehicleNumber, queue<Lane *> *instructionSet, Map *map)
{
    // set initial values for the movable object
    vehicle_type_ = GetVehicleTypeByOption(vehicleType);
    vehicle_number_ = vehicleNumber;
    speed_ = 0.f;
    acc_ = max_acc_;
    state_ = DRIVE;
    curr_map_ = map;
    instruction_set_ = instructionSet;
    source_lane_ = instruction_set_->front();
    instruction_set_->pop();
    dest_lane_ = instruction_set_->front();

    // get a pointer to the current intersection
    // current intersection is the intersection that the lane leads to
    curr_intersection_ = map->GetIntersection(source_lane_->GetIntersectionNumber());
    // the previous intersection, or the intersection of the source lane
    prev_intersection_ = nullptr;

    max_speed_ = Settings::MaxSpeeds[vehicle_type_->Type];
    max_acc_ = Settings::MaxAcceleration[vehicle_type_->Type];
    min_acc_ = Settings::MinAcceleration[vehicle_type_->Type];

    angular_vel_ = 0;
    rotation_ = source_lane_->GetDirection();
    position_ = source_lane_->GetStartPosition();
    vehicle_in_front_ = nullptr;

    // if vehicle texture hasn't been loaded yet, load it
    if (Vehicle::LoadVehicleTextures(vehicle_type_)) {

        // set up sprite
        int textureNumber;
        if (Settings::MultiColor) {
            textureNumber = (vehicle_number_ % vehicle_type_->ImageCount);
        } else {
            textureNumber = 1;
        }

        texture_ = &(vehicle_type_->Textures->at(textureNumber));
        sprite_.setTexture(*texture_);
    }
    sprite_.setScale(vehicle_type_->Scale);
    sprite_.setOrigin(sprite_.getTextureRect().width / 2, sprite_.getTextureRect().height / 3);

    data_box_ = new DataBox(position_);
    data_box_->AddData("Speed", speed_);
    data_box_->AddData("ID", vehicle_number_);
}

/// delete all active vehicles
void Vehicle::DeleteAllVehicles()
{
    for (Vehicle *v : Vehicle::ActiveVehicles) {
        v->state_ = DELETE;
        to_be_deleted_++;
    }

    ClearVehicles();
}

/// clear the 'to be deleted' vehicles
void Vehicle::ClearVehicles()
{
    auto it = ActiveVehicles.begin();

    // while there are cars to delete;
    while (to_be_deleted_ != 0 && it != ActiveVehicles.end()) {
        // if is to be deleted
        if ((*it)->GetState() == DELETE) {
            Vehicle *temp = (*it);
            it = ActiveVehicles.erase(it);

            delete temp;

            to_be_deleted_--;
            if (Settings::DrawActive)cout << "active vehicles : " << ActiveVehicles.size() << endl;
        } else {
            it++;
        }
    }
}

/// add a vehicle with an instruction set
Vehicle *Vehicle::AddVehicle(queue<Lane *> *instructionSet, Map *map, VehicleTypeOptions vehicleType, int vehicleNumber)
{
    auto *temp = new Vehicle(vehicleType, vehicleNumber, instructionSet, map);
    ActiveVehicles.push_back(temp);

    temp->vehicle_in_front_ = (temp->source_lane_->GetLastCar()) ? GetVehicle(temp->source_lane_->GetLastCar())
                                                                 : nullptr;


    //set this car as the last car that entered the lane
    temp->source_lane_->SetLastCar(vehicleNumber);
    temp->source_lane_->AddVehicleCount();
    VehicleCount++;

    if (Settings::DrawAdded)
        cout << "car " << vehicleNumber << " added to lane " << temp->source_lane_->GetLaneNumber() << endl;

    return temp;
}

/// load textures as required
bool Vehicle::LoadVehicleTextures(VehicleType *vehicleType)
{
    if (vehicleType->Textures == nullptr) {
        vehicleType->Textures = new vector<Texture>();

        string directory;
        Texture tempTexture;
        for (int i = 1; i <= vehicleType->ImageCount; ++i) {
            directory = vehicleType->ImageDir + to_string(i) + ".png";

            if (tempTexture.loadFromFile(directory)) {
                tempTexture.setSmooth(true);
                vehicleType->Textures->push_back(tempTexture);
            } else {
                cerr << "loading texture no." << i << " for " << vehicleType->VehicleTypeName << " failed" << endl;
            }
        }

        cout << "----------------------------------------------" << endl;
        cout << vehicleType->Textures->size() << "/" << vehicleType->ImageCount << " Textures successfully added"
             << endl;
        cout << "----------------------------------------------" << endl;
        vehicleType->ImageCount = vehicleType->Textures->size();
    }
    return vehicleType->ImageCount > 0 ? true : false;
}

/// convert vehicleTypeOption to VehicleType struct
VehicleType *Vehicle::GetVehicleTypeByOption(VehicleTypeOptions vehicleTypeOptions)
{
    switch (vehicleTypeOptions) {
        case TRUCK:
            return &(Vehicle::Truck);
        case MOTORCYCLE:
            return &(Vehicle::Motorcycle);
        case CAR:
        default:
            return &(Vehicle::Car);
    }
}

/// get vehicle by vehicleNumber
Vehicle *Vehicle::GetVehicle(int vehicleNumber)
{
    for (Vehicle *v : ActiveVehicles) {
        if (v->vehicle_number_ == vehicleNumber) {
            return v;
        }
    }

    return nullptr;
}

/// transfer a vehicle from a lane to another lane
void Vehicle::TransferVehicle(Vehicle *vehicle, Lane *toLane, Lane *fromLane)
{
    if (vehicle == nullptr) {
        cout << "vehicle not found" << endl;
        return;
    }

    if (fromLane != nullptr) {
        if (vehicle->source_lane_->GetLaneNumber() != fromLane->GetLaneNumber()) {
            cout << "vehicle is not in the given lane" << endl;
            return;
        }
        fromLane->RemoveVehicleCount();
    }

    vehicle->source_lane_ = toLane;
    vehicle->rotation_ = vehicle->source_lane_->GetDirection();
    vehicle->angular_vel_ = 0;
    vehicle->curr_intersection_ = vehicle->curr_map_->GetIntersection(vehicle->source_lane_->GetIntersectionNumber());
    vehicle->vehicle_in_front_ = (vehicle->source_lane_->GetLastCar()) ? GetVehicle(vehicle->source_lane_->GetLastCar())
                                                                       : nullptr;
    vehicle->source_lane_->SetLastCar(vehicle->vehicle_number_);
    vehicle->source_lane_->AddVehicleCount();

    vehicle->instruction_set_->pop();
    // if there are instructions left, transfer them to vehicle
    if (!vehicle->instruction_set_->empty()) {
        vehicle->dest_lane_ = vehicle->instruction_set_->front();
    } else {
        vehicle->dest_lane_ = nullptr;
    }
}

/// do drive cycle
State Vehicle::drive()
{
    // upon creation, all cars are stacked on each other.
    // while cars dont have a min distance, they wont start driving

    // check for distance with car in front
    if (vehicle_in_front_ != nullptr && vehicle_in_front_->state_ != DELETE) {
        float distanceFromNextCar = Settings::CalculateDistance(position_, vehicle_in_front_->position_);
        //cout << distanceFromNextCar << endl;
        float brakingDistance = -(speed_ * speed_) / (2 * min_acc_);

        if (distanceFromNextCar < brakingDistance + Settings::MinDistanceFromNextCar ||
            distanceFromNextCar < Settings::MinDistanceFromNextCar) {
            state_ = STOP;
            acc_ = min_acc_;
            return STOP;
        }
    }

    // check if car is in between lanes (inside an intersection)
    if (curr_intersection_->getGlobalBounds().contains(position_)) {
        // TODO: fix turning left
        if (angular_vel_ == 0) {
            float distanceSourceTarget = Settings::CalculateDistance(source_lane_->GetEndPosition(),
                                                                     dest_lane_->GetStartPosition());

            float angle = (source_lane_->GetDirection() - dest_lane_->GetDirection());

            if (angle > 180) angle -= 360;

            float turningRadius = (distanceSourceTarget / 2.f) / (sin(angle * M_PI / 360.f));

            float turningParameter = 2.f * M_PI * turningRadius;

            float turningDistance = (angle / 360.f) * turningParameter;

            angular_vel_ = -angle / turningDistance;
        }

        if (source_lane_ != nullptr) {
            prev_intersection_ = curr_map_->GetIntersection(source_lane_->GetIntersectionNumber());
            prev_intersection_->AddVehicleCount();
            source_lane_->RemoveVehicleCount();
            source_lane_ = nullptr;
        }

        state_ = TURN;

        //set rotation
        acc_ = (Settings::AccWhileTurning) ? max_acc_ / 2.f : 0;
        return TURN;
    }

    // check distance from stop (if lane is blocked)
    if (source_lane_ != nullptr && source_lane_ != dest_lane_ && source_lane_->GetIsBlocked() &&
        !sprite_.getGlobalBounds().contains(source_lane_->GetEndPosition())) {
        float distanceFromStop = Settings::CalculateDistance(this->position_, source_lane_->GetEndPosition());
        float brakingDistance = -(speed_ * speed_) / (2 * min_acc_);

        if (distanceFromStop < brakingDistance + Settings::MinDistanceFromStop) {
            state_ = STOP;
            acc_ = min_acc_;
            return STOP;
        }
    }

    // check if car has left intersection and is now in targetLane
    if (dest_lane_ != nullptr && dest_lane_->getGlobalBounds().contains(position_)) {
        // remove count from previous lane, and set in to nullptr
        prev_intersection_->RemoveVehicleCount();
        prev_intersection_ = nullptr;

        // we need to transfer vehicle to target lane
        TransferVehicle(this, dest_lane_, source_lane_);

        acc_ = max_acc_;
        state_ = DRIVE;
        return DRIVE;
    }

    // check if car is no longer in intersection
    if (dest_lane_ == nullptr && !source_lane_->getGlobalBounds().contains(position_)) {
        source_lane_->RemoveVehicleCount();

        to_be_deleted_++;
        state_ = DELETE;
        return DELETE;
    }

    // default = just drive
    acc_ = max_acc_;
    state_ = DRIVE;
    return DRIVE;

}

/// update a vehicle's location
void Vehicle::Update(float elapsedTime)
{

    data_box_->Update(position_);
    data_box_->SetData("Speed", Settings::ConvertVelocity(PXS, KMH, speed_));
    drive();
    apply_changes(elapsedTime);
}

/// apply the calculated next position
void Vehicle::apply_changes(float elapsed_time)
{
    // apply acceleration
    speed_ += acc_ * elapsed_time * Settings::Speed;

    // apply max Speed limit
    if (speed_ > max_speed_) speed_ = max_speed_;

    // apply min Speed limit
    if (speed_ < 0) speed_ = 0;

    // set rotation relative to currentSpeed, to create a constant turning radius
    Transform t;
    rotation_ += angular_vel_ * elapsed_time * speed_ * Settings::Speed;

    t.rotate(rotation_);

    // rotate the movement vector in relation to the forward vector (0,1)
    movement_vec_ = t.transformPoint(m_forwardVec);

    // apply movement vector on position, relative to elapsed time to ensure
    // a constant Speed at any FPS

    position_ += movement_vec_ * speed_ * elapsed_time * Settings::Speed;

    // apply rotation and position changes to the actual car sprite
    sprite_.setPosition(position_);
    sprite_.setRotation(rotation_);

}

/// render the vehicle
void Vehicle::Draw(RenderWindow *window)
{
    (*window).draw(this->sprite_);
    if (Settings::DrawVehicleDataBoxes) data_box_->Draw(window);
}



