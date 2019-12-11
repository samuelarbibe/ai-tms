//
//  MovableObject.cpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 22/11/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#include "Vehicle.hpp"

const Vector2f m_forwardVec(0.f, -1.f);

Vehicle::Vehicle(vector<Vehicle*> &activeVehicles, int carNumber, float maxSpeed, Lane * sourceLane, Lane * destinationLane){
    
    // set initial values for the movable object
    m_veheicleNumber = (carNumber)? carNumber : vehicleCount + 1;
    m_maxSpeed       = maxSpeed;
    m_speed          = maxSpeed;
    m_state          = DRIVE;
    m_sourceLane     = sourceLane;
    m_targetLane     = destinationLane;
    m_currentLane    = m_sourceLane;
        
    m_rotation       = sourceLane->GetDirection();
    m_position       = sourceLane->GetStartPosition();
    m_vehicleInFront = (sourceLane->GetLastCar()) ? GetVehicle(activeVehicles, sourceLane->GetLastCar()) : nullptr;
    
    sourceLane->SetLastCar(m_veheicleNumber);
    
    // load texture and set up object sprite
    m_texture.loadFromFile("/Users/samuelarbibe/Documents/SimulatorSFML/SimulatorSFML/assets/Cars/car_image2.png");
    m_sprite.setTexture(m_texture);
    
    m_sprite.setScale(0.12f, 0.12f);
    m_sprite.setOrigin(m_sprite.getTextureRect().width/2, m_sprite.getTextureRect().height/3);
}

Vehicle * Vehicle::GetVehicle(vector<Vehicle*> &activeVehicles, int vehicleNumber)
{
    for(Vehicle * v : activeVehicles)
    {
        if(v->GetVehicleNumber() == vehicleCount)
        {
            return v;
        }
    }
    return nullptr;
}


Vehicle * Vehicle::AddVehicle(vector<Vehicle*> &activeVehicles, int carNumber, float maxSpeed, Lane *sourceLane, Lane *destinationLane)
{
    Vehicle * temp = new Vehicle(activeVehicles, carNumber, maxSpeed, sourceLane, destinationLane);
    
    activeVehicles.resize(vehicleCount + 1);
    
    activeVehicles[vehicleCount++] = temp;
        
    cout << "Car " << temp->GetVehicleNumber() << " created" << endl;
    
    return temp;
}

void Vehicle::Update(float elapsedTime){
    
    calculateState();
    
    switch (m_state)
    {
        case DRIVE:
                Move(0.f, 2.f);
        
            break;
        case STOP:
                Move(0.f, -2.f);
            
            break;
        default:
            break;
    }
    
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
    
    if(distanceFromNextCar < 200 || (m_currentLane->GetIsBlocked() && distanceFromStop < 100))
    {
        m_state = STOP;
    }
    else
    {
        m_state = DRIVE;
    }
}

void Vehicle::Move(float rotationDt, float speedDt){
    
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


void Vehicle::Draw(RenderWindow *window)
{
    (*window).draw(this->m_sprite);
}
