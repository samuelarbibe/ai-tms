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

float Settings::MinLaneWidth = 250;
float Settings::LaneWidth = 300; // lane width in px.
float Settings::MaxLaneWidth = 320;
float Settings::Scale = 3; // 1 px = [scale] * 1 cm
float Settings::Speed = 7; // running speed

float Settings::DefaultLaneLength = 150;

int Settings::GridColumns = 50;
int Settings::GridRows = 50;

int Settings::SFMLRatio = 1;

int Settings::DefaultMapWidth = 3000;
int Settings::DefaultMapHeight = 3000;

// camera setting
float Settings::Zoom = 0.5f;

// [LaneWidth in px] * unitScale = represented Width
float Settings::UnitScales[4]{0.01, 1, 0.0328, 0.3937};
