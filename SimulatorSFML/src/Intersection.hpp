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
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <fstream>
#include "Block.hpp"

using namespace sf;

class Intersection{
private:
    
    Block   **   m_grid;
    char  * m_renderDir;
    const char * m_name;
    
    int m_rows;
    int m_cols;
    int m_width;
    int m_height;
    int m_cellWidth;
    int m_cellHeight;
    int m_numOfLanes;
    int * m_lanes;
    
    Texture m_texture;
    Sprite  m_sprite;
    
    RenderWindow * m_window;
        
    void LoadMapFromFile(const char * dirName);
    void CreateMapRender(const char * fileName);
 
public:
    Intersection();
    ~Intersection();
    
    void Init(int rows, int cols, int width, int height, const char * mapName, RenderWindow * window);
    void Draw(RenderWindow * window);
     
    Block ** GetGrid(){return this->m_grid;};
};

#endif /* Intersection_hpp */
