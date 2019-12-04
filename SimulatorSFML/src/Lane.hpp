//
//  Lane.hpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 03/12/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#ifndef Lane_hpp
#define Lane_hpp

#include <stdio.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <queue>

using namespace std;
using namespace sf;

const Color LaneColor(45, 45, 45);
const Color WhiteColor(230, 230, 230);
const Color BackgroundColor(150, 150, 150);

static int laneCount = 0;

class Lane : public RectangleShape
{
private:
    
    int m_roadNumber;
    int m_laneNumber;
    Vector2f m_startPosition;
    Vector2f m_endPosition;
    float m_direction;
    float m_width;
    float m_length;
    
    ConvexShape m_arrowShape;
    
    void createArrowShape();
    
public:
    Lane();
    ~Lane();
    
    void Init(int roadNumber, int laneNumber, Vector2f startPosition, float width, float length, float direction);
    
    float GetDirection(){return m_direction;};
    int   GetLaneNumber(){return m_laneNumber;};
    
    void Draw(RenderWindow * window);
};

#endif /* Lane_hpp */
