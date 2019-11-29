//
//  MovableObject.hpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 22/11/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//
#pragma once

#ifndef MovableObject_hpp
#define MovableObject_hpp

#include <stdio.h>
#include <iostream>
#include <math.h>
#include <SFML/Graphics.hpp>

using namespace sf;

class MovableObject{
private:
    Vector2f m_position;
    Sprite   m_sprite;
    Texture  m_texture;
    float    m_speed;
    float    m_rotation;
    bool     m_brake;
    bool     m_drive;
    
public:
    MovableObject();
    ~MovableObject();
    
    void Init(float initSpeed, float initRotation, int x, int y, const char * textureDirName);
    void Move(float rotationDt, float speedDt);
    void Draw(RenderWindow * window);
    void Update(float elapsedTime);
    
    Sprite GetSprite();    
};

#endif
