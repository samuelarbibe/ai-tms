//
// Created by Samuel Arbibe on 21/01/2020.
//

#ifndef SIMULATORSFML_SETTINGS_HPP
#define SIMULATORSFML_SETTINGS_HPP

#include <cmath>
#include <cstring>
#include <sstream>
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

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
	SMALL_CAR, MEDIUM_CAR, LONG_CAR, TRUCK
};

class Settings
{
  public:

	static float GetLaneWidthAs(DistanceUnits unit);
	static float GetMaxSpeedAs(VehicleTypeOptions vehicleType, VelocityUnits unit);
	static float ConvertSize(DistanceUnits fromUnit, DistanceUnits toUnit, float value);
	static float ConvertVelocity(VelocityUnits fromUnit, VelocityUnits toUnit, float value);

	static const Vector2f BaseVec;

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
	static bool DrawMinimap;
	static bool DrawSimTable;
	static bool FollowSelectedVehicle;
	static bool LaneDensityColorRamping;
	static bool ShowSelectedPhaseLanes;
	static bool PrintSimulationLog;

	static int Interval;
	static int Fps;
	static int AntiAliasing;
	static bool MultiColor;
	static bool MultiTypeVehicle;
	static float MinDistanceFromNextCar;
	static float MinDistanceFromStop;
	static bool AccWhileTurning;

	static float LaneWidth;
	static float MinLaneWidth;
	static float MaxLaneWidth;
	static float DashLineLength;
	static float DashLineSpace;
	static float Scale;
	static float Speed;
	static bool  DoubleSeparatorLine;
	static float VehicleSpawnRate;
	static float MaxDensity;

	static float DefaultLaneLength;
	static int GridColumns;
	static int GridRows;
	static bool DrawGrid;

	static int SFMLRatio;
	static int DefaultMapWidth;
	static int DefaultMapHeight;
	static bool MapOverflow;

	static float Zoom;
	static float DragFactor;

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
	static float MaxSpeeds[4];
	// same for accelerations
	static float Acceleration[4];
	// same for braking
	static float Deceleration[4];

	static float CalculateDistance(Vector2f a, Vector2f b);
	static float CalculateAngle(float a, float b);
	static string ConvertTimeToString(tm * time);
	static tm * ConvertStringToTime(const string str);

	static void GetHeatMapColor(float value, float *red, float *green, float *blue);

	static float OrangeDelay;
	static float DefaultCycleTime;
	static float MaxCycleTime;
	static float MinCycleTime;
	static float PhaseDelay;
};

#endif //SIMULATORSFML_SETTINGS_HPP
