//
//  MovableObject.cpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 22/11/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#include "Vehicle.hpp"

const Vector2f m_forwardVec(0.f, -1.f);

Vehicle::Vehicle(vector<Vehicle*> &activeVehicles, int carNumber, float maxSpeed, Lane * sourceLane, Lane * destinationLane, Intersection * currentIntersection)
{
    
    // set initial values for the movable object
    m_veheicleNumber = (carNumber)? carNumber : vehicleCount + 1;
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
    m_activeVehiclesPtr = &activeVehicles;
    m_vehicleInFront = (sourceLane->GetLastCar()) ? GetVehicle(sourceLane->GetLastCar(), activeVehicles) : nullptr;
    
    sourceLane->SetLastCar(m_veheicleNumber);
    
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

Vehicle * Vehicle::GetVehicle(int vehicleNumber, vector<Vehicle*> &activeVehicles)
{
    for(Vehicle * v : activeVehicles)
    {
        if(v->GetVehicleNumber() == vehicleNumber)
        {
            return v;
        }
    }
    return nullptr;
}


Vehicle * Vehicle::AddVehicle(vector<Vehicle*> &activeVehicles, int carNumber, float maxSpeed, Lane *sourceLane, Lane *destinationLane, Intersection * currentIntersection)
{
    Vehicle * temp = new Vehicle(activeVehicles, carNumber, maxSpeed, sourceLane, destinationLane, currentIntersection);
    
    activeVehicles.resize(vehicleCount + 1);
    
    activeVehicles[vehicleCount++] = temp;
        
    cout << "Car " << temp->GetVehicleNumber() << " created" << endl;
    
    return temp;
}

void Vehicle::RemoveVehicle(int vehicleNumber, vector<Vehicle*> &activeVehicles)
{
    Vehicle * temp = Vehicle::GetVehicle(vehicleNumber, activeVehicles);
    
    activeVehicles.erase(remove(activeVehicles.begin(), activeVehicles.end(), temp));
    
    vehicleCount--;
    
    cout << "Car " << Vehicle::GetVehicle(vehicleNumber, activeVehicles) << " deleted" << endl;
    
    //delete temp;
}

void Vehicle::Update(float elapsedTime){
    
    calculateState();
    
    // update speed
    switch (m_state)
    {
        case DRIVE:
                m_speed += (2.f);
            cout << "DRIVING" << endl;
            break;
            
        case TURN:
                m_speed += (0.f);
            cout << "TURNING" << endl;
        
            break;
        case STOP:
                m_speed += (-3.f);
            
            break;
        default:
            break;
    }
    
    // apply max speed limit
    if(m_speed > m_maxSpeed) m_speed = m_maxSpeed;
    
    // apply min speed limit
    if(m_speed < 0) m_speed = 0;
    
    // set rotation relative to current speed, to create a constant turning radius
    m_rotation += m_angularV / 1.2;
    
    Transform t;
    
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

void Vehicle::calculateState()
{
    // check for distance with car in front
    float xDist, yDist, distanceFromNextCar = 400;
    
    if(m_vehicleInFront != nullptr)
    {
        xDist = abs(m_position.x - m_vehicleInFront->m_position.x);
        yDist = abs(m_position.y - m_vehicleInFront->m_position.y);
        
        distanceFromNextCar = sqrt(xDist*xDist + yDist*yDist);
    }
    
    xDist = abs(m_position.x - m_currentLane->GetEndPosition().x);
    yDist = abs(m_position.y - m_currentLane->GetEndPosition().y);
    
    float distanceFromStop = sqrt(xDist*xDist + yDist*yDist);
      
    // if car is turning mid-lane
    if(m_currentIntersection->getGlobalBounds().contains(m_position))
    {
        // set current lane as target lane
        if(m_currentLane == m_sourceLane)
        {
            m_currentLane = m_targetLane;
            
            m_vehicleInFront = Vehicle::GetVehicle(m_currentLane->GetLastCar(), *m_activeVehiclesPtr);
            m_currentLane->SetLastCar(m_veheicleNumber);
        }
        
        xDist = abs(m_sourceLane->GetEndPosition().x - m_targetLane->GetStartPosition().x);
        yDist = abs(m_sourceLane->GetEndPosition().y - m_targetLane->GetStartPosition().y);
        
        float distanceSourceTarget = sqrt(xDist*xDist + yDist*yDist);
        
        
        //TODO: fix this algorithm!
        float turningRadius = distanceSourceTarget / (2 * sin((m_targetLane->GetDirection() - m_sourceLane->GetDirection())/2));
        
        // using  w = (v/r)  to find radial acceleration
        m_angularV = m_speed / turningRadius;
        
        m_state = TURN;
    }
    // if car is no longer in intersection
    else if(!m_sourceLane->getGlobalBounds().contains(m_position) && !m_targetLane->getGlobalBounds().contains(m_position))
    {
        Vehicle::RemoveVehicle(m_veheicleNumber, *m_activeVehiclesPtr);
    }
    else
    {
        m_angularV = 0;
        m_state = DRIVE;
    }
    
    if(distanceFromNextCar < 200 || (m_sourceLane->getGlobalBounds().contains(m_position) && m_sourceLane->GetIsBlocked() && distanceFromStop < 150))
    {
        m_state = STOP;
    }
}


void Vehicle::Draw(RenderWindow *window)
{
    (*window).draw(this->m_sprite);
}
