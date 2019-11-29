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

void Intersection::Init(int rows, int cols, int width, int height, const char * mapName, RenderWindow * window){
    
    this->m_name = mapName;
    this->m_rows   = rows;
    this->m_cols   = cols;
    this->m_width  = width;
    this->m_height = height;
    
    this->m_cellWidth  = width / cols;
    this->m_cellHeight = height / rows;
    
    this->m_window = window;
    
    m_grid = new Block*[rows];
    
    for (int i = 0; i < rows; i++) {
        m_grid[i] = new Block[cols];
    }
    
    
    if(mapName != nullptr){
        LoadMapFromFile(mapName);
    }
    /*
    else
    {
            
        std::ofstream fp("Maps/map.txt");
        
        if(!fp){
            std::cout << "Error opening map file." << std::endl;
            return;
        }
           
        fp << "Maps/none.jpg\n";
        fp << "4\n\n";
        
        for (int i = 0; i < m_rows; i++) {
            for (int j = 0; j < m_cols; j++) {
                fp << 0;
            }
        }
        
        fp.close();
        
        this->m_name = strdup("Maps/map.txt");
    }
    */
    m_texture.loadFromFile(this->m_renderDir);
    m_sprite.setTexture(m_texture);
}

void Intersection::LoadMapFromFile(const char *dirName){
    
    FILE * fp;
    char buffer[64];
    int data;
    
    fp = fopen(dirName, "rt");
    
    if(!fp){
        std::cout << "Error opening map file. Check if the file exists..." << std::endl;
        return;
    }
       
    // get render map render directory
    fscanf(fp, "%s", buffer);
    this->m_renderDir = strdup(buffer);
    
    // get number of lanes
    fscanf(fp, "%d", &(this->m_numOfLanes));
    
    // create a lane array
    this->m_lanes = (int *)malloc((m_numOfLanes + 1) * sizeof(int));
    
    // fill each i = laneNumber of array with its direction
    for (int i = 1; i <= this->m_numOfLanes; i++) {
        fscanf(fp, "%d", &(this->m_lanes[i]));
    }
    

    for (int i = 0; i < m_rows; i++) {
        for (int j = 0; j < m_cols; j++) {
            fscanf(fp, "%s", buffer);
            data = atoi(buffer);
            this->m_grid[i][j] = Block();
            this->m_grid[i][j].Init(m_cellWidth,
                                    m_cellHeight,
                                    j*m_cellWidth  + m_cellWidth/2,
                                    i*m_cellHeight + m_cellHeight/2,
                                    (data/100),
                                    ((data/10) % 10),
                                    (data%10));
            
            //std::cout << "(" << j*m_cellWidth << "," << i*m_cellHeight << ") - " << data << std::endl;
        }
    }
    
    std::cout << "Map loaded from " << dirName << " succefully" << std::endl;
    
    fclose(fp);
    
}

void Intersection::CreateMapRender(const char * fileDir)
{
    sf::Texture texture;

    texture.create(this->m_window->getSize().x, this->m_window->getSize().y);
    texture.update(*(this->m_window));
    
    if (texture.copyToImage().saveToFile(fileDir)){
        std::cout << "new map render saved to " << fileDir << std::endl;
        this->m_renderDir = strdup(fileDir);
    }
    else
    {
        std::cout << "could not create new map render to" << fileDir << std::endl;
    }
}

void Intersection::Draw(RenderWindow *window){
    
    if(m_texture.getSize().x == 0){
        
        for (int i = 0; i < this->m_rows; i++) {
            
            for (int j = 0; j < this->m_cols; j++) {
                
                (*window).draw(this->m_grid[i][j]);
                
            }
        }
        
        std::cout << "map render for <" << this->m_name << "> not found." << std::endl;
        CreateMapRender(this->m_renderDir);

        this->m_texture.loadFromFile(this->m_renderDir);
        this->m_sprite.setTexture(m_texture);
    }else{
 
        (*window).draw(m_sprite);
    }
    
}



