//
//  inputManager.hpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 18/11/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//
#pragma once

#ifndef inputManager_hpp
#define inputManager_hpp

#include <stdio.h>
#include <iostream>
#include <SFML/Graphics.hpp>

namespace Sim {
    class inputManager{
        
    public:
        inputManager(){}
        ~inputManager(){}
        
        bool IsSpriteClicked( sf::Sprite object, sf::Mouse::Button button, sf::RenderWindow &window); // check if sprite is being clicked
        
        sf::Vector2i GetMousePosition(sf::RenderWindow &window); // return the current mouse position, in case mouse is being clicked
    };
}

#endif /* inputManager_hpp */
