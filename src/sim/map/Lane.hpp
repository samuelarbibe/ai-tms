//
//  Lane.hpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 03/12/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#ifndef Lane_hpp
#define Lane_hpp

#include <stdio.h>
#include <iostream>
#include <list>
#include <math.h>

#include <SFML/Graphics.hpp>
#include <src/sim/simulator/DataBox.hpp>

#include "src/sim/simulator/Settings.hpp"

using namespace std;
using namespace sf;

const Color LaneColor(45, 45, 45);
const Color WhiteColor(230, 230, 230);
const Color BackgroundColor(150, 150, 150);

class Lane : public RectangleShape
{

  public:

	Lane(int laneNumber,
	     int roadNumber,
	     int intersectionNumber,
	     Vector2f startPosition,
	     float length,
	     float direction,
	     bool isInRoadDirection);
	~Lane();

	void Update(float elapsedTime);
	void Draw(RenderWindow *window);

	// get
	int GetLaneNumber() { return lane_number_; };
	int GetIntersectionNumber() { return intersection_number_; };
	int GetTotalVehicleCount() { return total_vehicle_count_; };
	int GetRoadNumber() { return road_number_; };
	int GetPhaseNumber() { return phase_number_; };
	bool GetIsBlocked() { return is_blocked_; };
	bool GetIsInRoadDirection() { return is_in_road_direction_; };
	float GetDirection() { return direction_; };
	int GetFrontVehicleId() {
		if (!vehicles_in_lane_.empty())
			return vehicles_in_lane_.front();
		return 0;
	}
	int GetBackVehicleId() {
		if (!vehicles_in_lane_.empty())
			return vehicles_in_lane_.back();
		return 0;
	}
	int GetCurrentVehicleCount() { return vehicles_in_lane_.size(); }
    float GetQueueLength() { return queue_length_ ;}
    float GetNormalizedQueueLength() { return queue_length_ / length_;}
    float GetDensity() { return density_;}
    float GetTraversalTime() { return traversal_time_;}
    float GetNormalizedDensity() { return density_ / Settings::MaxDensity;}

	Vector2f GetStartPosition() { return start_pos_; };

	Vector2f GetEndPosition() { return end_pos_; };
	// set
	void Select();
	void Unselect();
	void PushVehicleInLane(int vehicleId) {
		vehicles_in_lane_.push_back(vehicleId);
		total_vehicle_count_++;
	}
	void PopVehicleFromLane() {
		vehicles_in_lane_.pop_front();
	}
	void SetIsBlocked(bool blocked) { is_blocked_ = blocked; if(!blocked) queue_length_ = 0;}
	void SetPhaseNumber(int phaseNumber) { phase_number_ = phaseNumber; }
	void ColorRamp();
	void ClearLane()
	{
		total_vehicle_count_ = 0;
		density_ = 0;
		Unselect();
		vehicles_in_lane_.clear();
	}
	void SetQueueLength(float distance);

	// The count of the overall Lanes that have been created
	static int LaneCount;

  private:

    float calculate_traversal_time();
	// Is this intersection block
	bool is_blocked_;
	// Is this lane the same direction of the parent road
	bool is_in_road_direction_;
	// ID of the father intersection
	int intersection_number_;
	// ID of the father road
	int road_number_;
	// ID if this lane
	int lane_number_;
	// Total count of vehicles that passed in this lane
	int total_vehicle_count_;
	// The ID of the phase this lane belongs to
	int phase_number_;
	// Is this lane currently selected
	bool selected_;
	// Density of the lane.
	// Measured by Car-per-Meter of lane
	float density_;
    // The length of the queue in this lane;
    // calculated in Vehicle class,
    // as the distance between the
    // first and the last car in lane with a state of STOP;
    float queue_length_;
    // the time it will take for the last vehicle int
    // the queue to reach the intersection
    float traversal_time_;

	list<int> vehicles_in_lane_;

	Vector2f start_pos_;
	Vector2f end_pos_;

	float direction_;
	float width_;
	float length_;

	void create_arrow_shape(Transform t);
	ConvexShape arrow_shape_;

	void create_block_shape();
	RectangleShape lane_block_shape_;

    DataBox * data_box_;
};

#endif /* Lane_hpp */
