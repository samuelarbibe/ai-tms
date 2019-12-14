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
#include "Intersection.hpp"

#define CAR_IMAGE_DIR "/Users/samuelarbibe/Documents/GitHub/SimulatorSFML/SimulatorSFML/assets/Cars/car_image2.png"

#endif /* Vehicle_hpp */

using namespace std;
using namespace sf;

typedef enum State {STOP, DRIVE, TURN} State;

class Vehicle;

static int vehicleCount = 0;

static Texture carTexture;

class Vehicle
{
    
public:

    static Vehicle * AddVehicle(vector<Vehicle*> &activeVehicles, int carNumber, float maxSpeed, Lane * sourceLane, Lane * destinationLane, Intersection * currentIntersection);
    static void RemoveVehicle(int vehicleNumber, vector<Vehicle*> &activeVehicles);
    static Vehicle * GetVehicle(int vehicleNumber, vector<Vehicle*> &activeVehicles);

    Vehicle();
    Vehicle(vector<Vehicle*> &activeVehicles, int carNumber, float maxSpeed, Lane * sourceLane, Lane * destinationLane, Intersection * currentIntersection);
    ~Vehicle(){cout << "Vehicle " << m_veheicleNumber << " deleted" << endl;};
    
    void Draw(RenderWindow * window);
    void Update(float elapsedTime);
    
    //static Vehicle * GetVehicle(int vehicleNumber);

    Lane * GetSourceLane(){return this->m_sourceLane;};
    Lane * GetTargetLane(){return this->m_targetLane;};
    int GetVehicleNumber(){return this->m_veheicleNumber;};
    
private:
    
    Vector2f m_position;
    Vector2f m_movementVec;
    Sprite   m_sprite;
    //Texture  m_texture;
        
    float    m_speed;
    float    m_maxSpeed;
    float    m_rotation;
    float    m_angularV;
    
    Vehicle *m_vehicleInFront;
    vector<Vehicle*> *m_activeVehiclesPtr;
    
    int      m_veheicleNumber;
    Lane *   m_sourceLane;
    Lane *   m_targetLane;
    Lane *   m_currentLane;
    Intersection * m_currentIntersection;
        
    State    m_state;
    
    void calculateState();

};
