//
// Created by Samuel Arbibe on 16/02/2020.
//

#include "Route.hpp"

const Vector2f kForwardVec{0.f, -1.f};
int Route::RouteCount{0};

Route::Route(Lane *from, Lane *to)
{
    route_number_ = ++RouteCount;

    FromLane = from;
    FromLane = from;
    ToLane = to;

    Vertex * fromLine = new Vertex[2];
    fromLine[0] = sf::Vertex(FromLane->GetStartPosition());
    fromLine[0].color = Color::Green;
    fromLine[1] = sf::Vertex(FromLane->GetEndPosition());
    fromLine[1].color = Color::Green;

    Vertex * toLine = new Vertex[2];
    toLine[0] = sf::Vertex(ToLane->GetStartPosition());
    toLine[0].color = Color::Green;
    toLine[1] = sf::Vertex(ToLane->GetEndPosition());
    toLine[1].color = Color::Green;

    lines_.push_back(fromLine);
    lines_.push_back(toLine);

    BuildRadiusLine();
}

Route::~Route()
{
    lines_.clear();
}

void Route::BuildRadiusLine()
{
    Vector2f startPos = FromLane->GetEndPosition();
    Vector2f endPos = ToLane->GetStartPosition();

    float distanceSourceTarget = Settings::CalculateDistance(startPos, endPos);
    float angle = Settings::CalculateAngle(FromLane->GetDirection(), ToLane->GetDirection());
    float radius = (distanceSourceTarget / 2) / (sin(angle * M_PI / 360.f));


    Transform t;

    t.rotate(FromLane->GetDirection() + 90);
    t.scale(radius, radius);

    // if straight line
    if(angle < 1 && angle > -1)
    {
        radius_line_ = VertexArray(LinesStrip, 2);
        radius_line_[0].position = startPos;
        radius_line_[0].color = Color::Green;
        radius_line_[1].position = endPos;
        radius_line_[1].color = Color::Green;
    }
    else
    {
        Vector2f radiusVec = t.transformPoint(kForwardVec);
        Vector2f circleCenter = startPos + radiusVec;

        // a strip [alpha] of lines, making a quarter of a circle
        radius_line_ = VertexArray(LinesStrip, abs(angle));

        Transform cycle;
        cycle.rotate(angle / abs(angle));

        Vector2f newPos;

        for(int i = 0; i < int(abs(angle)); i++)
        {
            newPos = circleCenter - radiusVec;
            radius_line_[i].position = newPos;
            radius_line_[i].color = Color::Green;
            radiusVec = cycle.transformPoint(radiusVec);
        }
    }
}

void Route::Draw(RenderWindow *window)
{
    window->draw(radius_line_);

    for (Vertex *l : lines_) {
        window->draw(l, 2, Lines);
    }
}




