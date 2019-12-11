//
//  Vehicle.hpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 11/12/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#ifndef Vehicle_hpp
#define Vehicle_hpp

#include <stdio.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Lane.hpp"

#endif /* Vehicle_hpp */

using namespace std;
using namespace sf;

typedef enum State {STOP, DRIVE, TURN} State;

class Vehicle;

static int vehicleCount = 0;

class Vehicle
{
    
public:

    static Vehicle * AddVehicle(vector<Vehicle*> &activeVehicles, int carNumber, float maxSpeed, Lane * sourceLane, Lane * destinationLane);
    
    Vehicle();
    Vehicle(vector<Vehicle*> &activeVehicles, int carNumber, float maxSpeed, Lane * sourceLane, Lane * destinationLane);
    ~Vehicle(){cout << "Vehicle " << m_veheicleNumber << " deleted" << endl;};
    
    void Move(float rotationDt, float speedDt);
    void Draw(RenderWindow * window);
    void Update(float elapsedTime);
    
    Vehicle * GetVehicle(vector<Vehicle*> &activeVehicles, int vehicleNumber);

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
    Lane *   m_sourceLane;
    Lane *   m_targetLane;
    Lane *   m_currentLane;
        
    State    m_state;
    
    void calculateState();

};
