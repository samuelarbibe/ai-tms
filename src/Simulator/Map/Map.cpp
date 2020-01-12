//
// Created by Samuel Arbibe on 28/12/2019.
//


#include "Map.hpp"

Map::Map(int mapNumber, Vector2f position, int width, int height)
{
    if(mapNumber == 0)
    {
        mapNumber = 1;
    }
    m_mapNumber = mapNumber;
    m_position = position;
    m_width    = width;
    m_height   = height;

    m_numberOfIntersections = 0;
}

/// add an intersection to the map
Intersection * Map::AddIntersection(int intersectionNumber, Vector2f position,
                                   WeatherCondition weatherCondition) {
    if(!intersectionNumber)
    {
        intersectionNumber = Intersection::IntersectionCount + 1;
    }

    m_intersections.push_back(new Intersection(position, intersectionNumber, weatherCondition));

    m_numberOfIntersections++;
    Intersection::IntersectionCount++;

    if(DRAW_ADDED)std::cout << "Intersection " << intersectionNumber << " added" << endl;

    return m_intersections[m_numberOfIntersections-1];
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

    if (tempRoad)
    {
        temp = GetIntersection(tempRoad->GetIntersectionNumber());
    }

    if(temp)
    {
        return temp->AddLane(laneNumber, roadNumber, isInRoadDirection);
    }

    return nullptr;
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
    if(((int)inter1->getPosition().x != (int)inter2->getPosition().x && (int)inter1->getPosition().y != (int)inter2->getPosition().y)
            || ((int)inter1->getPosition().x == (int)inter2->getPosition().x && (int)inter1->getPosition().y == (int)inter2->getPosition().y))
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

    for (int i = 0; i < m_numberOfIntersections; i++)
    {
        if ((temp = m_intersections[i])->GetIntersectionNumber() == intersectionNumber)
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

    for (int i = 0; i < m_numberOfIntersections; i++)
    {
        if ((temp = m_intersections[i]->GetRoad(roadNumber)) != nullptr)
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

    for (int i = 0; i < m_numberOfIntersections; i++)
    {
        if ((temp = m_intersections[i]->GetLane(laneNumber)) != nullptr)
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

/*
/// re-locate 2 connected intersections, to fit the map
bool Map::reAssignIntersectionPositions(Intersection * intersection1, Intersection * intersection2 ,int connectionSide1, int connectionSide2)
{
    Vector2f pos1, pos2;

    if(abs(connectionSide1 - connectionSide2) != 2)
    {
        cerr << "requested connection not possible in current configuration" << endl;
        return false;
    }

    if(connectionSide1 > connectionSide2)
    {
        if(connectionSide1 == 3)
        {
            // 1 above, 2 below
            pos1 = Vector2f(m_width/2, m_height/4);
            pos2 = Vector2f(m_width/2, m_height/4 * 3);
        }
        else
        {
            // 1 right, 2 left
            pos1 = Vector2f(m_width/4 * 3, m_height/2);
            pos2 = Vector2f(m_width/4, m_height/2);
        }
    }
    else
    {
        if(connectionSide1 == 1)
        {
            // 1 below, 2 above
            pos2 = Vector2f(m_width/2, m_height/4);
            pos1 = Vector2f(m_width/2, m_height/4 * 3);
        }
        else
        {
            // 1 left, 2 right
            pos2 = Vector2f(m_width/4 * 3, m_height/2);
            pos1 = Vector2f(m_width/4, m_height/2);
        }
    }

    intersection1->reAssignIntersectionPosition(pos1);
    intersection2->reAssignIntersectionPosition(pos2);

    return true;
}
*/

/// update, for future use
void Map::Update(float elapsedTime)
{

}

/// draw the map, and all of its belongings
void Map::Draw(RenderWindow * window)
{
    for (int i = 0 ; i < m_numberOfIntersections; i++)
    {
        m_intersections[i]->Draw(window);
    }
}




