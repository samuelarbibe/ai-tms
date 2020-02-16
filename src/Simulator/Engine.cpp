//
//  Engine.cpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 22/11/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#include "Engine.hpp"


Engine::Engine(QWidget* Parent) : QSFMLCanvas(Parent, 1000/Settings::MaxFps)
{

    cout << "Setting Up Map..." << endl;
    map = new Map(0, Vector2i(this->width()/2, this->height()/2), Settings::DefaultMapWidth, Settings::DefaultMapWidth);

    cout << "Setting Up Camera..." << endl;
    snap_to_grid_ = true;
    view_pos_ = Vector2f(0, 0);
    t_view_pos_ = Vector2f(0, 0);
    SetView();
    SetMinimap(Settings::MinimapSize, Settings::MinimapMargin);
    this->setView(view_);

    cout << "Setting up snap grid..." << endl;
    BuildGrid(Settings::GridRows, Settings::GridColumns);

    cout << "Setting up max speeds..." << endl;
    Settings::MaxSpeeds[VehicleTypeOptions::CAR] = Settings::ConvertVelocity(VelocityUnits::KMH, VelocityUnits::PXS, 100.f);
    Settings::MaxSpeeds[VehicleTypeOptions::TRUCK] = Settings::ConvertVelocity(VelocityUnits::KMH, VelocityUnits::PXS, 90.f);
    Settings::MaxSpeeds[VehicleTypeOptions::MOTORCYCLE] = Settings::ConvertVelocity(VelocityUnits::KMH, VelocityUnits::PXS, 110.f);
}

/// set up the map according to the selected presets
void Engine::on_init()
{
    /*
    map->AddIntersection(0, map->GetSize()/2.f);

    map->AddRoad(0, 1, RIGHT, Settings::DefaultLaneLength);
    map->AddRoad(0, 1, DOWN, Settings::DefaultLaneLength);

    map->AddLane(0, 1, false);
    map->AddLane(0, 2, true);

    map->AddRoute(1, 2);
    */
}

/// set the viewport for the camera
void Engine::SetView()
{
    // view setup
    view_.reset(sf::FloatRect(t_view_pos_.x, t_view_pos_.y, Settings::DefaultMapWidth, Settings::DefaultMapHeight));
    view_.zoom(Settings::Zoom);
    // update the shown area index rectangle
    update_shown_area();
}

/// build the minimap
void Engine::SetMinimap(float size, float margin)
{
    // minimap viewPort setup
    minimap_.reset(sf::FloatRect(0, 0, Settings::DefaultMapWidth, Settings::DefaultMapWidth));
    minimap_.setSize(Vector2f(size, size));
    minimap_.setViewport(sf::FloatRect(1.f - (size / this->width()) - (margin / this->height()), margin / this->height() , size / this->width(), size / this->height()));
    float zoomFactor = Settings::DefaultMapWidth / size;
    minimap_.zoom(zoomFactor);
    float outlineThickness = 30;

    // background setup
    minimap_bg_ = RectangleShape(Vector2f(Settings::DefaultMapWidth - outlineThickness * 2,
            Settings::DefaultMapWidth - outlineThickness*2));
    minimap_bg_.setPosition(outlineThickness, outlineThickness);
    minimap_bg_.setFillColor(Color(110, 110, 110, 220));
    minimap_bg_.setOutlineColor(Color::Black);
    minimap_bg_.setOutlineThickness(outlineThickness);

    shown_area_index_ = RectangleShape(Vector2f(0, 0));
    shown_area_index_.setOutlineColor(Color(255, 0, 0, 100));
    shown_area_index_.setOutlineThickness(30.f);
    shown_area_index_.setFillColor(Color::Transparent);
    update_shown_area();
}

/// update camera after movement, and enforce overflow
void Engine::UpdateView(Vector2f posDelta, float newZoom)
{
    // view_pos_ is position before dragging
    // t_view_pos_ is the current view position
    t_view_pos_ = view_pos_ + posDelta;

    // enforce overflow blocking
    if(!Settings::MapOverflow && map != nullptr)
    {
        if(abs(t_view_pos_.x) > map->GetSize().x / 2 - shown_area_index_.getSize().x / 2)
        {
            t_view_pos_.x = (map->GetSize().x / 2 - shown_area_index_.getSize().x / 2) * t_view_pos_.x / abs(t_view_pos_.x);
        }
        if (abs(t_view_pos_.y) > map->GetSize().y / 2 - shown_area_index_.getSize().y / 2)
        {
            t_view_pos_.y = (map->GetSize().y / 2 - shown_area_index_.getSize().y / 2) * t_view_pos_.y / abs(t_view_pos_.y);
        }
    }

    if(newZoom != 0)
    {
        Settings::Zoom = newZoom;
    }
    // set view
    SetView();
}

/// update the shown area index in minimap
void Engine::update_shown_area()
{
    // calculate the actual position of the shown area
    Vector2f position = view_.getCenter();

    // to calculate shown area dimensions,
    // we multiply the map size by the zoom
    // view_size = map_size * zoom
    Vector2f size = view_.getSize();

    this->shown_area_index_.setSize(size);
    this->shown_area_index_.setOrigin(size / 2.f);
    this->shown_area_index_.setPosition(position);
}

/// build the snap grid
void Engine::BuildGrid(int rows, int cols)
{
    snap_grid_.Lines.clear(); // clear the old lines list
    snap_grid_.Rows = rows;
    snap_grid_.Columns = cols;
    snap_grid_.ColumnWidth = map->GetSize().x / snap_grid_.Columns;
    snap_grid_.RowHeight = map->GetSize().y / snap_grid_.Rows;

    // create all vertical lines of the grid
    for (int i = 1; i < snap_grid_.Columns; i++)
    {
        Vertex * line = new Vertex[2];

        line[0] = sf::Vertex(sf::Vector2f(i * snap_grid_.ColumnWidth, 0));
        line[1] = sf::Vertex(sf::Vector2f(i * snap_grid_.ColumnWidth, map->GetSize().y));

        snap_grid_.Lines.push_back(line);
    }
    // create all horizontal lines of snap grid
    for (int i = 1; i < snap_grid_.Rows; i++)
    {
        Vertex * line = new Vertex[2];

        line[0] = sf::Vertex(sf::Vector2f(0, i * snap_grid_.RowHeight));
        line[1] = sf::Vertex(sf::Vector2f(map->GetSize().x, i * snap_grid_.RowHeight));

        snap_grid_.Lines.push_back(line);
    }
}

/// display a given point on the map
Vector2f Engine::DrawPoint(Vector2f position)
{
    // convert it to units according to screen pixel to display ratio
    position *= float(Settings::SFMLRatio);
    // convert it to world coordinates
    position = this->mapPixelToCoords(Vector2i(position), view_);

    // check if a lane exists in the choosen location
    check_selection(position);

    Vector2f temp;
    if(snap_to_grid_)
    {
        temp = GetSnappedPoint(position);
    }
    else
    {
        temp = position;
    }

    click_point_ = CircleShape(snap_grid_.ColumnWidth / 3.f);
    click_point_.setOrigin(click_point_.getRadius(), click_point_.getRadius());
    click_point_.setFillColor(Color::Red);
    click_point_.setPosition(temp.x, temp.y);

    return temp;
}

/// generate a grid-snapped point with a given point
Vector2f Engine::GetSnappedPoint(Vector2f point)
{
    float x = 0, y = 0;

    if(int(point.x) % snap_grid_.ColumnWidth > snap_grid_.ColumnWidth / 2)
    {
        x = int(ceil(point.x / snap_grid_.ColumnWidth)) * snap_grid_.ColumnWidth;
    }
    else
    {
        x = int(floor(point.x / snap_grid_.ColumnWidth)) * snap_grid_.ColumnWidth;
    }

    if(int(point.y) % snap_grid_.RowHeight > snap_grid_.RowHeight / 2)
    {
        y = int(ceil(point.y / snap_grid_.RowHeight)) * snap_grid_.RowHeight;
    }
    else
    {
        y = int(floor(point.y / snap_grid_.RowHeight)) * snap_grid_.RowHeight;
    }

    return Vector2f(x,y);
}

/// check if a road was selected
void Engine::check_selection(Vector2f position)
{
    Lane * temp = map->CheckSelection(position);

    if(map->SelectedLane != nullptr)
    {
        map->SelectedLane->Unselect();
        map->SelectedLane = nullptr;
    }

    // if another lane is selected
    if(temp != nullptr)
    {
        map->SelectedLane = temp;
        map->SelectedLane->Select();
    }
}

/// get use input, and make changes accordingly
void Engine::input()
{
    // implementation of mouse dragging
    static bool dragging = false;
    static Vector2i startPos = Vector2i(0.0f, 0.0f);

    QPoint clickPoint = this->mapFromGlobal(QCursor::pos());
    Vector2i mousePos = Vector2i(clickPoint.x(), clickPoint.y());

    if (Mouse::isButtonPressed(sf::Mouse::Left) && this->getViewport(view_).contains(mousePos))
    {
        if (!dragging)
            startPos = mousePos;
        dragging = true;
    }
    else
    {
        if(dragging)
        {
            view_pos_ = t_view_pos_;
        }
        dragging = false;
    }

    if (dragging) {
        UpdateView(Vector2f(startPos.x - mousePos.x, startPos.y - mousePos.y));
    }

    if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
    {

    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        map->GetLane(1)->SetIsBlocked(true);
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        map->GetLane(1)->SetIsBlocked(false);
    }
}

/// build a map using instructions from a given json file
void Engine::LoadMap(string loadDirectory)
{
    // first, delete the old map.
    ResetMap();

    try
    {
        json j;
        // open the given file, read it to a json variable
        ifstream i(loadDirectory);
        i >> j;

        // build intersections
        for (auto data : j["intersections"]) {
            map->AddIntersection(data["id"], Vector2f(data["position"][0], data["position"][1]));
        }

        // build connecting roads
        for (auto data : j["connecting_roads"]) {
            map->AddConnectingRoad(data["id"], data["intersection_number"][0], data["intersection_number"][1]);
        }

        // build roads
        for (auto data : j["roads"]) {
            map->AddRoad(data["id"], data["intersection_number"], data["connection_side"], Settings::DefaultLaneLength);
        }

        for (auto data : j["lanes"]) {
            map->AddLane(data["id"], data["road_number"], data["is_in_road_direction"]);
        }

        for (auto data : j["routes"]) {
            map->AddRoute(data["from"], data["to"]);
        }

        cout << "Map has been successfully loaded from '" << loadDirectory << "'. " << endl;
    }
    catch(const std::exception& e)
    {
        cout << "Could not load map from this directory." << endl;
        cout << e.what() << endl;
    }
}

/// save the current map to a json file
void Engine::SaveMap(string saveDirectory)
{
    // first save intersections, then save connecting roads, then save roads, then save lanes
    json j;

    for(Intersection * inter : *map->GetIntersections())
    {
        j["intersections"].push_back(
                                      {
                                              {"id", inter->GetIntersectionNumber()},
                                              {"position", {inter->getPosition().x, inter->getPosition().y}}
                                      });
        for(Road * road : *inter->GetRoads())
        {
            // check if road is a connecting road
            if(!road->GetIsConnecting())
            {
                j["roads"].push_back(
                                     {
                                          {"id", road->GetRoadNumber()},
                                          {"intersection_number", road->GetIntersectionNumber()},
                                          {"connection_side", road->GetConnectionSide()}
                                      });

                for (Lane * lane : *road->GetLanes())
                {
                    j["lanes"].push_back(
                            {
                                    {"id", lane->GetRoadNumber()},
                                    {"road_number", lane->GetRoadNumber()},
                                    {"is_in_road_direction", lane->GetIsInRoadDirection()}
                            });
                }
            }
            else
            {
                // only save the connecting road once for the connected intersection
                if(inter->GetIntersectionNumber() == road->GetIntersectionNumber(0))
                {
                    j["connecting_roads"].push_back(
                            {
                                    {"id",                  road->GetRoadNumber()},
                                    {"intersection_number", {road->GetIntersectionNumber(
                                            0), road->GetIntersectionNumber(1)}}
                            });

                    for (Lane * lane : *road->GetLanes())
                    {
                        j["lanes"].push_back(
                                {
                                        {"id", lane->GetRoadNumber()},
                                        {"road_number", lane->GetRoadNumber()},
                                        {"is_in_road_direction", lane->GetIsInRoadDirection()}
                                });
                    }
                }
            }
        }
    }

    for(Route * route : *map->GetRoutes())
    {
        j["routes"].push_back(
                {
                        {"from", route->FromLane->GetLaneNumber()},
                        {"to", route->ToLane->GetLaneNumber()}
                }
        );
    }

    // write to file
    ofstream o(saveDirectory);
    o << setw(4) << j << endl;
    o.close();

    cout << "Map saved to '" << saveDirectory << "' successfully." << endl;
}

/// reset the whole map, delete everything
void Engine::ResetMap()
{
    cout << "Deleting Vehicles..." << endl;
    Vehicle::DeleteAllVehicles();

    cout << "Resetting Map..." << endl;
    delete map;
    map = new Map(0, Vector2i(this->width()/2, this->height()/2), Settings::DefaultMapWidth, Settings::DefaultMapWidth);

    cout << "*** Map has been reset. ***" << endl;
}

/// do the game cycle (input->update->draw)
void Engine::cycle()
{
    input();
    update((timer_.interval() / 1000.f));
    render();
}

/// update all the engine's objects
void Engine::update(float elapsedTime)
{
    map->Update(elapsedTime);

    for (Vehicle *v : Vehicle::ActiveVehicles)
    {
        v->Update(elapsedTime);
    }

    if(Settings::DrawFps)cout << "FPS : " << 1.f/elapsedTime << endl;
    //clear all cars to be deleted
    Vehicle::ClearVehicles();
}

/// add a vehicle at a random track
bool Engine::AddVehicleRandomly()
{
    // find a random starting point
    Lane * l = map->GetPossibleStartingLane();
    if(l == nullptr)
    {
        cout << "no starting lanes available." << endl;
        return false;
    }
    // find a starting route from starting lane
    Route * r = map->GetPossibleRoute(l->GetLaneNumber());

    if(r == nullptr)
    {
        cout << "no routes available. please add them to the map" << endl;
        return false;
    }

    // while new routes to append are available
    // new routes will be searched starting from the previous route end
    queue<Lane*> * tempQueue = new queue<Lane*>();;
    Lane * lastLane;

    while(r != nullptr)
    {
        tempQueue->push(r->FromLane);
        lastLane = r->ToLane;
        r = map->GetPossibleRoute(r->ToLane->GetLaneNumber());
    }
    tempQueue->push(lastLane);


    return (Vehicle::AddVehicle(tempQueue, this->map) != nullptr);
}

/// render the engine's objects
void Engine::render()
{
    // Clean out the last frame
    clear(BackgroundColor);
 
    // Draw the map
    this->map->Draw(this);

    // Draw all vehicles
    for(Vehicle * v : Vehicle::ActiveVehicles)
    {
        v->Draw(this);
    }

    // Draw the click index
    this->draw(this->click_point_);

    // Draw the grid
    if(Settings::DrawGrid)
    {
        for (Vertex *l : this->snap_grid_.Lines) {
            this->draw(l, 2, Lines);
        }
    }

    // draw the minimap
    this->setView(minimap_); // switch to minimap for rendering
    draw_minimap(); // render minimap
    this->setView(view_); // switch back to main view

}

/// drawing the minimap is drawing everything but the vehicles and the grid, on a smaller scale
void Engine::draw_minimap()
{
    // Draw the minimap's background
    this->draw(minimap_bg_);

    // Draw the map
    this->map->Draw(this);

    // Draw the click index
    this->draw(this->click_point_);

    // Draw the shown area index
    this->draw(shown_area_index_);
}







