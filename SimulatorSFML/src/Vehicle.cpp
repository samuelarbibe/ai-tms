//
//  MovableObject.cpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 22/11/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#include "Vehicle.hpp"

const Vector2f m_forwardVec(0.f, -1.f);

Vehicle::Vehicle(int vehicleNumber, float maxSpeed, Lane * sourceLane, Lane * destinationLane, Intersection * currentIntersection)
{
    
    // set initial values for the movable object
    m_vehicleNumber = (vehicleNumber)? vehicleNumber : vehicleCount + 1;
    m_maxSpeed       = maxSpeed;
    m_speed          = maxSpeed;
    m_state          = DRIVE;
    m_sourceLane     = sourceLane;
    m_targetLane     = destinationLane;
    m_currentIntersection   = currentIntersection;
    m_currentLane    = m_sourceLane;
    m_angularV      = 0;
        
    m_rotation       = sourceLane->GetDirection();
    m_position       = sourceLane->GetStartPosition();
    m_vehicleInFront = nullptr;
        
    // load texture and set up object sprite
    
    // if car texture hasn't been loaded yet
    if(carTexture.getSize().x == 0)
    {
        // load it
        carTexture.loadFromFile(CAR_IMAGE_DIR);
        
        cout << "Car texture successfully loaded" << endl;
    }
    //m_texture.loadFromFile(CAR_IMAGE_DIR);
    m_sprite.setTexture(carTexture);
    
    m_sprite.setScale(0.12f, 0.12f);
    m_sprite.setOrigin(m_sprite.getTextureRect().width/2, m_sprite.getTextureRect().height/3);
}

void Vehicle::ClearVehicles()
{
    list<Vehicle*>::iterator it = activeVehicles.begin();
    
    // while there are cars to delete;
    while(toBeDeleted != 0 && it != activeVehicles.end())
    {
        // if is to be deleted
        if((*it)->GetState() == DELETE)
        {
            Vehicle * temp = (*it);
            it = activeVehicles.erase(it);
            
            delete temp;
            
            toBeDeleted--;
            cout << "active vehicles : " << activeVehicles.size() << endl;
        }
        else
        {
            it++;
        }
    }
        
}

Vehicle * Vehicle::AddVehicle(int vehicleNumber, float maxSpeed, Lane * sourceLane, Lane * destinationLane, Intersection * currentIntersection)
{
    //add vehicle to vector
    if(vehicleNumber == 0)vehicleNumber = vehicleCount + 1;
    Vehicle * temp = new Vehicle(vehicleNumber,  maxSpeed, sourceLane, destinationLane, currentIntersection);
    activeVehicles.push_back(temp);
    
    temp->m_vehicleInFront = (sourceLane->GetLastCar()) ? GetVehicle(sourceLane->GetLastCar()) : nullptr;
    
    //set this car as the last car that entered the lane
    sourceLane->SetLastCar(vehicleNumber);
    vehicleCount++;
    
    cout << "car " << vehicleNumber << " added to lane " << sourceLane->GetLaneNumber() << endl;

    
    //cout << "active vehicles : " << activeVehicles.size() << endl;
    
    return temp;
}

Vehicle * Vehicle::GetVehicle(int vehicleNumber)
{
    for(Vehicle *v : activeVehicles)
    {
        if(v->m_vehicleNumber == vehicleNumber)
        {
            return v;
        }
    }
    
    return nullptr;
}

void Vehicle::TransferVehicle(int vehicleNumber, Lane * fromLane, Lane * toLane)
{
    Vehicle * temp = GetVehicle(vehicleNumber);
    
    if(temp == nullptr)
    {
        cout << "vehicle not found" << endl;
        return;
    }
    
    if(fromLane != nullptr && temp->m_currentLane->GetLaneNumber() != fromLane->GetLaneNumber())
    {
        cout << "vehicle is not in the given lane" << endl;
        return;
    }
    
    temp->m_currentLane    = toLane;
    temp->m_rotation       = m_currentLane->GetDirection();
    temp->m_angularV       = 0;
    temp->m_speed          = m_speed;
    temp->m_position       = m_currentLane->GetStartPosition();
    temp->m_vehicleInFront = (m_currentLane->GetLastCar()) ? GetVehicle(m_currentLane->GetLastCar()) : nullptr;
    temp->m_currentLane->SetLastCar(temp->m_vehicleNumber);
}

State Vehicle::drive()
{
    
    // check for distance with car in front
    if(m_vehicleInFront != nullptr)
    {
        float distanceFromNextCar = calculateDistance(m_position, m_vehicleInFront->m_position);
        
        if(distanceFromNextCar < 200)
        {
            m_state = STOP;
            if(distanceFromNextCar < 150)
            {
                m_speed = 0;
            }
            else
            {
                float deceleration = (-(m_speed * m_speed)) / (20*(100 - distanceFromNextCar));
                //decelerate car;
                m_speed -= deceleration;
            }
            return m_state;
        }
          
    }
    
    // check if car is in between lanes
    if(m_currentIntersection->getGlobalBounds().contains(m_position))
    {
        m_currentLane = nullptr;
        m_state = TURN;
        
        float distanceSourceTarget = calculateDistance(m_sourceLane->GetEndPosition(), m_targetLane->GetStartPosition());
        
        //TODO: fix this algorithm!
        float turningRadius = distanceSourceTarget / (2 * sin((m_targetLane->GetDirection() - m_sourceLane->GetDirection())/2));
        
        // using  w = (v/r)  to find radial acceleration
        m_angularV = m_speed / turningRadius;
        
        //set rotoation
        m_speed += 1.5f;
        return TURN;
    }
    
    // check distance from stop (if lane is blocked)
    if(m_currentLane != nullptr && m_currentLane != m_targetLane && m_currentLane->GetIsBlocked())
    {
        float distanceFromStop = calculateDistance(this->m_position, m_currentLane->GetEndPosition());
        
        if(distanceFromStop < 150 && distanceFromStop > 20)
        {
            m_state = STOP;
            if(distanceFromStop < 20)
            {
                m_speed = 0;
            }
            else
            {
                float deceleration = (-(m_speed * m_speed)) / (100*(50 - distanceFromStop));
                //decelerate car;
                m_speed -= deceleration;
            }
        }
        
        return m_state;
    }
     
    // check if car is in targetLane
    if(m_currentLane != m_targetLane && m_targetLane->getGlobalBounds().contains(m_position))
    {
        // we need to transfer vehicle to target lane
        TransferVehicle(m_vehicleNumber, nullptr, m_targetLane);
        
        m_speed += 1.5f;
        m_state = DRIVE;
        return DRIVE;
    }
    
    // check if car is no longer in intersection
    if(!m_currentIntersection->getGlobalBounds().contains(m_position) &&
       !m_currentLane->getGlobalBounds().contains(m_position))
    {
        toBeDeleted++;
        
        m_state = DELETE;
        return DELETE;
    }
    
    m_speed += 1.5f;
    m_state = DRIVE;
    return DRIVE;
}

float Vehicle::calculateDistance(Vector2f a, Vector2f b)
{
    float xDist = abs(a.x - b.x);
    float yDist = abs(a.y - b.y);
    
    return sqrt(xDist*xDist + yDist*yDist);
}


void Vehicle::Update(float elapsedTime){
        
    drive();
    applyChanges(elapsedTime);
}

void Vehicle::applyChanges(float elapsedTime)
{
    // apply max speed limit
    if(m_speed > m_maxSpeed) m_speed = m_maxSpeed;
    
    // apply min speed limit
    if(m_speed < 0) m_speed = 0;
    
    // set rotation relative to current speed, to create a constant turning radius
    
    Transform t;
    
    m_rotation += m_angularV * elapsedTime * m_speed / 3.6f;
    
    t.rotate(m_rotation);
    
    // rotate the movement vector in relation to the forward vector (0,1)
    m_movementVec = t.transformPoint(m_forwardVec);
    
    // apply movement vector on position, relative to elapsed time to ensure
    // a constant speed at any FPS
    m_position += m_movementVec * m_speed * elapsedTime;
    
    // apply rotation and position changes to the actual car sprite
    m_sprite.setPosition(m_position);
    m_sprite.setRotation(m_rotation);
}

void Vehicle::Draw(RenderWindow *window)
{
    (*window).draw(this->m_sprite);
}
