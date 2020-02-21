//
// Created by Samuel Arbibe on 20/02/2020.
//

#ifndef SIMULATORSFML_LIGHT_HPP
#define SIMULATORSFML_LIGHT_HPP

#include <SFML/Graphics.hpp>
#include "Road.hpp"

using namespace sf;
using namespace std;

enum LightState {GREEN, ORANGE, RED};

class Light : public RectangleShape
{
public:
    Light(int lightNumber, int phaseNumber, Road * parentRoad);
    ~Light();

    void Draw(RenderWindow * window);
    void Update(float elapsedTime);

    void SetState(LightState state){state_ = state;}
	void UpdatePosition();

    int GetPhaseNumber(){return phase_number_;}
    int GetLightNumber(){return light_number_;}

    Road * GetParentRoad(){return parent_road_;}

    static int LightCount;
private:
    int light_number_;
    int phase_number_;

    LightState state_;
    Road * parent_road_;

    vector<CircleShape*> circles_;
};


#endif //SIMULATORSFML_LIGHT_HPP
