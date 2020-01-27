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
    Vector2f GetSnappedPoint(Vector2f point);
    Vector2f DrawPoint(Vector2f position);
    void SetView(float zoom, Vector2f pos);
    void SetMinimap(float size, float margin);

    View GetView(){return m_view;}

    Map * map;

private:

    void OnInit();
    void OnUpdate();
    void OnDraw();

    void input();
    void DrawMinimap();
    void updateShownArea();
    void updateView(Vector2f posDelta);
    void update(float elapsedTime);

    CircleShape m_clickPoint;
    bool m_snapToGrid = true;
    bool m_showGrid;
    Grid m_snapGrid;

    View m_view;
    View m_minimap;

    RectangleShape m_minimapBackground;
    RectangleShape m_shownArea;
};


#endif /* Engine_hpp */
