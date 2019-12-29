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

Intersection * Map::AddIntersection(int intersectionNumber, Vector2f position,
                                   WeatherCondition weatherCondition) {
    if(!intersectionNumber)
    {
        intersectionNumber = IntersectionCount + 1;
    }

    //TODO: add position finding algorithm

    m_intersections.push_back(new Intersection(position, intersectionNumber, weatherCondition));

    m_numberOfIntersections++;
    IntersectionCount++;

    if(DRAW_ADDED)std::cout << "Intersection " << intersectionNumber << " added" << endl;

    return m_intersections[m_numberOfIntersections-1];
}

Road * Map::AddRoad(int roadNumber, int intersectionNumber, int connectionSide, float length)
{
    Intersection * temp = GetIntersection(intersectionNumber);

    if(temp)
    {
        return temp->AddRoad(roadNumber, connectionSide, length);
    }

    return nullptr;
}

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

Road * Map::AddConnectingRoad(int roadNumber, int intersectionNumber1, int intersectionNumber2 ,int connectionSide1, int connectionSide2)
{
    Intersection * inter1 = GetIntersection(intersectionNumber1);
    Intersection * inter2 = GetIntersection(intersectionNumber2);

    if(inter1 == nullptr || inter2 == nullptr)
    {
        if(DRAW_ADDED)cerr << "one of the given intersections was not found..." << endl;
        return nullptr;
    }

    if(!reAssignIntersectionPositions(inter1, inter2 ,connectionSide1, connectionSide2))
    {
        cerr << "Connection Failed..." << endl;
        return nullptr;
    }

    return inter1->AddConnectingRoad(roadNumber, connectionSide1, connectionSide2, inter2);
}

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

void Map::Update(float elapsedTime)
{

}

void Map::Draw(RenderWindow * window)
{
    for (int i = 0 ; i < m_numberOfIntersections; i++)
    {
        m_intersections[i]->Draw(window);
    }
}


