//
//  Road.cpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 03/12/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#include "Road.hpp"

int Road::RoadCount = 0;

/// ctor for a normal road
Road::Road(int roadNumber, int intersectionNumber, int connectionSide, Vector2f startPosition, float length,
           float direction)
{
    is_connecting_ = false;
    road_number_ = roadNumber;
    intersection_number_[0] = intersectionNumber;
    intersection_number_[1] = intersectionNumber;
    connection_side_[0] = connectionSide;
    start_pos_ = startPosition;
    length_ = length;
    direction_ = direction;
    number_of_lanes_ = 0;
    width_ = 0;
    current_vehicle_count_ = 0;
    total_vehicle_count_ = 0;

    // calculate end position:
    Vector2f lengthVec;

    Transform t;
    t.rotate(direction + 180);

    lengthVec = t.transformPoint(Vector2f(0.f, -1.f)) * length;
    end_pos_ = start_pos_ - lengthVec;

    // init rectangle shape
    this->setOrigin(width_ / 2, 0.f);
    this->setPosition(start_pos_);
    this->setOutlineColor(WhiteColor);
    this->setOutlineThickness(1.f);
    this->setFillColor(Color::Transparent);
    this->setRotation(direction_ + 180);
    this->setSize(Vector2f(width_, length_));

    data_box_ = new DataBox(end_pos_);
    data_box_->AddData("ID", road_number_);
    data_box_->AddData("Count", 0);
}

/// ctor for a connecting road
Road::Road(int roadNumber, int intersectionNumber1, int intersectionNumber2, int connectionSide1, int connectionSide2,
           Vector2f conPosition1, Vector2f conPosition2, float direction)
{
    is_connecting_ = true;
    road_number_ = roadNumber;
    intersection_number_[0] = intersectionNumber1;
    intersection_number_[1] = intersectionNumber2;
    connection_side_[0] = connectionSide1;
    connection_side_[1] = connectionSide2;
    start_pos_ = conPosition1;
    end_pos_ = conPosition2;
    direction_ = direction;
    number_of_lanes_ = 0;
    width_ = 0;
    length_ = Settings::CalculateDistance(start_pos_, end_pos_);

    // calculate end position:

    // init rectangle shape
    this->setOrigin(width_ / 2, 0.f);
    this->setPosition(start_pos_);
    this->setOutlineColor(WhiteColor);
    this->setOutlineThickness(1.f);
    this->setFillColor(Color::Transparent);
    this->setRotation(direction_ + 180);
    this->setSize(Vector2f(width_, length_));

    data_box_ = new DataBox(end_pos_);
    data_box_->AddData("ID", road_number_);
    data_box_->AddData("Count", 0);
}

Road::~Road()
{
    for (Lane *lane : lanes_)
    {
        delete lane;
    }
    if (Settings::DrawDelete)cout << "Road " << road_number_ << " deleted" << endl;

    delete data_box_;
}

/// add a lane to a road
Lane *Road::AddLane(int laneNumber, bool isInRoadDirection)
{
    if (!laneNumber)
    {
        laneNumber = Lane::LaneCount + 1;
    }

    if (isInRoadDirection)
    {
        lanes_.push_back(
                new Lane(laneNumber, road_number_, intersection_number_[isInRoadDirection], start_pos_, length_,
                         direction_, isInRoadDirection));
    } else
    {
        lanes_.push_back(new Lane(laneNumber, road_number_, intersection_number_[isInRoadDirection], end_pos_, length_,
                                  (direction_ + 180.f), isInRoadDirection));
    }

    number_of_lanes_++;
    Lane::LaneCount++;

    // adjust road size
    width_ = number_of_lanes_ * Settings::LaneWidth;
    this->setSize(Vector2f(width_, length_));
    this->setOrigin(width_ / 2, 0.f);

    if (Settings::DrawAdded)
        std::cout << "lane " << lanes_[number_of_lanes_ - 1]->GetLaneNumber() << " added to road " << road_number_
                  << std::endl;

    return lanes_[number_of_lanes_ - 1];
}

/// get lane by laneNumber
Lane *Road::GetLane(int laneNumber)
{
    for (int i = 0; i < number_of_lanes_; i++)
    {
        if (lanes_[i]->GetLaneNumber() == laneNumber)
        {
            return lanes_[i];
        }
    }

    return nullptr;
}

/// re-locate all lanes in road to align with the road
void Road::ReAssignLanePositions()
{
    Vector2f firstLanePoint;
    Vector2f firstLaneDifference;
    Vector2f laneDifference;
    Vector2f lengthVec;

    // temporarily deactivate Deletion drawing
    bool prevState = Settings::DrawDelete;
    Settings::DrawDelete = false;

    this->width_ = Settings::LaneWidth * number_of_lanes_;
    this->setSize(Vector2f(width_, length_));

    Transform t, x;

    t.rotate(direction_ + 90);
    laneDifference = t.transformPoint(0.f, -1.f) * Settings::LaneWidth;

    (number_of_lanes_ % 2) ?
    x.scale(number_of_lanes_ / 2, number_of_lanes_ / 2) :
    x.scale((number_of_lanes_ - 1) / 2 + 0.5, (number_of_lanes_ - 1) / 2 + 0.5);

    firstLaneDifference = x.transformPoint(laneDifference);

    firstLanePoint = start_pos_ - firstLaneDifference;

    for (int i = 0; i < number_of_lanes_; i++)
    {

        Transform z, y;

        z.scale(i, i);

        int tempLaneNumber = lanes_[i]->GetLaneNumber();
        float tempLaneDirection = lanes_[i]->GetDirection();

        //delete lanes_[i];

        //if lane is in road direction
        if (tempLaneDirection == direction_)
        {
            // send calculated starting point
            *lanes_[i] = Lane(tempLaneNumber, road_number_, intersection_number_[1],
                                 firstLanePoint + z.transformPoint(laneDifference), length_, direction_, true);
        }
        else
        {
            // send starting point + length vector
            y.rotate(direction_);
            lengthVec = y.transformPoint(Vector2f(0.f, -1.f)) * length_;


            *lanes_[i] = Lane(tempLaneNumber, road_number_, intersection_number_[0],
                                 firstLanePoint + z.transformPoint(laneDifference) + lengthVec,
                                 length_, (direction_ + 180.f), false);
        }
    }

    Settings::DrawDelete = prevState;
}

/// update the road's start position
void Road::UpdateStartPosition(Vector2f position)
{
    start_pos_ = position;
    length_ = Settings::CalculateDistance(end_pos_, start_pos_);

    this->setOrigin(width_ / 2, 0.f);
    this->setPosition(start_pos_);
    this->setSize(Vector2f(width_, length_));

    ReAssignLanePositions();
}

/// update the road's end position
void Road::UpdateEndPosition(Vector2f position)
{
    end_pos_ = position;
    length_ = Settings::CalculateDistance(end_pos_, start_pos_);
    this->setSize(Vector2f(width_, length_));

    ReAssignLanePositions();
}

/// check if a road has been selected
Lane *Road::CheckSelection(Vector2f position)
{
    // for each intersection in map
    for (Lane *lane : lanes_)
    {
        // if selection found
        if (lane->getGlobalBounds().contains(position))
        {
            return lane;
        }
    }
    return nullptr;
}

/// update, for databoxes ect.
void Road::Update(float elapsedTime)
{
    if (Settings::DrawRoadDataBoxes)
    {
        current_vehicle_count_ = 0;
        total_vehicle_count_ = 0;
        for (Lane *l : lanes_)
        {
            l->Update(elapsedTime);
            current_vehicle_count_ += l->GetCurrentVehicleCount();
            total_vehicle_count_ += l->GetTotalVehicleCount();
        }

        data_box_->SetData("Count", current_vehicle_count_);
    }
}

/// delete a given lane in this road
bool Road::DeleteLane(int laneNumber)
{
    Lane *targetLane = this->GetLane(laneNumber);

    // if lane was found
    if (targetLane != nullptr)
    {
        // remove the targetLane from the list by iterator
        auto it = find(lanes_.begin(), lanes_.end(), targetLane);
        lanes_.erase(it);
        number_of_lanes_--;
        return true;
    }
    return false;
}

/// draw the road and al of its lanes
void Road::Draw(RenderWindow *window)
{
    window->draw(*this);
    for (int i = 0; i < number_of_lanes_; i++)
    {
        lanes_[i]->Draw(window);
    }
    if (Settings::DrawRoadDataBoxes) data_box_->Draw(window);
}

