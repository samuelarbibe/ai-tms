//
// Created by Samuel Arbibe on 21/01/2020.
//

#include "Settings.hpp"

bool Settings::DrawFps = false;
bool Settings::DrawActive = false;
bool Settings::DrawDelete = false;
bool Settings::DrawAdded = false;

int Settings::MaxFps = 200;
bool Settings::MultiColor = true;
float Settings::MinDistanceFromNextCar = 166;
float Settings::MinDistanceFromStop = 66;
bool Settings::AccWhileTurning = true;

float Settings::MinLaneWidth = 83;
float Settings::LaneWidth = 100; // lane width in px.
float Settings::MaxLaneWidth = 107;
float Settings::Scale = 3; // 1 px * [scale] = 1 cm
float Settings::Speed = 80; // running speed

float Settings::DefaultLaneLength = 500; // lane length in px

int Settings::DefaultMapWidth = 3000;
int Settings::DefaultMapHeight = 3000;

int Settings::GridColumns = 50;
int Settings::GridRows = 50;

int Settings::SFMLRatio = 1;

bool Settings::MapOverflow = false;

// camera setting
float Settings::Zoom = 0.5f;

// minimap Settings
float Settings::MinimapSize = 120.f;
float Settings::MinimapMargin = 10.f;

// [LaneWidth in px] * Scale * unitScale = laneWidth in Unit
// M, CM, Feet, Inch
float Settings::DistanceUnitScales[5]
{
    0.01, 1, 0.0328, 0.3937, 1/Scale
};

// [Speed in px/s] * Scale * unitScale = Speed in Unit
// that means that [speed in cm/s] * unitScale = Speed in Unit
// CM/S, KM/H, M/S, MPH, PX/S
float Settings::VelocityUnitScales[5]
{
    1, 0.036, 0.01, 0.022, 1/Scale
};


// [LaneWidth in px] * Scale * unitScale = laneWidth in Unit
float Settings::GetLaneWidthAs(DistanceUnits unit)
{
    // base
    float len = Settings::LaneWidth * Settings::Scale;

    return len * DistanceUnitScales[int(unit)];
}

// convert a given value from a unit to another unit
float Settings::ConvertSize(DistanceUnits fromUnit, DistanceUnits toUnit, float value)
{
    // first, convert value to px
    // LaneWidth in px = laneWidth in unit / Scale / unitScale
    float valueInPx =  value / Scale / DistanceUnitScales[int(fromUnit)];

    // convert it to the target unit
    return valueInPx * Scale * DistanceUnitScales[int(toUnit)];
}

// convert a velocity from a given unit to a given unit
float Settings::ConvertVelocity(VelocityUnits fromUnit, VelocityUnits toUnit, float value)
{
    // first, convert value to px/s
    float valueInPx = value / Scale / VelocityUnitScales[int(fromUnit)];

    // convert it to the target unit
    return valueInPx * Scale * VelocityUnitScales[int(toUnit)];
}

// max speed for all the cars in px/s
float Settings::MaxSpeeds[3]
{
    ConvertVelocity(KMH, PXS, 100.f),
    ConvertVelocity(KMH, PXS, 90.f),
    ConvertVelocity(KMH, PXS, 110.f)
};
float Settings::MaxAcceleration[3]
{
    ConvertVelocity(KMH, PXS, 4.5f),
    ConvertVelocity(KMH, PXS, 3.f),
    ConvertVelocity(KMH, PXS, 5.5f)
};
float Settings::MinAcceleration[3]
{
    ConvertVelocity(KMH, PXS, -8.5f),
    ConvertVelocity(KMH, PXS, -7.f),
    ConvertVelocity(KMH, PXS, -12.f)
};

float Settings::GetMaxSpeedAs(VehicleTypeOptions vehicleType, VelocityUnits unit)
{
    return Settings::ConvertVelocity(PXS, unit, Settings::MaxSpeeds[vehicleType]);
}
