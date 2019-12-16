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

typedef enum State {STOP, DRIVE, TURN, DELETE} State;

static Texture carTexture;

class Vehicle
{
    
public:

    static list<Vehicle*> activeVehicles;
    static int vehicleCount;
    static int toBeDeleted;
    
    static void ClearVehicles();
    static Vehicle * GetVehicle(int vehicleNumber);
    static Vehicle * AddVehicle(int vehicleNumber, float maxSpeed, Lane * sourceLane, Lane * destinationLane, Intersection * currentIntersection);
    
    Vehicle();
    Vehicle(int vehicleNumber, float maxSpeed, Lane * sourceLane, Lane * destinationLane, Intersection * currentIntersection);
    ~Vehicle(){cout << "Vehicle " << m_vehicleNumber << " deleted" << endl;};
        
    void TransferVehicle(int vehicleNumber, Lane * fromLane, Lane * toLane);
    
    void Draw(RenderWindow * window);
    void Update(float elapsedTime);
    
    Lane * GetSourceLane(){return this->m_sourceLane;};
    Lane * GetTargetLane(){return this->m_targetLane;};
    State GetState(){return m_state;};
    int GetVehicleNumber(){return this->m_vehicleNumber;};
    
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
    
    int      m_vehicleNumber;
    
    Lane *   m_sourceLane;
    Lane *   m_targetLane;
    Lane *   m_currentLane;
    Intersection * m_currentIntersection;
        
    State    m_state;
    
    State drive();
    void  applyChanges(float elapsedTime);
    float calculateDistance(Vector2f a, Vector2f b);

};

