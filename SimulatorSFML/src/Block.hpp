#pragma once
//
//  Block.hpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 23/11/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#ifndef Block_hpp
#define Block_hpp

#define LANE_IMAGE  "Blocks/lane.png"
#define EMPTY_IMAGE "Blocks/empty.png"
#define ROAD_IMAGE  "Blocks/road.png"
#define STOP_IMAGE  "Blocks/stop.png"

#include <stdio.h>
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace sf;

typedef enum BlockType{EMPTY, LANE, STOP, TURN} BlockType;

class Block : public RectangleShape{
private:
    BlockType m_type;
    Texture m_texture;
    int m_width;
    int m_height;
    int m_x;
    int m_y;
    int m_lane;
    int m_direction;
    int m_to;
    
public:
    Block();
    ~Block();
    
    void Init(float bWidth, float bHeight, int xPos, int yPos, int dataA, int dataB, int dataC);
};

#endif /* Block_hpp */
