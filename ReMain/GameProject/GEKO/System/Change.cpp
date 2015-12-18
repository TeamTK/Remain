#include "Change.h"
#include "Camera.h"

Change::Change()
{
}

Change::~Change()
{
}

std::string Change::IntToString(int num)
{
	return std::to_string(num);
}

std::string Change::FloatToString(float num)
{
	return std::to_string(num);
}

std::string Change::DoubleToString(double num)
{
	return std::to_string(num);
}

int Change::StringToInt(std::string str)
{
	return std::stoi(str);
}

float Change::StringToFloat(std::string str)
{
	return std::stof(str);
}

double Change::StringToDouble(std::string str)
{
	return std::stod(str);
}

Vector3D Change::ScreenToWorld(const Vector2D &pos, float z)
{
	Vector4D v4 = Vector4D(pos.x, pos.y, z, 1.0f);

	Matrix view;
	Matrix proj;
	Matrix viewport;

	//ãtçsóÒâª
	D3DXMatrixInverse(&view, NULL, Camera::GetView());
	D3DXMatrixInverse(&proj, NULL, Camera::GetProjection());
	D3DXMatrixInverse(&viewport, NULL, Camera::GetViewport());

	v4 = v4 * viewport * proj * view;

	return Vector3D(v4.x / v4.w, v4.y / v4.w, v4.z / v4.w);
}

Vector2D Change::WorldToScreen(const Vector3D &pos)
{
	Vector3D s = pos * (*Camera::GetView()) * (*Camera::GetProjection());
	Vector3D s2 = s / s.z * (*Camera::GetViewport());
	return Vector2D(s2.x, s2.y);
}

#define FPS 60

float Change::SecondToFrame(float second)
{
	return second * FPS;
}

float Change::MinuteToFrame(float minute)
{
	return SecondToFrame(minute * 60);
}

float Change::HourToFrame(float hour)
{
	return MinuteToFrame(hour * 60);
}

float Change::FrameToSecond(float frame)
{
	return frame / FPS;
}

float Change::FrameToMinute(float frame)
{
	return frame / (60 * FPS);
}

float Change::FrameToHour(float frame)
{
	return frame / (60 * 60 * FPS);
}

float Change::MilliToMeter(float milli)
{
	return 0.001f * milli;
}

float Change::CentiToMeter(float centi)
{
	return 0.01f * centi;
}

float Change::KiloToMeter(float kilo)
{
	return 1000.0f * kilo;
}

float Change::MilliToCenti(float milli)
{
	return 0.1f * milli;
}

float Change::MeterToCenti(float meter)
{
	return 100.0f * meter;
}

float Change::KiloToCenti(float kilo)
{
	return 100000.0f * kilo;
}