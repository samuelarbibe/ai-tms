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
#include "../UI/Widgets/QsfmlCanvas.hpp"
#include <math.h>
#include "Map/Map.hpp"
#include "Vehicle.hpp"
#include "Flags.hpp"

using namespace sf;

struct Grid
{
    int Coloumns;
    int Rows;
    int ColoumnWidth;
    int RowHeight;
};

class Engine : public QSFMLCanvas
{
public:

    Engine(QWidget* Parent) : QSFMLCanvas(Parent, 1000/MAX_FPS){}

    void SetSnapToGrid(bool snapToGrid){this->m_snapToGrid = snapToGrid;}
    Vector2i GetSnappedPoint(Vector2i point);
    Vector2i DrawPoint(Vector2i position);
    Map * map;

private:

    void OnInit();
    void OnUpdate();
    void OnDraw();

    void input();
    void update(float elapsedTime);

    CircleShape m_clickPoint;
    bool m_snapToGrid;
    Grid m_snapGrid;
};


#endif /* Engine_hpp */
