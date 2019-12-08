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
#include <SFML/Graphics.hpp>
#include <math.h>
//#include "Vehicle.hpp"

using namespace std;
using namespace sf;

const Color LaneColor(45, 45, 45);
const Color WhiteColor(230, 230, 230);
const Color BackgroundColor(150, 150, 150);

static int laneCount = 0;
static int carCount = 0;

typedef enum State {STOP, DRIVE, TURN} State;

class Vehicle;

class Lane : public RectangleShape
{
    
public:
    
    Lane();
    Lane(int laneNumber, int roadNumber, Vector2f startPosition, float width, float length, float direction);
    ~Lane();
    
    void Update(float elapsedTime);
    bool  GetIsBlocked() {return m_isBlocked;};
    float GetDirection() {return m_direction;};
    int   GetLaneNumber(){return m_laneNumber;};
    Vector2f GetStartLocaiton(){return m_startPosition;};
    
    Vehicle * AddVehicle(Lane * destinationLane);
    
    void  Draw(RenderWindow * window);
    
private:
    
    bool     m_isBlocked;
    int      m_roadNumber;
    int      m_laneNumber;
    int      m_vehicleCount;
    
    Vector2f m_startPosition;
    Vector2f m_endPosition;
    
    float    m_direction;
    float    m_width;
    float    m_length;
    
    void     m_createArrowShape(Transform t);
    
    vector<Vehicle*> m_vehicles;
    
    ConvexShape m_arrowShape;
    
};

class Vehicle
{
    
public:
    
    Vehicle();
    Vehicle(int carNumber, float maxSpeed, Lane * sourceLane, Lane * destinationLane, Vehicle * vehicleInFront);
    ~Vehicle();
    
    void Move(float rotationDt, float speedDt);
    void Draw(RenderWindow * window);
    void Update(float elapsedTime);
    
    Lane * GetSourceLane(){return this->m_sourceLane;};
    Lane * GetTargetLane(){return this->m_targetLane;};
    int GetVehicleNumber(){return this->m_veheicleNumber;};
    
private:
    
    Vector2f m_position;
    Vector2f m_movementVec;
    Sprite   m_sprite;
    Texture  m_texture;
        
    float    m_speed;
    float    m_maxSpeed;
    float    m_rotation;
    
    Vehicle *m_vehicleInFront;
    
    int      m_veheicleNumber;
    Lane  *  m_sourceLane;
    Lane  *  m_targetLane;
    Lane  *  m_currentLane;
        
    State    m_state;

};

#endif /* Lane_hpp */
