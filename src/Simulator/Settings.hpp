//
// Created by Samuel Arbibe on 21/01/2020.
//

#ifndef SIMULATORSFML_SETTINGS_HPP
#define SIMULATORSFML_SETTINGS_HPP

enum Units {M, CM, FEET, INCH, PX};

class Settings {
public:

    static float GetLaneWidthAs(Units unit);
    static float ConvertSize(Units fromUnit, Units toUnit, float value);

    static bool DrawFps;
    static bool DrawActive;
    static bool DrawDelete;
    static bool DrawAdded;

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

    static int SFMLRatio;
    static int DefaultMapWidth;
    static int DefaultMapHeight;

    static float Zoom;

    static float MinimapSize;
    static float MinimapMargin;

    // an array of scales for each distance unit
    // 0 - M
    // 1 - CM
    // 2 - Feet
    // 3 - Inches
    // 4 - Pixels
    static float UnitScales[5];
};


#endif //SIMULATORSFML_SETTINGS_HPP
