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
#include <fstream>
#include <list>
#include <SFML/Graphics.hpp>
#include "../UI/Widgets/QsfmlCanvas.hpp"
#include <cmath>
#include <QtWidgets>
#include <nlohmann/json.hpp>
#include "Map/Map.hpp"
#include "Vehicle.hpp"
#include "Settings.hpp"


using namespace sf;
using json = nlohmann::json;

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

    Engine(QWidget* Parent);
    ~Engine(){};

    void SetSnapToGrid(bool snapToGrid){this->m_snapToGrid = snapToGrid;}
    void ShowGrid(bool showGrid){this->m_showGrid = showGrid;}
    void BuildGrid(int rows, int cols);
    Vector2f GetSnappedPoint(Vector2f point);
    Vector2f DrawPoint(Vector2f position);
    void SetView();
    void UpdateView(Vector2f posDelta = Vector2f(0,0), float zoom = 0);
    void SetMinimap(float size, float margin);
    void ResetMap();
    void SaveMap(string saveDirectory);
    void LoadMap(string loadDirectory);

    Map * map;

private:

    void OnInit();
    void OnUpdate();
    void OnDraw();

    void input();
    void DrawMinimap();
    void updateShownArea();
    void update(float elapsedTime);
    void checkSelection(Vector2f position);

    CircleShape m_clickPoint;
    bool m_snapToGrid = true;
    bool m_showGrid;
    Grid m_snapGrid;

    View m_view;
    View m_minimap;

    Vector2f m_viewPosition;
    Vector2f m_viewTempPosition;

    RectangleShape m_minimapBackground;
    RectangleShape m_shownArea;

};


#endif /* Engine_hpp */
