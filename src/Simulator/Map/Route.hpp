//
// Created by Samuel Arbibe on 16/02/2020.
//

#ifndef SIMULATORSFML_ROUTE_HPP
#define SIMULATORSFML_ROUTE_HPP

#include <list>

#include "Lane.hpp"

class Route
{
public:
    Route(Lane * from, Lane * to);
    ~Route();

    Lane * FromLane;
    Lane * ToLane;

    void Draw(RenderWindow * window);
    int GetRouteNumber(){return route_number_;}
    void ReloadRoute();
    void SetSelected(bool selected){selected_ = selected;}

    static int RouteCount;
private:
    void BuildRadiusLine();
	void BuildLaneLines();

	bool selected_;

    int route_number_;

    vector<Vertex*> lines_;
    VertexArray radius_line_;
};


#endif //SIMULATORSFML_ROUTE_HPP
