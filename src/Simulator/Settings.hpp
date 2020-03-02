//
// Created by Samuel Arbibe on 21/01/2020.
//

#ifndef SIMULATORSFML_SETTINGS_HPP
#define SIMULATORSFML_SETTINGS_HPP

#include <math.h>

#include <SFML/Graphics.hpp>

using namespace sf;

enum DistanceUnits
{
	M, CM, FEET, INCH, PX
};
enum VelocityUnits
{
	CMS, KMH, MS, MPH, PXS
};
enum VehicleTypeOptions
{
	CAR, TRUCK, MOTORCYCLE
};

class Settings
{
  public:

	static float GetLaneWidthAs(DistanceUnits unit);
	static float GetMaxSpeedAs(VehicleTypeOptions vehicleType, VelocityUnits unit);
	static float ConvertSize(DistanceUnits fromUnit, DistanceUnits toUnit, float value);
	static float ConvertVelocity(VelocityUnits fromUnit, VelocityUnits toUnit, float value);

	static const Vector2f ForwardVector;

	static bool DrawFps;
	static bool DrawActive;
	static bool DrawDelete;
	static bool DrawAdded;
	static bool DrawVehicleDataBoxes;
	static bool DrawRoadDataBoxes;
	static bool DrawLightDataBoxes;
	static bool DrawRoutes;
	static bool DrawLaneBlock;
	static bool DrawTextures;
	static bool DrawClickPoint;
	static bool FollowSelectedVehicle;

	static int MaxFps;
	static bool MultiColor;
	static float MinDistanceFromNextCar;
	static float MinDistanceFromStop;
	static bool AccWhileTurning;

	static float LaneWidth;
	static float MinLaneWidth;
	static float MaxLaneWidth;
	static float Scale;
	static float Speed;

	static float DefaultLaneLength;
	static int GridColumns;
	static int GridRows;
	static bool DrawGrid;

	static int SFMLRatio;
	static int DefaultMapWidth;
	static int DefaultMapHeight;
	static bool MapOverflow;

	static float Zoom;

	static float MinimapSize;
	static float MinimapMargin;

	// an array of scales for each distance unit
	// 0 - M
	// 1 - CM
	// 2 - Feet
	// 3 - Inches
	// 4 - Pixels
	static float DistanceUnitScales[5];

	// an array of scales for each distance unit
	// 0 - cm/s
	// 1 - km/h
	// 2 - m/s
	// 3 - mph
	// 4 - px/s
	static float VelocityUnitScales[5];

	// and array of max speeds for each vehicle in km/h
	// 0 - Car
	// 1 - Truck
	// 2 - Motorcycle
	static float MaxSpeeds[3];
	// same for accelerations
	static float MaxAcceleration[3];
	// same for braking
	static float MinAcceleration[3];

	static float CalculateDistance(Vector2f a, Vector2f b);
	static float CalculateAngle(float a, float b);

	static float OrangeDelay;
	static float DefaultCycleTime;
	static float PhaseDelay;
};

#endif //SIMULATORSFML_SETTINGS_HPP
