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
Road::Road(int roadNumber,
           int intersectionNumber,
           int connectionSide,
           Vector2f startPosition,
           float length,
           float direction) {
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
	this->setFillColor(Color::Transparent);
	this->setRotation(direction_ + 180);
	this->setSize(Vector2f(width_, length_));

	data_box_ = new DataBox(end_pos_);
	data_box_->AddData("ID", road_number_);
	data_box_->AddData("Count", 0);
}

/// ctor for a connecting road
Road::Road(int roadNumber,
           int intersectionNumber1,
           int intersectionNumber2,
           int connectionSide1,
           int connectionSide2,
           Vector2f conPosition1,
           Vector2f conPosition2,
           float direction) {
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
	this->setFillColor(Color::Transparent);
	this->setRotation(direction_ + 180);
	this->setSize(Vector2f(width_, length_));

	data_box_ = new DataBox(end_pos_);
	data_box_->AddData("ID", road_number_);
	data_box_->AddData("Count", 0);
}

Road::~Road() {
	for (Lane *lane : lanes_)
	{
		delete lane;
	}
	if (Settings::DrawDelete)
		cout << "Road " << road_number_ << " deleted" << endl;

	delete data_box_;
}

/// add a lane to a road
Lane *Road::AddLane(int laneNumber, bool isInRoadDirection) {
	if (!laneNumber)
	{
		laneNumber = Lane::LaneCount + 1;
	}

	if (isInRoadDirection)
	{
		lanes_.push_back(
			new Lane(laneNumber,
			         road_number_,
			         intersection_number_[isInRoadDirection],
			         start_pos_,
			         length_,
			         direction_,
			         isInRoadDirection));
	} else
	{
		lanes_.push_back(new Lane(laneNumber,
		                          road_number_,
		                          intersection_number_[isInRoadDirection],
		                          end_pos_,
		                          length_,
		                          (direction_ + 180.f),
		                          isInRoadDirection));
	}

	number_of_lanes_++;
	Lane::LaneCount++;

	// adjust road size
	width_ = number_of_lanes_ * Settings::LaneWidth;
	this->setSize(Vector2f(width_, length_));
	this->setOrigin(width_ / 2, 0.f);

	if (Settings::DrawAdded)
		std::cout << "lane " << lanes_[number_of_lanes_ - 1]->GetLaneNumber()
		          << " added to road " << road_number_
		          << std::endl;

	return lanes_[number_of_lanes_ - 1];
}

/// get lane by laneNumber
Lane *Road::GetLane(int laneNumber) {
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
void Road::ReAssignLanePositions() {

	Vector2f firstLanePoint;
	Vector2f firstLaneDifference;
	Vector2f laneDifference;
	Vector2f lengthVec;

	// temporarily deactivate Deletion drawing
	bool prevState = Settings::DrawDelete;
	Settings::DrawDelete = false;

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

		//if lane is in road direction
		if (tempLaneDirection == direction_)
		{
			// send calculated starting point
			*lanes_[i] = Lane(tempLaneNumber,
			                  road_number_,
			                  intersection_number_[1],
			                  firstLanePoint + z.transformPoint(laneDifference),
			                  Settings::CalculateDistance(start_pos_, end_pos_),
			                  direction_, true);
		} else
		{
			// send starting point + length vector
			y.rotate(direction_);
			lengthVec = y.transformPoint(Vector2f(0.f, -1.f)) * length_;

			*lanes_[i] = Lane(tempLaneNumber,
			                  road_number_,
			                  intersection_number_[0],
			                  firstLanePoint + z.transformPoint(laneDifference)
				                  + lengthVec,
			                  Settings::CalculateDistance(start_pos_, end_pos_),
			                  (direction_ + 180.f), false);
		}
	}

	Settings::DrawDelete = prevState;

	BuildLaneLines();
}

/// update the road's start position
void Road::UpdateStartPosition(Vector2f position) {
	start_pos_ = position;
	length_ = Settings::CalculateDistance(end_pos_, start_pos_);
	this->setOrigin(width_ / 2, 0.f);
	this->setPosition(start_pos_);
	this->setSize(Vector2f(width_, length_));

	ReAssignLanePositions();
}

/// update the road's end position
void Road::UpdateEndPosition(Vector2f position) {
	end_pos_ = position;
	length_ = Settings::CalculateDistance(end_pos_, start_pos_);
	this->setSize(Vector2f(width_, length_));

	ReAssignLanePositions();
}

/// check if a road has been selected
Lane *Road::CheckSelection(Vector2f position) {
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
void Road::Update(float elapsedTime) {

	current_vehicle_count_ = 0;
	total_vehicle_count_ = 0;

	for (Lane *l : lanes_)
	{
		l->Update(elapsedTime);
		current_vehicle_count_ += l->GetCurrentVehicleCount();
		total_vehicle_count_ += l->GetTotalVehicleCount();
	}

	if (Settings::DrawRoadDataBoxes)
	{
		data_box_->SetData("Count", current_vehicle_count_);
	}
}

/// delete a given lane in this road
bool Road::DeleteLane(int laneNumber) {
	Lane *targetLane = this->GetLane(laneNumber);

	// if lane was found
	if (targetLane != nullptr)
	{
		// remove the targetLane from the list by iterator
		auto it = lanes_.begin();
		while (it != lanes_.end())
		{
			if ((*it)->GetLaneNumber() == laneNumber)
			{
				it = lanes_.erase(it);
				delete targetLane;
				number_of_lanes_--;
				return true;
			} else
			{
				it++;
			}
		}
	}
	return false;
}

/// create all the lines rendered on the road
void Road::BuildLaneLines() {

	if (number_of_lanes_ > 0)
	{
		// clear prev vector
		lane_lines_.clear();

		int lineCount = number_of_lanes_ + 1;

		bool firstLaneInDir = lanes_[0]->GetIsInRoadDirection();

		int directionSwitchIndex = 0;

		for (Lane *lane : lanes_)
		{
			if (lane->GetIsInRoadDirection() != firstLaneInDir)
			{
				break;
			}
			directionSwitchIndex++;
		}

		if (directionSwitchIndex == 0 || directionSwitchIndex == lineCount)
		{
			directionSwitchIndex = -1;
		}

		// first and last lines will be thick white lines;
		// the indexed line will be a double line / thick white line
		// everything else will be regular dashed lines

		// pre-made direction vectors for dashed lines:
		Transform t, d, s;
		t.rotate(this->direction_);
		d.scale(Settings::DashLineLength, Settings::DashLineLength);
		s.scale(Settings::DashLineSpace, Settings::DashLineSpace);

		Vector2f dashVec = t.transformPoint(Settings::BaseVec);
		dashVec = d.transformPoint(dashVec);

		Vector2f spacerVec = t.transformPoint(Settings::BaseVec);
		spacerVec = s.transformPoint(spacerVec);


		//pre-made direction vectors for lines starting points:
		Transform h, v;

		h.scale(this->length_, this->length_);
		h.rotate(this->direction_);

		v.scale(Settings::LaneWidth, Settings::LaneWidth);
		v.rotate(this->direction_ - 90);

		Vector2f lengthVec = h.transformPoint(Settings::BaseVec);
		Vector2f laneWidthVec = v.transformPoint(Settings::BaseVec);

		Vector2f upperLeftCorner =
			start_pos_ - laneWidthVec * float(number_of_lanes_) / 2.f;

		Vector2f startPos, endPos;
		LaneLine temp;

		for (int i = 0; i < lineCount; i++)
		{
			temp = LaneLine();
			startPos = upperLeftCorner + laneWidthVec * float(i);
			endPos = startPos + lengthVec;

			// border lines
			if (i == 0 || i == lineCount - 1)
			{
				VertexArray line = VertexArray(Lines, 2);

				line[0].position = startPos;
				line[1].position = endPos;

				temp.push_back(line);
			}
				// separator line
			else if (i == directionSwitchIndex)
			{
				if (Settings::DoubleSeparatorLine)
				{
					VertexArray line1 = VertexArray(Lines, 2);

					line1[0].position = startPos + laneWidthVec * 0.05f;
					line1[0].color = Color::Yellow;
					line1[1].position = endPos + laneWidthVec * 0.05f;
					line1[1].color = Color::Yellow;

					temp.push_back(line1);

					VertexArray line2 = VertexArray(Lines, 2);

					line2[0].position = startPos - laneWidthVec * 0.05f;
					line2[0].color = Color::Yellow;
					line2[1].position = endPos - laneWidthVec * 0.05f;
					line2[1].color = Color::Yellow;

					temp.push_back(line2);
				} else
				{
					VertexArray line = VertexArray(Lines, 2);

					line[0].position = startPos;
					line[0].color = Color::Yellow;
					line[1].position = endPos;
					line[1].color = Color::Yellow;

					temp.push_back(line);
				}
			}
				// dashed lines
			else
			{
				Vector2f tempPos = startPos;

				tempPos += spacerVec;

				while (Settings::CalculateDistance(startPos, tempPos + dashVec)
					< length_)
				{
					VertexArray line = VertexArray(Lines, 2);

					line[0].position = tempPos;
					tempPos += dashVec;
					line[1].position = tempPos;
					tempPos += spacerVec;

					temp.push_back(line);
				}

				if (Settings::CalculateDistance(tempPos, endPos)
					> Settings::CalculateDistance(Vector2f(0, 0), dashVec))
				{
					VertexArray line = VertexArray(Lines, 2);

					line[0].position = tempPos;
					line[1].position = endPos;

					temp.push_back(line);
				}
			}
			lane_lines_.push_back(temp);
		}
	}
}

/// reload the road dimensions
void Road::ReloadRoadDimensions() {
	width_ = Settings::LaneWidth * number_of_lanes_;
	length_ = Settings::CalculateDistance(start_pos_, end_pos_);
}

/// draw the road and al of its lanes
void Road::Draw(RenderWindow *window) {
	for (int i = 0; i < number_of_lanes_; i++)
	{
		lanes_[i]->Draw(window);
	}

	window->draw(*this);

	for (LaneLine laneLine : lane_lines_)
	{
		for (VertexArray va : laneLine)
		{
			window->draw(va);
		}
	}

	if (Settings::DrawRoadDataBoxes)
		data_box_->Draw(window);
}


