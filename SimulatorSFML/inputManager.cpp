//
//  inputManager.cpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 18/11/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#include "inputManager.hpp"


namespace Sim {

    bool inputManager::IsSpriteClicked(sf::Sprite object, sf::Mouse::Button button, sf::RenderWindow &window){
        
        if (sf::Mouse::isButtonPressed(button)) {
            
            //create a temp rect around object
            sf::IntRect tempRect(object.getPosition().x,
                                 object.getPosition().y,
                                 object.getGlobalBounds().width,
                                 object.getGlobalBounds().height);
            
            if (tempRect.contains(GetMousePosition(window))) { // check if click is on object
                return true;
            }
        }
        
        return false;
    }

    sf::Vector2i inputManager::GetMousePosition(sf::RenderWindow &window){
        return sf::Mouse::getPosition(window);
    }
}
