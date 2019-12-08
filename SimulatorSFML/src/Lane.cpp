//
//  Lane.cpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 03/12/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#include "Lane.hpp"

Lane::Lane(){}

Lane::Lane(int laneNumber, int roadNumber, Vector2f startPosition, float width, float length, float direction)
{
    m_laneNumber    = laneNumber;
    m_roadNumber    = roadNumber;
    m_startPosition = startPosition;
    m_width         = width;
    m_length        = length;
    m_direction     = fmod(direction,360.f);
    m_isBlocked     = false;
    m_vehicleCount  = 0;
    
    // calculate end position:
    Vector2f lengthVec;
    
    Transform t;
    t.rotate(direction);
    
    lengthVec = t.transformPoint(Vector2f(0.f, -1.f)) * length;
    
    m_endPosition = lengthVec + m_startPosition;
    
    // init rectangle shape
    this->setOrigin(m_width/2, 0.f);
    this->setPosition(m_startPosition);
    this->setOutlineColor(WhiteColor);
    this->setOutlineThickness(1.f);
    this->setRotation(m_direction + 180);
    this->setSize(Vector2f(m_width, m_length));
    
    this->setFillColor(LaneColor);
    
    
    // create direction arrow shape
    
    m_createArrowShape(t);
}

Lane::~Lane(){}


void Lane::m_createArrowShape(Transform t)
{
    m_arrowShape.setPointCount(7);
    
    t.scale(m_width/4, m_width/4);
    m_arrowShape.setPoint(0, m_endPosition - t.transformPoint(Vector2f(0.f, -2.f)) );
    
    t.rotate(-45);
    m_arrowShape.setPoint(1, m_arrowShape.getPoint(0) - t.transformPoint(Vector2f(0.f, -1.f)));
    
    t.rotate(90);
    m_arrowShape.setPoint(6, m_arrowShape.getPoint(0) - t.transformPoint(Vector2f(0.f, -1.f)));
    
    t.rotate(45);
    m_arrowShape.setPoint(2, m_arrowShape.getPoint(1) - t.transformPoint(Vector2f(0.f, -1.0f)));
    
    t.rotate(180);
    m_arrowShape.setPoint(5, m_arrowShape.getPoint(6) - t.transformPoint(Vector2f(0.f, -1.f)));
    
    t.rotate(90);
    m_arrowShape.setPoint(3, m_arrowShape.getPoint(2) - t.transformPoint(Vector2f(0.f, -1.f)));
    
    m_arrowShape.setPoint(4, m_arrowShape.getPoint(5) - t.transformPoint(Vector2f(0.f, -1.f)));
    
    m_arrowShape.setFillColor(WhiteColor);
}


Vehicle * Lane::AddVehicle(Lane * destinationLane)
{
    Vehicle * temp = nullptr;
    
    if(!m_vehicles.empty())
    {
        temp = m_vehicles[m_vehicleCount - 1];
    }
    
    m_vehicles.push_back(new Vehicle(0, 100, this, destinationLane, temp));
    m_vehicleCount++;
    
    temp = m_vehicles[m_vehicleCount - 1];
    
    
    cout << "car " << temp->GetVehicleNumber() << " added to lane " << m_laneNumber << endl;
    
    return temp;
}

void Lane::Update(float elapsedTime)
{
    for(Vehicle * v : m_vehicles)
    {
        v->Update(elapsedTime);
    }
}

void Lane::Draw(RenderWindow *window)
{
    window->draw(*this);
    window->draw(m_arrowShape);
    
    for(Vehicle * v : m_vehicles)
    {
        v->Draw(window);
    }
}


