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
    Light(int lightNumber, Vector2f position);
    ~Light();

    void Draw(RenderWindow * window);
    void Update(float elapsedTime);

    void SetState(LightState state){state_ = state;}
    int GetLightNumber(){return light_number_;}


    static int LightCount;
private:
    int light_number_;
    LightState state_;

    vector<CircleShape*> circles_;
};


#endif //SIMULATORSFML_LIGHT_HPP
