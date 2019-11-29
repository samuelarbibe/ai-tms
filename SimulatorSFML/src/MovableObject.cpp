//
//  MovableObject.cpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 22/11/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#include "MovableObject.hpp"

MovableObject::MovableObject(){};

MovableObject::~MovableObject(){}

void MovableObject::Init(float initSpeed, float initRotation, int x, int y, const char * textureDirName){
    
    // set initial values for the movable object
    m_speed    = initSpeed;
    m_rotation = initRotation;
    m_position.x = x;
    m_position.y = y;
    
    // load texture and set up object sprite
    m_texture.loadFromFile(textureDirName);
    m_sprite.setTexture(m_texture);
    
    m_sprite.setScale(0.15f, 0.15f);
    m_sprite.setOrigin(m_sprite.getTextureRect().width/3, m_sprite.getTextureRect().height/2);
}

Sprite MovableObject::GetSprite(){
    return m_sprite;
}

void MovableObject::Move(float rotationDt, float speedDt){
    
    m_rotation += (m_speed == 0)? 0 : (rotationDt * m_speed / 1500);
    m_speed = (m_speed < 0)? 0 : (m_speed + speedDt);
}

void MovableObject::Update(float elapsedTime){
    
    m_position.x += cos(m_rotation * M_PI /180) * m_speed * elapsedTime ;
    m_position.y += sin(m_rotation * M_PI /180) * m_speed * elapsedTime ;
    
    m_sprite.setPosition(m_position);
    m_sprite.setRotation(m_rotation);
    
}

void MovableObject::Draw(RenderWindow *window){
    (*window).draw(this->m_sprite);
}
