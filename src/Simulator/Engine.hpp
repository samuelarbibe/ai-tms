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
#include <cmath>

#include <SFML/Graphics.hpp>
#include <QtWidgets>
#include <nlohmann/json.hpp>

#include "Map/Map.hpp"
#include "Vehicle.hpp"
#include "Settings.hpp"
#include "../UI/Widgets/QsfmlCanvas.hpp"


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

    void SetSnapToGrid(bool snapToGrid){this->snap_to_grid_ = snapToGrid;}
    void SetMinimap(float size, float margin);
    void SetView();

    void BuildGrid(int rows, int cols);
    void UpdateView(Vector2f posDelta = Vector2f(0,0), float zoom = 0);
    void SaveMap(string saveDirectory);
    void LoadMap(string loadDirectory);
    void ResetMap();

    Vector2f DrawPoint(Vector2f position);
    Vector2f GetSnappedPoint(Vector2f point);

    Map * map;

private:

    void on_init();
    void cycle();
    void render();

    void input();
    void draw_minimap();
    void update_shown_area();
    void update(float elapsedTime);
    void check_selection(Vector2f position);

    CircleShape click_point_;

    bool snap_to_grid_;
    Grid snap_grid_;

    View view_;
    View minimap_;

    Vector2f view_pos_;
    Vector2f t_view_pos_;

    RectangleShape minimap_bg_;
    RectangleShape shown_area_index_;
};


#endif /* Engine_hpp */
