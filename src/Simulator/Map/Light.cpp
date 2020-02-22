//
// Created by Samuel Arbibe on 20/02/2020.
//

#include "Light.hpp"

int Light::LightCount = 0;

Light::Light(int lightNumber, int phaseNumber, Road * parentRoad)
{
    parent_road_ = parentRoad;
    phase_number_ = phaseNumber;
    light_number_ = lightNumber;
    state_ = RED;

    UpdatePosition();

    data_box_ = new DataBox(this->getPosition());
    data_box_->AddData("ID", light_number_);
    data_box_->AddData("Phase", phase_number_);
    data_box_->AddData("State", state_);
}

Light::~Light()
{
	delete data_box_;

    if(Settings::DrawDelete) cout << "Light " << light_number_ << " deleted" << endl;
}

void Light::UpdatePosition()
{
    Vector2f roadPos = parent_road_->GetStartPosition();
    float direction = parent_road_->GetRoadDirection();

    Transform t;
    t.rotate(direction - 90);
    float margin = parent_road_->GetWidth() / 2 + 30;
    t.scale(margin, margin);
    Vector2f sideVector = t.transformPoint(Settings::ForwardVector);

    t = Transform();
    t.rotate(direction);
    t.scale(20, 20);
    Vector2f marginVector = t.transformPoint(Settings::ForwardVector);
    Vector2f position = roadPos + sideVector + marginVector;

    direction -= 180;

    this->setSize(Vector2f(40, 100));
    this->setOrigin(getSize().x / 2,0);
    this->setPosition(position);
    this->setFillColor(Color::Black);
    this->setRotation(direction);

    sf::CircleShape * circlePtr = new CircleShape();
    float radius = this->getSize().x/2 - 7;

    Vector2f basePos = this->getPosition();

    t = Transform();
    t.rotate(direction-180);
    // set axis relative to basePos and direction
    Vector2f yDir = t.transformPoint(Settings::ForwardVector);

    // set location relative to new axis
    Vector2f yMargin = Vector2f(radius * 2 + 7, radius * 2 + 7);
    t = Transform();
    t.scale(yMargin);
    yMargin = t.transformPoint(yDir);

    t = Transform();
    t.scale(0.5, 0.5);

    Vector2f first = basePos + t.transformPoint(yMargin);

    circles_.clear();

    circlePtr->setOrigin(radius, radius);
    circlePtr->setFillColor(Color::Black);
    circlePtr->setPosition(first);
    circlePtr->setRadius(radius);
    circles_.emplace_back(circlePtr);

    circlePtr = new CircleShape();
    circlePtr->setOrigin(radius, radius);
    circlePtr->setFillColor(Color::Black);
    circlePtr->setPosition(first + yMargin);
    circlePtr->setRadius(radius);
    circles_.emplace_back(circlePtr);

    circlePtr = new CircleShape();
    circlePtr->setOrigin(radius, radius);
    circlePtr->setFillColor(Color::Black);
    circlePtr->setPosition(first + yMargin + yMargin);
    circlePtr->setRadius(radius);
    circles_.emplace_back(circlePtr);
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

    if(Settings::DrawLightDataBoxes)data_box_->SetData("State", state_);
}

void Light::Draw(RenderWindow *window)
{
    window->draw(*this);

    for(int i = 0; i < 3; i++)
    {
        window->draw(*circles_[i]);
    }

    if(Settings::DrawLightDataBoxes)data_box_->Draw(window);
}




