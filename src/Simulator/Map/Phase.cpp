//
// Created by Samuel Arbibe on 20/02/2020.
//

#include "Phase.hpp"

int Phase::PhaseCount = 0;

Phase::Phase(int phaseNumber, float cycleTime)
{
    number_of_lights_ = 0;
    phase_number_ = phaseNumber;
    cycle_time_ = cycleTime;
    open_ = false;
    open_time_ = 0;
    state_ = RED;
}

Phase::~Phase()
{
    for(Light * light : lights_)
    {
        delete light;
    }

    if(Settings::DrawDelete)cout << "Phase " << phase_number_ << " deleted." << endl;
}

Light *Phase::AddLight(int lightNumber, Road * parentRoad)
{
    if(lightNumber == 0)
    {
        lightNumber = Light::LightCount + 1;
    }

    Light * temp = new Light(lightNumber, phase_number_, parentRoad);
    lights_.push_back(temp);

    ++Light::LightCount;
    ++number_of_lights_;

    return temp;
}

void Phase::AddLane(Lane *lane)
{
    lanes_.push_back(lane);
}

void Phase::ReloadPhase()
{
    for(Light * l : lights_)
    {
        l->UpdatePosition();
    }
}

void Phase::Update(float elapsedTime)
{
    if(open_)
    {
        open_time_ += elapsedTime * Settings::Speed;

        if (open_time_ > cycle_time_)
        {
            open_time_ = 0;
            open_ = false;
            state_ = RED;
        }
        else if (open_time_ < Settings::OrangeDelay)
        {
            state_ = ORANGE;
        }
        else
        {
            state_ = GREEN;
        }
    }

    for(Light * l : lights_)
    {
        l->SetState(state_);
        l->Update(elapsedTime);
    }

    for(Lane * l : lanes_)
    {
        if(state_ == GREEN)
        {
            l->SetIsBlocked(false);
        }
        else
        {
            l->SetIsBlocked(true);
        }
    }
}

void Phase::Draw(RenderWindow *window)
{
    for(Light * l : lights_)
    {
        l->Draw(window);
    }
}



