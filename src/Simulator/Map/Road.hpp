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
#include "../Settings.hpp"
#include "../../UI/Widgets/DataBox.hpp"


using namespace std;


class Road : public RectangleShape
{
    
public:
    
    Road(int roadNumber, int intersectionNumber, int connectionSide, Vector2f startPosition, float length, float direction);
    Road(int roadNumber, int intersectionNumber1, int intersectionNumber2, int connectionSide1, int connectionSide2, Vector2f conPosition1, Vector2f conPosition2, float direction);
    ~Road();

    Lane * AddLane(int laneNumber, bool isInRoadDirection);

    Lane * GetLane(int laneNumber);
    float  GetWidth(){return width_;}
    int    GetRoadNumber(){return road_number_;}
    int    GetIntersectionNumber(int index = 0){return intersection_number_[index];}
    int    GetConnectionSide(int index = 0){return connection_side_[index];}
    bool   GetIsConnecting(){return is_connecting_;}
    int    GetLaneCount(){return lanes_.size();};
    vector<Lane*> *  GetLanes(){return &(lanes_);};
    
    void   ReAssignLanePositions();
    void   UpdateStartPosition(Vector2f position);
    void   UpdateEndPosition(Vector2f position);
    bool   DeleteLane(int laneNumber);

    void   Update(float elapsedTime);
    void   Draw(RenderWindow * window);

    Lane * CheckSelection(Vector2f position);

    static int RoadCount;
    
private:

    bool       is_connecting_;
    int        road_number_;
    int        number_of_lanes_;
    int        intersection_number_[2];
    int        connection_side_[2];
    int        current_vehicle_count_;
    int        total_vehicle_count_;

    Vector2f   start_pos_;
    Vector2f   end_pos_;
    
    float      direction_;
    float      length_;
    float      width_;
    
    vector<Lane*> lanes_;

    DataBox * data_box_;
};



#endif /* Road_hpp */
