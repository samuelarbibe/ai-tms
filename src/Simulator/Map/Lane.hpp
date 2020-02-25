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
#include <math.h>

#include <SFML/Graphics.hpp>

#include "../Settings.hpp"

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

	bool GetIsBlocked() { return is_blocked_; };
	bool GetIsInRoadDirection() { return is_in_road_direction_; };
	float GetDirection() { return direction_; };
	int GetLaneNumber() { return lane_number_; };
	int GetIntersectionNumber() { return intersection_number_; };
	int GetCurrentVehicleCount() { return current_vehicle_count_; };
	int GetTotalVehicleCount() { return total_vehicle_count_; };
	int GetLastCar() { return last_entered_car_number_; };
	int GetRoadNumber() { return road_number_; };
	int GetPhaseNumber() { return phase_number_; };
	Vector2f GetStartPosition() { return start_pos_; };
	Vector2f GetEndPosition() { return end_pos_; };

	void Select();
	void Unselect();
	void AddVehicleCount() {
		current_vehicle_count_++;
		total_vehicle_count_++;
	};
	void RemoveVehicleCount() { current_vehicle_count_--; };
	void SetIsBlocked(bool blocked) { is_blocked_ = blocked; };
	void SetLastCar(int lastCar) { last_entered_car_number_ = lastCar; };
	void SetPhaseNumber(int phaseNumber) { phase_number_ = phaseNumber; }

	void Draw(RenderWindow *window);

	static int LaneCount;

  private:

	bool is_blocked_;
	bool is_in_road_direction_;
	int intersection_number_;
	int road_number_;
	int lane_number_;
	int current_vehicle_count_;
	int total_vehicle_count_;
	int last_entered_car_number_;
	int phase_number_;

	Vector2f start_pos_;
	Vector2f end_pos_;

	float direction_;
	float width_;
	float length_;

	void create_arrow_shape(Transform t);
	ConvexShape arrow_shape_;

	void create_block_shape();
	RectangleShape lane_block_shape_;
};

#endif /* Lane_hpp */
