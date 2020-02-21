//
// Created by Samuel Arbibe on 20/02/2020.
//

#include "Light.hpp"

int Light::LightCount = 0;

Light::Light(int lightNumber, Vector2f position)
{
    light_number_ = lightNumber;
    state_ = RED;

    this->setPosition(position);
    this->setSize(Vector2f(40, 100));
    this->setOrigin(0,0);
    this->setFillColor(Color::Black);

    sf::CircleShape * circlePtr = new CircleShape();
    float radius = this->getSize().x/2 - 7;

    Vector2f basePos = this->getPosition();
    Vector2f xMargin = Vector2f(getSize().x/2 - radius, 0);
    Vector2f yMargin = Vector2f(0, radius * 2 + 7);

    Vector2f first = basePos + xMargin + Vector2f(0, circlePtr->getRadius()/2 + 5);

    circlePtr->setFillColor(Color::Black);
    circlePtr->setPosition(first);
    circlePtr->setRadius(radius);
    circles_.emplace_back(circlePtr);

    circlePtr = new CircleShape();
    circlePtr->setFillColor(Color::Black);
    circlePtr->setPosition(first + yMargin);
    circlePtr->setRadius(radius);
    circles_.emplace_back(circlePtr);

    circlePtr = new CircleShape();
    circlePtr->setFillColor(Color::Black);
    circlePtr->setPosition(first + yMargin + yMargin);
    circlePtr->setRadius(radius);
    circles_.emplace_back(circlePtr);
}

Light::~Light()
{

}

void Light::Update(float elapsedTime)
{
    circles_[0]->setFillColor(Color::Black);
    circles_[1]->setFillColor(Color::Black);
    circles_[2]->setFillColor(Color::Black);

    switch(state_)
    {
        case RED:
            circles_[0]->setFillColor(Color::Red);
            break;
        case ORANGE:
            circles_[1]->setFillColor(Color::Yellow);
            break;
        case GREEN:
            circles_[2]->setFillColor(Color::Green);
            break;
    }
}

void Light::Draw(RenderWindow *window)
{
    window->draw(*this);

    for(int i = 0; i < 3; i++)
    {
        window->draw(*circles_[i]);
    }
}




