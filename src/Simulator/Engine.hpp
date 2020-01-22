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
#include "Settings.hpp"

using namespace sf;

struct Grid
{
    int Columns;
    int Rows;
    int ColumnWidth;
    int RowHeight;
    list<Vertex*> Lines;
};

class Engine : public QSFMLCanvas
{
public:

    Engine(QWidget* Parent) : QSFMLCanvas(Parent, 1000/Settings::MaxFps){}

    void SetSnapToGrid(bool snapToGrid){this->m_snapToGrid = snapToGrid;}
    void ShowGrid(bool showGrid){this->m_showGrid = showGrid;}
    void BuildGrid(int rows, int cols);
    Vector2f GetMousePos();
    Vector2f GetSnappedPoint(Vector2f point);
    Vector2f DrawPoint(Vector2f position);

    View GetView(){return m_view;}

    Map * map;

private:

    void OnInit();
    void OnUpdate();
    void OnDraw();

    void input();
    void update(float elapsedTime);

    CircleShape m_clickPoint;
    bool m_snapToGrid;
    bool m_showGrid;
    Grid m_snapGrid;

    View m_view;
};


#endif /* Engine_hpp */
