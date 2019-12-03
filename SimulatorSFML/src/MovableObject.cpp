//
//  MovableObject.cpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 22/11/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#include "MovableObject.hpp"

const Vector2f m_forwardVec(1.f, 0.f);

MovableObject::MovableObject(){};

MovableObject::~MovableObject(){}

void MovableObject::Init(float initSpeed, float initRotation, Vector2f initPosition, const char * textureDirName, int sourceLane, int targetLane){
    
    // set initial values for the movable object
    m_speed    = initSpeed;
    m_rotation = initRotation;
    m_position = initPosition;
    m_movementVec = m_forwardVec;
    m_maxSpeed = 300;
    m_sourceLane = sourceLane;
    m_targetLane = targetLane;
    
    // load texture and set up object sprite
    m_texture.loadFromFile(textureDirName);
    m_sprite.setTexture(m_texture);
    
    m_sprite.setScale(0.15f, 0.15f);
    m_sprite.setOrigin(m_sprite.getTextureRect().width/3, m_sprite.getTextureRect().height/2);
}

void MovableObject::Move(float rotationDt, float speedDt){
    
    // update speed
    m_speed += speedDt;
    
    // apply max speed limit
    if(m_speed > m_maxSpeed) m_speed = m_maxSpeed;
    
    // apply min speed limit
    if(m_speed < 0) m_speed = 0;
    
    // set rotation relative to current speed, to create a constant turning radius
    m_rotation += rotationDt * m_speed/1000;
    
    Transform t;
    
    t.rotate(m_rotation);
    
    // rotate the movement vector in relation to the forward vector (0,1)
    m_movementVec = t.transformPoint(m_forwardVec);
    
}

void MovableObject::Update(float elapsedTime){
    
    // apply movement vector on position, relative to elapsed time to ensure
    // a constant speed at any FPS
    m_position += m_movementVec * m_speed * elapsedTime;
    
    // apply rotation and position changes to the actual car sprite
    m_sprite.setPosition(m_position);
    m_sprite.setRotation(m_rotation);
    
}

void MovableObject::Draw(RenderWindow *window){
    (*window).draw(this->m_sprite);
}
