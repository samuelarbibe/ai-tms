//
//  Lane.cpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 03/12/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#include "Lane.hpp"

int Lane::LaneCount = 0;

Lane::Lane(int laneNumber,
           int roadNumber,
           int intersectionNumber,
           Vector2f startPosition,
           float length,
           float direction,
           bool isInRoadDirection) {
	is_in_road_direction_ = isInRoadDirection;
	lane_number_ = laneNumber;
	road_number_ = roadNumber;
	intersection_number_ = intersectionNumber;
	start_pos_ = startPosition;
	width_ = Settings::LaneWidth;
	length_ = length;
	direction_ = fmod(direction, 360.f);
	is_blocked_ = false;
	total_vehicle_count_ = 0;
	phase_number_ = 0;
	density_ = 0;
	selected_ = false;
	queue_length_ = 0;
	traversal_time_ = 0;

	// calculate end position:
	Vector2f lengthVec;

	Transform t;
	t.rotate(direction);

	lengthVec = t.transformPoint(Vector2f(0.f, -1.f)) * length;

	end_pos_ = lengthVec + start_pos_;

	// init rectangle shape
	this->setOrigin(width_ / 2.f, 0.f);
	this->setPosition(start_pos_);
	this->setRotation(direction_ + 180);
	this->setSize(Vector2f(width_, length_));
	if (Settings::LaneDensityColorRamping)
	{
		ColorRamp();
	} else
	{
		this->setFillColor(LaneColor);
	}

	// create direction arrow shape
	create_arrow_shape(t);

	// create lane block rectangle shape
	create_block_shape();

    data_box_ = new DataBox(end_pos_);
    data_box_->AddData("ID", lane_number_);
    data_box_->AddData("TT", 0);
    data_box_->AddData("Qlen", 0);
}

Lane::~Lane() {
	if (Settings::DrawDelete)
		cout << "Lane " << lane_number_ << " deleted" << endl;
}

/// create the arrow shapes indicating the lane's direction
void Lane::create_arrow_shape(Transform t) {
	arrow_shape_.setPointCount(7);

	t.scale(width_ / 4, width_ / 4);
	arrow_shape_.setPoint(0, end_pos_ - t.transformPoint(Vector2f(0.f, -2.f)));

	t.rotate(-45);
	arrow_shape_.setPoint(1, arrow_shape_.getPoint(0) - t.transformPoint(Vector2f(0.f, -1.f)));

	t.rotate(90);
	arrow_shape_.setPoint(6, arrow_shape_.getPoint(0) - t.transformPoint(Vector2f(0.f, -1.f)));

	t.rotate(45);
	arrow_shape_.setPoint(2, arrow_shape_.getPoint(1) - t.transformPoint(Vector2f(0.f, -1.0f)));

	t.rotate(180);
	arrow_shape_.setPoint(5, arrow_shape_.getPoint(6) - t.transformPoint(Vector2f(0.f, -1.f)));

	t.rotate(90);
	arrow_shape_.setPoint(3, arrow_shape_.getPoint(2) - t.transformPoint(Vector2f(0.f, -1.f)));

	arrow_shape_.setPoint(4, arrow_shape_.getPoint(5) - t.transformPoint(Vector2f(0.f, -1.f)));

	arrow_shape_.setFillColor(WhiteColor);
}

/// create the block shape shown in a blocked lane
void Lane::create_block_shape() {
	lane_block_shape_ = RectangleShape();

	float block_height = 15.f;

	lane_block_shape_.setOrigin(Vector2f(this->width_ / 2, block_height));
	lane_block_shape_.setPosition(this->end_pos_);
	lane_block_shape_.setSize(Vector2f(this->width_, block_height));
	lane_block_shape_.rotate(this->getRotation());
	lane_block_shape_.setFillColor(Color::White);
}

/// update, for future use
void Lane::Update(float elapsedTime) {

    density_ = vehicles_in_lane_.size() / Settings::ConvertSize(PX, M, length_);

    if(queue_length_ > 0)
    {
        traversal_time_ = calculate_traversal_time();
    }

    if (Settings::DrawRoadDataBoxes)
    {
        data_box_->SetData("Qlen", queue_length_);
        data_box_->SetData("TT", traversal_time_);
    }

    // disable lane coloring if needed
    if(selected_)
	{
        this->setFillColor(Color::Red);
    }
	else if(Settings::LaneDensityColorRamping)
	{
        this->ColorRamp();
    }
	else
	{
        this->setFillColor(LaneColor);
    }
}

/// calculate the traversal time
float Lane::calculate_traversal_time()
{

/*
    // first, the time it gets to get to top speed:
    // a = Settings::Acceleration
    // Vmax = Settings::MaxSpeed[0] (used as average)
    //
    // Vmax = a * t -> t = Vmax / a
    float Vmax = Settings::ConvertVelocity(PXS, MS, Settings::MaxSpeeds[0]);
    float a = Settings::ConvertVelocity(PXS, MS,Settings::Acceleration[0]);
    float x = Settings::ConvertSize(PX, M, queue_length_);

    float t1 = Vmax / a ;
    // now that we reached max speed, we need to calculate the distance that has been traveled.
    // while getting to the max speed
    // x = 0.5 * a * t^2
    float x1 = 0.5f * a * t1 * t1;

    // if the distance to reach max speed is bigger then queue length,
    // set the traversal time to the time it takes to intersection
    // while ignoring max speed
    if(x1 > x)
    {
        t1 = sqrt(2 * x / a);
    }
        // else, combine the time to reach max speed with the time
        // it takes to reach the intersection from the max speed point
    else
    {
        // the distance left from the intersection
        x1 = x - x1;
        t1 += x1 / Vmax;
    }

    return t1;
    */

    return Settings::ConvertSize(PX, M, queue_length_)/2;
}

/// create a color visualisation of lane density
void Lane::ColorRamp() {

	// normalizing the lane density.
	// density is vehicle-per-meter

	float value = (density_ / Settings::MaxDensity);
 	if (value > 1.f) value = 1.f;
	float r, g, b;

	Settings::GetHeatMapColor(value, &r, &g, &b);

	this->setFillColor(Color(r, g, b, 255));
}

/// try to set the current queue length in this lane
void Lane::SetQueueLength(float queueLength)
{
    if(queueLength > queue_length_)
    {
        queue_length_ = queueLength;
    }
}

/// set this lane as selected
void Lane::Select() {
	selected_ = true;
}

/// set this lane as unselected
void Lane::Unselect() {
	selected_ = false;
}

/// draw the road
void Lane::Draw(RenderWindow *window) {
	window->draw(*this);
	window->draw(arrow_shape_);

	if (Settings::DrawLaneBlock && is_blocked_)
	{
		window->draw(lane_block_shape_);
	}

    if (Settings::DrawRoadDataBoxes)
        data_box_->Draw(window);
}


