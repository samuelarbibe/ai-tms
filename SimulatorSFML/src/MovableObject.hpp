#pragma once
//
//  MovableObject.hpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 22/11/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#ifndef MovableObject_hpp
#define MovableObject_hpp

#include <stdio.h>
#include <iostream>
#include <math.h>
#include <SFML/Graphics.hpp>
//#include "Intersection.hpp"

using namespace sf;

class MovableObject{
    
public:
    
    MovableObject(float initSpeed, float initRotation, Vector2f initPosition, const char * textureDirName, int sourceLane, int targetLane);
    ~MovableObject();
    
    void Move(float rotationDt, float speedDt);
    void Draw(RenderWindow * window);
    void Update(float elapsedTime);
    
    int GetSourceLane(){return this->m_sourceLane;};
    int GetTargetLane(){return this->m_targetLane;};
    
private:
    
    Vector2f m_position;
    Vector2f m_movementVec;
    Sprite   m_sprite;
    Texture  m_texture;
    
    float    m_speed;
    float    m_maxSpeed;
    float    m_rotation;
    
    int      m_sourceLane;
    int      m_targetLane;
    
    bool     m_brake;
    bool     m_drive;
        

};

#endif
