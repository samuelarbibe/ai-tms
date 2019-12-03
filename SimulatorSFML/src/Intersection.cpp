//
//  Intersection.cpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 23/11/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#include "Intersection.hpp"

Intersection::Intersection(){};

Intersection::~Intersection(){};

void Intersection::Init(Vector2f position, int width, int height, int intersectioNumber, RenderWindow * window)
{
    
    this->m_intersectionNumber = intersectioNumber;
    this->m_position = position;
    this->m_width  = width;
    this->m_height = height;
        
    this->m_window = window;
}

void Intersection::LoadMapFromFile(const char *dirName)
{

}


void Intersection::CreateMapRender(const char * fileDir)
{
    /*
    sf::Texture texture;

    texture.create(this->m_window->getSize().x, this->m_window->getSize().y);
    texture.update(*(this->m_window));
    
    // screenshot map and save it to file
    if (texture.copyToImage().saveToFile(fileDir))
    {
        std::cout << "new map render saved to " << fileDir << std::endl;
        this->m_renderDir = strdup(fileDir);
    }
    else
    {
        std::cout << "could not create new map render to" << fileDir << std::endl;
    }
     */
}

void Intersection::Draw(RenderWindow *window)
{

        (*window).draw(m_sprite);
}

