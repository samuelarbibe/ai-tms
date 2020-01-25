//
// Created by Samuel Arbibe on 21/01/2020.
//

#include "Settings.hpp"

bool Settings::DrawFps = false;
bool Settings::DrawActive = true;
bool Settings::DrawDelete = false;
bool Settings::DrawAdded = true;

int Settings::MaxFps = 60;
bool Settings::MultiColor = true;
float Settings::MinDistanceFromNextCar = 180;
float Settings::MinDistanceFromStop = 65;
bool Settings::AccWhileTurning = true;

float Settings::MinLaneWidth = 83;
float Settings::LaneWidth = 100; // lane width in px.
float Settings::MaxLaneWidth = 107;
float Settings::Scale = 3; // 1 px = [scale] * 1 cm
float Settings::Speed = 7; // running speed

float Settings::DefaultLaneLength = 500; // lane length in px

int Settings::GridColumns = 50;
int Settings::GridRows = 50;

int Settings::SFMLRatio = 1;

int Settings::DefaultMapWidth = 3000;
int Settings::DefaultMapHeight = 3000;

// camera setting
float Settings::Zoom = 0.5f;

// minimap Settings
float Settings::MinimapSize = 120.f;
float Settings::MinimapMargin = 10.f;

// [LaneWidth in px] * Scale * unitScale = laneWidth in Unit
// M, CM, Feet, Inch
float Settings::UnitScales[5]{0.01, 1, 0.0328, 0.3937, 1/Scale};

// [LaneWidth in px] * Scale * unitScale = laneWidth in Unit
float Settings::GetLaneWidthAs(Units unit)
{
    // base
    float len = Settings::LaneWidth * Settings::Scale;

    return len * UnitScales[int(unit)];
}

// convert a given value from a unit to another unit
float Settings::ConvertSize(Units fromUnit, Units toUnit, float value)
{
    // first, convert value to px
    // LaneWidth in px = laneWidth in unit / Scale / unitScale
    float valueInPx =  value / Scale / UnitScales[int(fromUnit)];

    // convert it to the target unit
    return valueInPx * Scale * UnitScales[int(toUnit)];
}