//
//  Engine.hpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 22/11/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#ifndef Engine_hpp
#define Engine_hpp

#include <iostream>
#include <list>
#include <SFML/Graphics.hpp>
#include "Intersection.hpp"
#include "Vehicle.hpp"

using namespace sf;

class Engine{
private:
    
    RenderWindow m_window;
    
    Sprite m_backgroundSprite;
    Texture m_backgroundTexture;
    
    void input();
    void update(float dtInSeconds);
    void draw();
    
public:
    Engine(int windowWidth, int windowHeight, const char * windowName);
        
    Intersection * inter;
    
    void Start();
};


#endif /* Engine_hpp */
