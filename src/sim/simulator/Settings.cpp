//
// Created by Samuel Arbibe on 21/01/2020.
//

#include "Settings.hpp"

const Vector2f Settings::BaseVec = Vector2f(0.f, -1.f);

bool Settings::DrawFps = false;
bool Settings::DrawActive = false;
bool Settings::DrawDelete = false;
bool Settings::DrawAdded = false;

bool Settings::DrawVehicleDataBoxes = false;
bool Settings::DrawRoadDataBoxes = false;
bool Settings::DrawLightDataBoxes = false;
bool Settings::DrawRoutes = false;
bool Settings::DrawGrid = false;
bool Settings::DrawLaneBlock = false;
bool Settings::DrawTextures = true;
bool Settings::DrawClickPoint = false;
bool Settings::DrawMinimap = false;
bool Settings::DrawSimTable = false;
bool Settings::FollowSelectedVehicle = true;
bool Settings::LaneDensityColorRamping = false;
bool Settings::ShowSelectedPhaseLanes = false;
bool Settings::PrintSimulationLog = true;


int Settings::Interval = 700; // max is 1000
int Settings::Fps = 30;
int Settings::AntiAliasing = 0;
bool Settings::MultiColor = false;
bool Settings::MultiTypeVehicle = true;
float Settings::MinDistanceFromNextCar = 55;
float Settings::MinDistanceFromStop = 50;
bool Settings::AccWhileTurning = true;

float Settings::MinLaneWidth = 83;
float Settings::LaneWidth = 115; // lane width in px.
float Settings::MaxLaneWidth = 140;
float Settings::DashLineLength = 100;
float Settings::DashLineSpace = 80;
float Settings::Scale = 3; // 1 px / [scale] = 1 cm
float Settings::Speed = 1; // running speed
bool Settings::DoubleSeparatorLine = true;
float Settings::VehicleSpawnRate = 0.9f;
float Settings::MaxDensity = 0.20f;

float Settings::DefaultLaneLength = 3000; // lane length in px

int Settings::DefaultMapWidth = 10000;
int Settings::DefaultMapHeight = 10000;

int Settings::GridColumns = 50;
int Settings::GridRows = 50;

int Settings::SFMLRatio = 1;

bool Settings::MapOverflow = false;

// camera setting
float Settings::Zoom = 0.1f;
float Settings::DragFactor = 5.f;

// minimap Settings
float Settings::MinimapSize = 120.f;
float Settings::MinimapMargin = 10.f;

// [LaneWidth in px] * Scale * unitScale = laneWidth in Unit
// M, CM, Feet, Inch
float Settings::DistanceUnitScales[5]
	{
		0.01, 1, 0.0328, 0.3937, 1 / Scale
	};

// [Speed in px/s] * Scale * unitScale = Speed in Unit
// that means that [speed in cm/s] * unitScale = Speed in Unit
// CM/S, KM/H, M/S, MPH, PX/S
float Settings::VelocityUnitScales[5]
	{
		1, 0.036, 0.01, 0.022, 1 / Scale
	};

// [LaneWidth in px] * Scale * unitScale = laneWidth in Unit
float Settings::GetLaneWidthAs(DistanceUnits unit) {
	// base
	float len = Settings::LaneWidth * Settings::Scale;

	return len * DistanceUnitScales[int(unit)];
}

// convert a given value from a unit to another unit
float Settings::ConvertSize(DistanceUnits fromUnit, DistanceUnits toUnit, float value) {
	// first, convert value to px
	// LaneWidth in px = laneWidth in unit / Scale / unitScale
	float valueInPx = value / Scale / DistanceUnitScales[int(fromUnit)];

	// convert it to the target unit
	return valueInPx * Scale * DistanceUnitScales[int(toUnit)];
}

// convert a velocity from a given unit to a given unit
float Settings::ConvertVelocity(VelocityUnits fromUnit, VelocityUnits toUnit, float value) {
	// first, convert value to px/s
	float valueInPx = value / Scale / VelocityUnitScales[int(fromUnit)];

	// convert it to the target unit
	return valueInPx * Scale * VelocityUnitScales[int(toUnit)];
}

// max speed for all the cars in px/s
float Settings::MaxSpeeds[4]
    {
        ConvertVelocity(KMH, PXS, 50.f),
        ConvertVelocity(KMH, PXS, 50.f),
        ConvertVelocity(KMH, PXS, 50.f),
        ConvertVelocity(KMH, PXS, 50.f)
    };
// convert m/ss to px/ss
float Settings::Acceleration[4]
	{
		ConvertVelocity(MS, PXS, 2.f),
		ConvertVelocity(MS, PXS, 2.f),
		ConvertVelocity(MS, PXS, 2.f),
		ConvertVelocity(MS, PXS, 2.f)
	};
float Settings::Deceleration[4]
	{
		ConvertVelocity(MS, PXS, -4.5f),
		ConvertVelocity(MS, PXS, -4.5f),
		ConvertVelocity(MS, PXS, -4.5f),
		ConvertVelocity(MS, PXS, -4.5f)
	};

float Settings::GetMaxSpeedAs(VehicleTypeOptions vehicleType, VelocityUnits unit) {
	return Settings::ConvertVelocity(PXS, unit, Settings::MaxSpeeds[vehicleType]);
}

/// calculate distance between 2 vectors
float Settings::CalculateDistance(Vector2f a, Vector2f b) {
	float xDist = abs(a.x - b.x);
	float yDist = abs(a.y - b.y);

	return sqrt(xDist * xDist + yDist * yDist);
}

float Settings::CalculateAngle(float a, float b) {

	if (a == 0)
		a += 360;
	if (b == 0)
		b += 360;

	float temp = -(a - b);

	if (temp < -180)
	{
		temp += 360;
	}

	if (temp > 180)
	{
		temp -= 360;
	}

	return temp;
}

// convert a time to a string, to fit JSON formatting
// the string will be formatted int this format:
//          'DD-MM-YYYY HH:MM:SS'
string Settings::ConvertTimeToString(tm *time) {

	ostringstream oss;
	oss << asctime(time);
	string var = oss.str();

	return var;
}

tm *Settings::ConvertStringToTime(const string str) {

	tm * time = new tm();

	time->tm_mday = stoi(str.substr(0, 2));
	time->tm_mon = stoi(str.substr(3, 2)) - 1;
	time->tm_year = stoi(str.substr(6, 4)) - 1900;

	time->tm_hour = stoi(str.substr(11, 2));
	time->tm_min = stoi(str.substr(14, 2));
	time->tm_sec = stoi(str.substr(17, 2));

	return time;
}

void Settings::GetHeatMapColor(float value, float *red, float *green, float *blue)
{
	int aR = 0;   int aG = 0; int aB=255;  // RGB for our 1st color blue
	int bR = 255; int bG = 0; int bB=0;    // RGB for our 2nd color red

	*red   = (float)(bR - aR) * value + aR;      // 255 * value.
	*green = (float)(bG - aG) * value + aG;      // 0.
	*blue  = (float)(bB - aB) * value + aB;      // 255 * (1 - value).
}

float Settings::OrangeDelay = 3.f;
float Settings::DefaultCycleTime = 20.f;
float Settings::MaxCycleTime = 60.f;
float Settings::MinCycleTime = 20.f;
float Settings::PhaseDelay = 1.5f;
