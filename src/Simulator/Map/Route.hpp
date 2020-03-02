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

    Lane * FromLane;
    Lane * ToLane;

    Route(Lane * from, Lane * to);
    ~Route();

    void Draw(RenderWindow * window);
    void ReloadRoute();

    // get
    int GetRouteNumber(){return route_number_;}

    // set
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
