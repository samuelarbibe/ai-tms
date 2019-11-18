//
//  assetManager.cpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 18/11/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#include "assetManager.hpp"

namespace Sim {
    
    void assetManager::LoadTexture(std::string name, std::string fileName){
        
        sf::Texture tex;
        
        if(tex.loadFromFile(fileName)){ // if succefully loaded texture from file
            this->m_textures[name] = tex;
        }else{
            std::cout << "Error loading texture from file:" << fileName << " for " << name << std::endl;
        }
    }

    sf::Texture & assetManager::GetTexture(std::string name){
        
        return this->m_textures.at(name);
    }
}
