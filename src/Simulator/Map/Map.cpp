//
// Created by Samuel Arbibe on 28/12/2019.
//

#include <QStringList>
#include "Map.hpp"

Map::Map(int mapNumber, Vector2i position, int width, int height)
{
    if(mapNumber == 0)
    {
        mapNumber = 1;
    }
    map_number_ = mapNumber;
    position_ = position;
    width_    = width;
    height_   = height;
    SelectedLane = nullptr;

    number_of_intersections_ = 0;
}

Map::~Map()
{
    for (Intersection * inter : intersections_)
    {
        delete inter;
    }

    if(Settings::DrawDelete)cout << "Map " << map_number_ << " deleted" << endl;
}

/// add an intersection to the map
Intersection * Map::AddIntersection(int intersectionNumber, Vector2f position)
{
    if(!intersectionNumber)
    {
        intersectionNumber = Intersection::IntersectionCount + 1;
    }

    intersections_.push_back(new Intersection(position, intersectionNumber));

    number_of_intersections_++;
    Intersection::IntersectionCount++;

    if(Settings::DrawAdded)std::cout << "Intersection " << intersectionNumber << " added" << endl;

    return intersections_[number_of_intersections_ - 1];
}

/// add a road to a specific intersection
Road * Map::AddRoad(int roadNumber, int intersectionNumber, int connectionSide, float length)
{
    Intersection * temp = GetIntersection(intersectionNumber);

    if(temp)
    {
        return temp->AddRoad(roadNumber, connectionSide, length);
    }

    return nullptr;
}

/// add a road to a specific road
Lane * Map::AddLane(int laneNumber, int roadNumber, bool isInRoadDirection)
{
    Road *tempRoad = GetRoad(roadNumber);
    Intersection *temp = nullptr;
    Lane * tempLane = nullptr;

    if (tempRoad)
    {
        temp = GetIntersection(tempRoad->GetIntersectionNumber());
    }

    if(temp)
    {
        tempLane = temp->AddLane(laneNumber, roadNumber, isInRoadDirection);
    }

    this->ReloadMap();

    return tempLane;
}

/// add a road connecting between two intersections
Road * Map::AddConnectingRoad(int roadNumber, int intersectionNumber1, int intersectionNumber2)
{
    Intersection * inter1 = GetIntersection(intersectionNumber1);
    Intersection * inter2 = GetIntersection(intersectionNumber2);

    if(inter1 == nullptr || inter2 == nullptr)
    {
        cerr << "one of the given intersections was not found..." << endl;
        return nullptr;
    }

    // if intersection do not align on one of the axis, return error
    if((int(inter1->getPosition().x) != int(inter2->getPosition().x) && int(inter1->getPosition().y) != int(inter2->getPosition().y))
            || (int(inter1->getPosition().x) == int(inter2->getPosition().x) && int(inter1->getPosition().y) == int(inter2->getPosition().y)))
    {
        cerr << "the intersections must align on one of the axis" << endl;
        return nullptr;
    }
    /*
    if(!reAssignIntersectionPositions(inter1, inter2 ,connectionSide1, connectionSide2))
    {
        cerr << "Connection Failed..." << endl;
        return nullptr;
    }
    */
    pair<ConnectionSides, ConnectionSides> connections;
    connections = AssignConnectionSides(inter1->getPosition(), inter2->getPosition());

    return inter1->AddConnectingRoad(roadNumber, connections.first, connections.second, inter2);
}

/// get intersection by intersectionNumber
Intersection * Map::GetIntersection(int intersectionNumber)
{
    Intersection * temp;

    for (int i = 0; i < number_of_intersections_; i++)
    {
        if ((temp = intersections_[i])->GetIntersectionNumber() == intersectionNumber)
        {
            return temp;
        }
    }

    cout << "error : intersection not found in Map..." << endl;

    return nullptr;
}

/// get intersection by lane number
Intersection * Map::GetIntersectionByLaneNumber(int laneNumber) {

    Lane * l = this->GetLane(laneNumber);
    Road * r = this->GetRoad(l->GetRoadNumber());
    return this->GetIntersection(r->GetIntersectionNumber(0));
}

/// get road by roadNumber
Road * Map::GetRoad(int roadNumber)
{
    Road * temp;

    for (int i = 0; i < number_of_intersections_; i++)
    {
        if ((temp = intersections_[i]->GetRoad(roadNumber)) != nullptr)
        {
            return temp;
        }
    }

    cout << "error : road not found in map..." << endl;

    return nullptr;
}

/// get lane by lane number
Lane * Map::GetLane(int laneNumber)
{
    Lane * temp;

    for (int i = 0; i < number_of_intersections_; i++)
    {
        if ((temp = intersections_[i]->GetLane(laneNumber)) != nullptr)
        {
            return temp;
        }
    }

    cout << "error : lane not found in map..." << endl;

    return nullptr;
}

/// set 2 relative connection sides, by intersection positions
pair<ConnectionSides, ConnectionSides> Map::AssignConnectionSides( Vector2f pos1, Vector2f pos2)
{
    ConnectionSides con1, con2;
    if(pos1.x > pos2.x)
    {
        con1 = ConnectionSides::LEFT;
        con2 = ConnectionSides::RIGHT;
    }
    else if(pos1.x < pos2.x)
    {
        con1 = ConnectionSides::RIGHT;
        con2 = ConnectionSides::LEFT;
    }
    else if(pos1.y > pos2.y)
    {
        con1 = ConnectionSides::UP;
        con2 = ConnectionSides::DOWN;
    }
    else if(pos1.y < pos2.y)
    {
        con1 = ConnectionSides::DOWN;
        con2 = ConnectionSides::UP;
    }

    pair<ConnectionSides, ConnectionSides> connections(con1, con2);
    return connections;
}

Lane * Map::CheckSelection(Vector2f position)
{
    // for each intersection in map
    Lane * temp;
    for(Intersection * inter : intersections_)
    {
        // if selection found
        temp = inter->CheckSelection(position);
        if(temp != nullptr) return temp;
    }
    return nullptr;
}

/// Reload all intersection in this map
void Map::ReloadMap()
{
    // disable delete output temporarily for reload
    for (Intersection *i : intersections_)
    {
        i->ReloadIntersection();
    }
}

/// update, for future use
void Map::Update(float elapsedTime)
{
    for(Intersection * i : intersections_)
    {
        i->Update(elapsedTime);
    }
}

int Map::GetRoadCount()
{
    int sum = 0;

    for(Intersection * inter : intersections_)
    {
        sum += inter->GetRoadCount();
    }

    return sum;
}

int Map::GetLaneCount()
{
    int sum = 0;

    for(Intersection * inter : intersections_)
    {
        sum += inter->GetLaneCount();
    }

    return sum;
}

bool Map::DeleteLane(int laneNumber)
{
    Intersection * targetIntersection = GetIntersectionByLaneNumber(laneNumber);

    if(targetIntersection != nullptr)
    {
        // delete the given lane
        targetIntersection->DeleteLane(laneNumber);

        // if intersection has no roads left, delete it as well
        if(targetIntersection->GetRoadCount() == 0)
        {
            auto it = find(intersections_.begin(), intersections_.end(), targetIntersection);
            intersections_.erase(it);
            number_of_intersections_ --;
        }

        // set selected as nullptr
        this->SelectedLane = nullptr;

        // reload the map to display the changes
        this->ReloadMap();

        return true;
    }
    return false;
}

/// draw the map, and all of its belongings
void Map::Draw(RenderWindow * window)
{
    // Draw all intersections
    for (int i = 0 ; i < number_of_intersections_; i++)
    {
        intersections_[i]->Draw(window);
    }
}

QStringList Map::GetLaneIdList()
{
    QStringList idList = QStringList();
    for(Intersection * inter : intersections_)
    {
        for(Road * road : *inter->GetRoads())
        {
            for (Lane * lane : *road->GetLanes())
            {
                idList.append(QString::number(lane->GetLaneNumber()));
            }
        }
    }

    return idList;
}

QStringList Map::GetRoadIdList()
{
    QStringList idList = QStringList();
    for(Intersection * inter : intersections_)
    {
        for(Road * road : *inter->GetRoads())
        {
            idList.append(QString::number(road->GetRoadNumber()));
        }
    }

    return idList;
}

QStringList Map::GetIntersectionIdList()
{
    QStringList idList = QStringList();
    for(Intersection * inter : intersections_)
    {
        idList.append(QString::number(inter->GetIntersectionNumber()));
    }

    return idList;
}




