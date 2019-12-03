#pragma once
//
//  Intersection.hpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 23/11/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#ifndef Intersection_hpp
#define Intersection_hpp

#include <stdio.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include "Lane.hpp"

using namespace sf;

class Intersection{
private:
    
    char  * m_renderDir;
    int m_intersectionNumber;
    
    Lane * m_lanes;
    int m_numOfLanes;
    
    int m_width;
    int m_height;
    Vector2f m_position;
    
    Texture m_texture;
    Sprite  m_sprite;
    
    RenderWindow * m_window;
        
    void LoadMapFromFile(const char * dirName);
    void CreateMapRender(const char * fileName);
 
public:
    Intersection();
    ~Intersection();
    
    void Init(Vector2f position, int width, int height, int intersectioNumber, RenderWindow * window);
    void Draw(RenderWindow * window);
     
    Lane * GetLanes(){return this->m_lanes;};
};

#endif /* Intersection_hpp */
