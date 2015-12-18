#ifndef _CHANGE_H_
#define _CHANGE_H_

#include <string>
#include "Math.h"

class Change
{
public:
	Change();
	~Change();
	static std::string IntToString(int num);
	static std::string FloatToString(float num);
	static std::string DoubleToString(double num);
	static int StringToInt(std::string str);
	static float StringToFloat(std::string str);
	static double StringToDouble(std::string str);
	static Vector3D ScreenToWorld(const Vector2D &pos, float z = 0.0f);
	static Vector2D WorldToScreen(const Vector3D &pos);
	static float SecondToFrame(float second);
	static float MinuteToFrame(float minute);
	static float HourToFrame(float hour);
	static float FrameToSecond(float frame);
	static float FrameToMinute(float frame);
	static float FrameToHour(float frame);
	static float MilliToMeter(float milli);
	static float CentiToMeter(float centi);
	static float KiloToMeter(float kilo);
	static float MilliToCenti(float milli);
	static float MeterToCenti(float meter);
	static float KiloToCenti(float kilo);

private:

};

#endif