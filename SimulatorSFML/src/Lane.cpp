//
//  Lane.cpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 03/12/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#include "Lane.hpp"

Lane::Lane(){}
Lane::~Lane(){}

void Lane::Init(int roadNumber, int laneNumber, Vector2f startPosition, float width, float length, float direction)
{
    m_roadNumber = roadNumber;
    m_laneNumber = laneNumber;
    m_startPosition = startPosition;
    m_width = width;
    m_length = length;
    m_direction = direction;
    
    // calculate end position:
    Vector2f lengthVec;
    
    Transform t;
    t.rotate(direction);
    
    lengthVec = t.transformPoint(Vector2f(1.f, 0.f)) * length;
    
    m_endPosition = lengthVec + m_startPosition;
    
    // init rectangle shape
    this->setOrigin(m_width/2, 0.f);
    this->setPosition(m_startPosition);
    this->setOutlineColor(WhiteColor);
    this->setOutlineThickness(1.f);
    this->setRotation(-m_direction - 90.f);
    this->setSize(Vector2f(m_width, m_length));
    
    this->setFillColor(LaneColor);
    
    //cout << startPosition.x << "," << startPosition.y << endl;
    //cout << m_endPosition.x << "," << m_endPosition.y << endl;
}

void Lane::Draw(RenderWindow *window)
{
    window->draw(*this);
}
