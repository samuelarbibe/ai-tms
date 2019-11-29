//
//  Block.cpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 23/11/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#include "Block.hpp"

Block::Block(){};

Block::~Block(){};

void Block::Init(float bWidth, float bHeight, int xPos, int yPos, int dataA, int dataB, int dataC){
    
    this->m_width = bWidth;
    this->m_height = bHeight;
    this->m_x = xPos;
    this->m_y = yPos;
    this->m_direction = dataB;
    this->m_type = static_cast<BlockType>(dataA);
    
    
    this->setSize(*(new Vector2f(m_width, m_height)));
    this->setPosition(m_x, m_y);
    this->setOutlineColor(Color::White);
    this->setOutlineThickness(0.f);
    this->setOrigin(bWidth/2, bHeight/2);
    
    switch (m_type) {
        case (EMPTY):
            
            m_texture.loadFromFile(EMPTY_IMAGE);
            
            break;
        case (LANE):
            
            m_texture.loadFromFile(LANE_IMAGE);
            this->setTexture(&m_texture);
            
            this->setRotation(-((dataB - 1) * 90));
            this->m_lane = dataC;
            
            break;
        case (STOP):
            
            m_texture.loadFromFile(STOP_IMAGE);
            this->setTexture(&m_texture);
            
            this->setRotation(-((dataB - 1) * 90));
            this->m_lane = dataC;
            break;
        case (TURN):
            
            m_texture.loadFromFile(ROAD_IMAGE);
            this->setTexture(&m_texture);
            
            this->m_lane = dataC; // turn from lane x
            this->m_to = dataB; // to lane y
            
            break;
        default:
            break;
    }
    
    this->setTexture(&m_texture);
}
