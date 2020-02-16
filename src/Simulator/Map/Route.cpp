//
// Created by Samuel Arbibe on 16/02/2020.
//

#include "Route.hpp"

const Vector2f m_forwardVec{0.f, -1.f};

Route::Route(Lane *from, Lane *to)
{
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
    int angle = -(FromLane->GetDirection() - ToLane->GetDirection());
    float radius = (distanceSourceTarget / 2.f) / (sin(angle * M_PI / 360.f));

    Transform t;
    if(angle > 180)
    {
        t.rotate(FromLane->GetDirection() - 90);
    }
    else
    {
        t.rotate(FromLane->GetDirection() + 90);
    }
    t.scale(radius, radius);


    Vector2f radiusVec = t.transformPoint(m_forwardVec);
    Vector2f circleCenter = startPos + radiusVec;

    // a strip [alpha] of lines, making a quarter of a circle
    radius_line_ = VertexArray(LinesStrip, abs(angle));

    Transform cycle;
    Vector2f addVec = radiusVec;
    cycle.rotate(abs(angle) / angle);

    Vector2f newPos;

    for(int i = 0; i < abs(angle); i++)
    {
        newPos = circleCenter - addVec;
        radius_line_[i].position = newPos;
        radius_line_[i].color = Color::Green;
        addVec = cycle.transformPoint(addVec);
    }
}

void Route::Draw(RenderWindow *window)
{
    window->draw(radius_line_);

    for (Vertex *l : lines_) {
        window->draw(l, 2, Lines);
    }
}




