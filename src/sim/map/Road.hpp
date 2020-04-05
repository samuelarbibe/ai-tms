//
//  Road.hpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 03/12/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#ifndef Road_hpp
#define Road_hpp

#include <iostream>
#include <list>

#include <SFML/Graphics.hpp>

#include "Lane.hpp"
#include "src/sim/simulator/Settings.hpp"
#include "src/sim/simulator/DataBox.hpp"


using namespace std;

typedef vector<VertexArray> LaneLine;

class Road : public RectangleShape
{
    
public:
    
    Road(int roadNumber, int intersectionNumber, int connectionSide, Vector2f startPosition, float length, float direction);
    Road(int roadNumber, int intersectionNumber1, int intersectionNumber2, int connectionSide1, int connectionSide2, Vector2f conPosition1, Vector2f conPosition2, float direction);
    ~Road();

    void   Draw(RenderWindow * window);
    void   Update(float elapsedTime);
    void   ReloadRoadDimensions();
    void   BuildLaneLines();

    // Add entities
    Lane * AddLane(int laneNumber, bool isInRoadDirection);

    // get
    Lane * GetLane(int laneNumber);
    float  GetWidth(){return width_;}
    int    GetRoadNumber(){return road_number_;}
    int    GetIntersectionNumber(int index = 0){return intersection_number_[index];}
    int    GetConnectionSide(int index = 0){return connection_side_[index];}
    bool   GetIsConnecting(){return is_connecting_;}
    int    GetLaneCount(){return lanes_.size();};
    float  GetRoadDirection(){return direction_;}
    Vector2f GetStartPosition(){return start_pos_;}
    Vector2f GetEndPosition(){return end_pos_;}
    vector<Lane*> *  GetLanes(){return &(lanes_);};

    // set
    void   ReAssignLanePositions();
    void   UpdateStartPosition(Vector2f position);
    void   UpdateEndPosition(Vector2f position);
    bool   DeleteLane(int laneNumber);
    
    Lane * CheckSelection(Vector2f position);

	// The total count of all the roads created this session
    static int RoadCount;
    
private:

	// ID of this road
    int        road_number_;
	// The number of lanes that belong to this road
    int        number_of_lanes_;
    // ID of intersection / intersections this road connects to
    // if [1] is empty, this is a non-connecting road
    int        intersection_number_[2];
    // The connection side of this road to intersection_number_[0] and
    // intersection_number_[1], respectively
    int        connection_side_[2];
    // The current count of the active vehicles in this road
    int        current_vehicle_count_;
    // The count of all the vehicles that passed through this road
    int        total_vehicle_count_;
	// Is this road connecting
    bool       is_connecting_;

    Vector2f   start_pos_;
    Vector2f   end_pos_;
    
    float      direction_;
    float      length_;
    float      width_;
    
    vector<Lane*> lanes_;

    vector<LaneLine> lane_lines_;

    DataBox * data_box_;
};



#endif /* Road_hpp */
