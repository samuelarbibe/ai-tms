//
//  assetManager.hpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 18/11/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//
#pragma once

#ifndef assetManager_hpp
#define assetManager_hpp

#include <stdio.h>
#include <iostream>
#include <map>

#include <SFML/Graphics.hpp>

namespace Sim {
    
    class assetManager{
        
    public:
        assetManager(){}
        ~assetManager(){}
        
        void LoadTexture(std::string name, std::string fileName);
        sf::Texture &GetTexture(std::string name);
        
    private:
        std::map<std::string, sf::Texture> m_textures;
    };
}


#endif /* assetManager_hpp */
