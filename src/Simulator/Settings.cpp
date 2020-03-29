//
// Created by Samuel Arbibe on 21/01/2020.
//

#include "Settings.hpp"

const Vector2f Settings::BaseVec = Vector2f(0.f, -1.f);

bool Settings::DrawFps = false;
bool Settings::DrawActive = true;
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

int Settings::Interval = 120; // max is 1000
int Settings::Fps = 60;
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
float Settings::VehicleSpawnDelay = 0.9f;

float Settings::DefaultLaneLength = 1500; // lane length in px

int Settings::DefaultMapWidth = 5000;
int Settings::DefaultMapHeight = 5000;

int Settings::GridColumns = 50;
int Settings::GridRows = 50;

int Settings::SFMLRatio = 1;

bool Settings::MapOverflow = false;

// camera setting
float Settings::Zoom = 0.1f;

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
float Settings::Acceleration[4]
	{
		ConvertVelocity(MSS, PXS, 2.f),
		ConvertVelocity(MSS, PXS, 2.f),
		ConvertVelocity(MSS, PXS, 2.f),
		ConvertVelocity(MSS, PXS, 2.f)
	};
float Settings::Deceleration[4]
	{
		ConvertVelocity(MSS, PXS, -4.5f),
		ConvertVelocity(MSS, PXS, -4.5f),
		ConvertVelocity(MSS, PXS, -4.5f),
		ConvertVelocity(MSS, PXS, -4.5f)
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
	const int NUM_COLORS = 4;
	static float color[NUM_COLORS][3] = { {0,0,255}, {0,255,0}, {255,255,0}, {255,0,0} };
	// A static array of 4 colors:  (blue,   green,  yellow,  red) using {r,g,b} for each.

	int idx1;        // |-- Our desired color will be between these two indexes in "color".
	int idx2;        // |
	float fractBetween = 0;  // Fraction between "idx1" and "idx2" where our value is.

	if(value <= 0)      {  idx1 = idx2 = 0;            }    // accounts for an input <=0
	else if(value >= 1)  {  idx1 = idx2 = NUM_COLORS-1; }    // accounts for an input >=0
	else
	{
		value = value * (NUM_COLORS-1);        // Will multiply value by 3.
		idx1  = floor(value);                  // Our desired color will be after this index.
		idx2  = idx1+1;                        // ... and before this index (inclusive).
		fractBetween = value - float(idx1);    // Distance between the two indexes (0-1).
	}

	*red   = (color[idx2][0] - color[idx1][0])*fractBetween + color[idx1][0];
	*green = (color[idx2][1] - color[idx1][1])*fractBetween + color[idx1][1];
	*blue  = (color[idx2][2] - color[idx1][2])*fractBetween + color[idx1][2];
}

float Settings::OrangeDelay = 3.f;
float Settings::DefaultCycleTime = 20.f;
float Settings::MaxCycleTime = 60.f;
float Settings::MinCycleTime = 5.f;
float Settings::PhaseDelay = 1.5f;
