//
// Created by Samuel Arbibe on 21/01/2020.
//

#ifndef SIMULATORSFML_SETTINGS_HPP
#define SIMULATORSFML_SETTINGS_HPP

enum Units {M, CM, FEET, INCH};

class Settings {
public:
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

    // an array of scales for each distance unit
    static float UnitScales[4];
    // 0 - M
    // 1 - CM
    // 2 - Feet
    // 3 - Inches

};


#endif //SIMULATORSFML_SETTINGS_HPP
