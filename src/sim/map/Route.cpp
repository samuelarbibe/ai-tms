//
// Created by Samuel Arbibe on 16/02/2020.
//

#include "Route.hpp"

int Route::RouteCount{0};

Route::Route(Lane *from, Lane *to)
{
    route_number_ = ++RouteCount;
    selected_ = false;

    FromLane = from;
    FromLane = from;
    ToLane = to;

    lines_.push_back(new Vertex[2]);
    lines_.push_back(new Vertex[2]);

	ReloadRoute();
}

Route::~Route()
{
    lines_.clear();
}

/// build the green lane lines
void Route::BuildLaneLines()
{

	lines_[0][0] = Vertex(FromLane->GetStartPosition());
	lines_[0][0].color = Color::Green;
	lines_[0][1] = sf::Vertex(FromLane->GetEndPosition());
	lines_[0][1].color = Color::Green;

	lines_[1][0] = Vertex(ToLane->GetStartPosition());
	lines_[1][0].color = Color::Green;
	lines_[1][1]= sf::Vertex(ToLane->GetEndPosition());
	lines_[1][1].color = Color::Green;

}

/// build the green radius lines in intersections
void Route::BuildRadiusLine()
{
	radius_line_.clear();

	
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
        Vector2f radiusVec = t.transformPoint(Settings::BaseVec);
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

/// reload this route
void Route::ReloadRoute()
{
	BuildLaneLines();
	BuildRadiusLine();
}

/// draw
void Route::Draw(RenderWindow *window)
{
	if(Settings::DrawRoutes || selected_)
	{
		window->draw(radius_line_);

		for (Vertex *l : lines_)
		{
			window->draw(l, 2, Lines);
		}
	}
}





