#include "Math.h"
#include "Window.h"
#include "Change.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>

#define RedToDeg 57.29577951f
#define DegToRed 0.017453293f

float Math::ChangeToDegree(float radian)
{
	return (radian * RedToDeg);
}

float Math::ChangeToRadian(float degree)
{
	return (degree * DegToRed);
}

unsigned int Math::ChangeTo16(unsigned int num)
{
	unsigned int i = num;
	char buf[1024] = "";
	char *str_ptr = &buf[sizeof(buf) - 1];

	while (i){
		*--str_ptr = i % 16;
		if (10>*str_ptr){
			*str_ptr += 0x30;//0x30は0の文字コード
		}
		else{
			*str_ptr += (0x41 - 0x0A);//0x41はAの文字コード
		}
		i /= 16;
	}

	int temp;
	sscanf_s(str_ptr, "%x", &temp);

	return (unsigned int)temp;
}

float Math::VelocityToFrameM(float meter)
{
	return meter / Change::SecondToFrame(1.0f);
}

float Math::VelocityToFrameK(float kilo)
{
	return Change::KiloToMeter(kilo) / Change::HourToFrame(1.0f);
}

/////Vector2Dクラス/////

Vector2D::Vector2D()
{
	x = y = 0.0f;
}

Vector2D::Vector2D(const Vector2D &v)
{
	x = v.x, y = v.y;
}

Vector2D::Vector2D(float x, float y)
{
	this->x = x, this->y = y;
}

Vector2D::~Vector2D()
{
}

Vector2D Vector2D::operator + (const Vector2D &v) const
{
	return Vector2D(x + v.x, y + v.y);
}

Vector2D Vector2D::operator - (const Vector2D &v) const
{
	return Vector2D(x - v.x, y - v.y);
}

void Vector2D::operator += (const Vector2D &v)
{
	x += v.x; y += v.y;
}

void Vector2D::operator -= (const Vector2D &v)
{
	x -= v.x; y -= v.y;
}

Vector2D Vector2D::operator * (float f) const
{
	return Vector2D(x * f, y * f);
}

Vector2D Vector2D::operator / (float f) const
{
	if (f != 0.0f) return Vector2D(x / f, y / f);
	return Vector2D(x, y);
}

void Vector2D::operator *= (float &f)
{
	x *= f, y *= f;
}

void Vector2D::operator /= (float &f)
{
	if (f != 0.0f) x /= f, y /= f;
}

float Vector2D::Length()
{
	return powf((x * x) + (y * y), 0.5f);
}

float Vector2D::LengthSq()
{
	return (x * x) + (y * y);
}

void Vector2D::SetNormalize()
{
	float length = Length();
	x /= length;
	y /= length;
}

Vector2D Vector2D::GetNormalize() const
{
	Vector2D out = *this;
	out.SetNormalize();
	return out;
}

float Vector2D::Dot(const Vector2D& v1, const Vector2D& v2)
{
	return (v1.x * v2.x) + (v1.y * v2.y);
}

float Vector2D::Cross(const Vector2D& v1, const Vector2D& v2)
{
	return (v1.x * v2.y) - (v1.y * v2.x);
}

float Vector2D::Distance(const Vector2D &v1, const Vector2D &v2)
{
	Vector2D v = v1 - v2;
	return powf((v.x * v.x) + (v.y * v.y), 0.5f);
}

void Vector2D::DebugDraw(const char *name)
{
	printf("******************************************************************************\n");
	printf("%s\n", name);
	printf("Vector2D : X = %f : Y = %f\n", x, y);
}

/////Vector3Dクラス/////

Vector3D::Vector3D()
{
	x = y = z = 0.0f;
};

Vector3D::Vector3D(const Vector3D &v)
{
	x = v.x, y = v.y, z = v.z;
};

Vector3D::Vector3D(float x, float y, float z)
{
	this->x = x, this->y = y, this->z = z;
}

Vector3D::Vector3D(const D3DXVECTOR3 &v)
{
	x = v.x;
	y = v.y;
	z = v.z;
}

Vector3D Vector3D::operator + (const Vector3D &v) const
{
	return Vector3D(x + v.x, y + v.y, z + v.z);
}

Vector3D Vector3D::operator - (const Vector3D &v) const
{
	return Vector3D(x - v.x, y - v.y, z - v.z);
}

Vector3D Vector3D::operator * (const Matrix& matrix) const
{
	Vector3D v;
	v.x = matrix._11 * x + matrix._21 * y + matrix._31 * z + matrix._41 * 1.0f;
	v.y = matrix._12 * x + matrix._22 * y + matrix._32 * z + matrix._42 * 1.0f;
	v.z = matrix._13 * x + matrix._23 * y + matrix._33 * z + matrix._43 * 1.0f;
	return v;
}

Vector3D Vector3D::operator * (const D3DXMATRIX &d3dxMatrix) const
{
	Vector3D v;
	v.x = d3dxMatrix._11 * x + d3dxMatrix._21 * y + d3dxMatrix._31 * z + d3dxMatrix._41 * 1.0f;
	v.y = d3dxMatrix._12 * x + d3dxMatrix._22 * y + d3dxMatrix._32 * z + d3dxMatrix._42 * 1.0f;
	v.z = d3dxMatrix._13 * x + d3dxMatrix._23 * y + d3dxMatrix._33 * z + d3dxMatrix._43 * 1.0f;
	return v;
}

void Vector3D::operator += (const Vector3D &v)
{
	x += v.x; y += v.y; z += v.z;
}

void Vector3D::operator -= (const Vector3D &v)
{
	x -= v.x; y -= v.y; z -= v.z;
}

Vector3D Vector3D::operator * (float f) const
{
	return Vector3D(x * f, y * f, z * f);
}

Vector3D Vector3D::operator / (float f) const
{
	if (f == 0.0f) return Vector3D(x, y, z);
	return Vector3D(x / f, y / f, z / f);
}

void Vector3D::operator *= (float &f)
{
	x *= f, y *= f, z *= f;
}

void Vector3D::operator /= (float &f)
{
	if (f != 0.0f) x /= f, y /= f, z /= f;
}

float Vector3D::Dot(const Vector3D &v) const
{
	return x * v.x + y * v.y * z * v.z;
}

Vector3D Vector3D::Cross(const Vector3D &v) const
{
	return Vector3D(
		y * v.z - z * v.y,
		z * v.x - x * v.z,
		x * v.y - y * v.x);
}

float Vector3D::Length()
{
	return powf((x * x) + (y * y) + (z * z), 0.5f);
}

float Vector3D::LengthSq()
{
	return (x * x) + (y * y) + (z * z);
}

void Vector3D::SetNormalize()
{
	float length = Length();
	x /= length;
	y /= length;
	z /= length;
}

Vector3D Vector3D::GetNormalize() const
{
	Vector3D out = *this;
	out.SetNormalize();
	return out;
}

Vector3D Vector3D::Lerp(Vector3D start, Vector3D end, float percentage)
{
	return start * (1 - percentage) + end * percentage;
}

Vector3D Vector3D::Matrix3x3(const Vector3D &v, const Matrix &matrix)
{
	Vector3D vec;
	vec.x = v.x * matrix._11 + v.y * matrix._21 + v.z * matrix._31;
	vec.y = v.x * matrix._12 + v.y * matrix._22 + v.z * matrix._32;
	vec.z = v.x * matrix._13 + v.y * matrix._23 + v.z * matrix._33;
	return vec;
}

Vector3D Vector3D::Cross(const Vector3D& v1, const Vector3D& v2)
{
	return Vector3D(
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x);
}

float Vector3D::Dot(const Vector3D& v1, const Vector3D& v2)
{
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

float Vector3D::Distance(const Vector3D &v1, const Vector3D &v2)
{
	Vector3D v = v1 - v2;
	return powf((v.x * v.x) + (v.y * v.y) + (v.z * v.z), 0.5f);
}

void Vector3D::DebugDraw(const char *name) const
{
	printf("%s\n", name);
	printf("Vector3D :X = %f :Y = %f :Z = %f\n", x, y, z);
}

/////Vector4D/////

Vector4D::Vector4D()
{
	x = y = z = 0.0f;
	w = 1.0f;
}

Vector4D::Vector4D(const Vector4D &v)
{
	*this = v;
}

Vector4D::Vector4D(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Vector4D::Vector4D(const D3DXVECTOR4 &v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
}

Vector4D::~Vector4D()
{
}

Vector4D Vector4D::operator * (const Matrix& matrix) const
{
	Vector4D v;
	v.x = matrix._11 * x + matrix._21 * y + matrix._31 * z + matrix._41 * w;
	v.y = matrix._12 * x + matrix._22 * y + matrix._32 * z + matrix._42 * w;
	v.z = matrix._13 * x + matrix._23 * y + matrix._33 * z + matrix._43 * w;
	v.w = matrix._14 * x + matrix._24 * y + matrix._34 * z + matrix._44 * w;
	return v;
}

Vector4D Vector4D::operator * (const D3DXMATRIX &d3dxMatrix) const
{
	Vector4D v;
	v.x = d3dxMatrix._11 * x + d3dxMatrix._21 * y + d3dxMatrix._31 * z + d3dxMatrix._41 * w;
	v.y = d3dxMatrix._12 * x + d3dxMatrix._22 * y + d3dxMatrix._32 * z + d3dxMatrix._42 * w;
	v.z = d3dxMatrix._13 * x + d3dxMatrix._23 * y + d3dxMatrix._33 * z + d3dxMatrix._43 * w;
	v.w = d3dxMatrix._14 * x + d3dxMatrix._24 * y + d3dxMatrix._34 * z + d3dxMatrix._44 * w;
	return v;
}

D3DXVECTOR4 Vector4D::Change()
{
	return D3DXVECTOR4(x, y, z, w);
}

void Vector4D::DebugDraw(const char *name)
{
	printf("******************************************************************************\n");
	printf("%s\n", name);
	printf("Vector4D : X = %f : Y = %f : Z = %f : W = %f\n", x, y, z, w);
}

Matrix::Matrix()
{
	ZeroMemory(this, sizeof(Matrix));
	_11 = 1.0f;
	_22 = 1.0f;
	_33 = 1.0f;
	_44 = 1.0f;
}

Matrix::Matrix(const Matrix &matrix)
{
	_11 = matrix._11; _12 = matrix._12; _13 = matrix._13; _14 = matrix._14;
	_21 = matrix._21; _22 = matrix._22; _23 = matrix._23; _24 = matrix._24;
	_31 = matrix._31; _32 = matrix._32; _33 = matrix._33; _34 = matrix._34;
	_41 = matrix._41; _42 = matrix._42; _43 = matrix._43; _44 = matrix._44;
}

Matrix::Matrix(const D3DXMATRIX &d3dxMatrix)
{
	_11 = d3dxMatrix._11; _12 = d3dxMatrix._12; _13 = d3dxMatrix._13; _14 = d3dxMatrix._14;
	_21 = d3dxMatrix._21; _22 = d3dxMatrix._22; _23 = d3dxMatrix._23; _24 = d3dxMatrix._24;
	_31 = d3dxMatrix._31; _32 = d3dxMatrix._32; _33 = d3dxMatrix._33; _34 = d3dxMatrix._34;
	_41 = d3dxMatrix._41; _42 = d3dxMatrix._42; _43 = d3dxMatrix._43; _44 = d3dxMatrix._44;
}

void Matrix::Indentity()
{
	ZeroMemory(this, sizeof(Matrix));
	D3DXMATRIX::_11 = 1.0f;
	D3DXMATRIX::_22 = 1.0f;
	D3DXMATRIX::_33 = 1.0f;
	D3DXMATRIX::_44 = 1.0f;
}

void Matrix::SetAxisX(float _11, float _21, float _31, float _41)
{
	this->_11 = _11;
	this->_21 = _21;
	this->_31 = _31;
	this->_41 = _41;
}

void Matrix::SetAxisY(float _12, float _22, float _32, float _42)
{
	this->_12 = _12;
	this->_22 = _22;
	this->_32 = _32;
	this->_42 = _42;
}

void Matrix::SetAxisZ(float _13, float _23, float _33, float _43)
{
	this->_13 = _13;
	this->_23 = _23;
	this->_33 = _33;
	this->_43 = _43;
}

void Matrix::SetAxisW(float _14, float _24, float _34, float _44)
{
	this->_14 = _14;
	this->_24 = _24;
	this->_34 = _34;
	this->_44 = _44;
}

void Matrix::SetInverse()
{
	float det = 0;

	det += (_11 * _22 * _33 * _44) + (_11 * _23 * _34 * _42) + (_11 * _24 * _32 * _43);
	det += (_12 * _21 * _34 * _43) + (_12 * _23 * _31 * _44) + (_12 * _24 * _33 * _41);
	det += (_13 * _21 * _32 * _44) + (_13 * _22 * _34 * _41) + (_13 * _24 * _31 * _42);
	det += (_14 * _21 * _33 * _42) + (_14 * _22 * _31 * _43) + (_14 * _23 * _32 * _41);

	det -= (_11 * _22 * _34 * _43) - (_11 * _23 * _32 * _44) - (_11 * _24 * _33 * _42);
	det -= (_12 * _21 * _33 * _44) - (_12 * _23 * _34 * _41) - (_12 * _24 * _31 * _43);
	det -= (_13 * _21 * _34 * _42) - (_13 * _22 * _31 * _44) - (_13 * _24 * _32 * _41);
	det -= (_14 * _21 * _32 * _43) - (_14 * _22 * _33 * _41) - (_14 * _23 * _31 * _42);

	//1行目
	_11 = (_22 * _33 * _44) + (_23 * _34 * _42) + (_24 * _32 * _43) - (_22 * _34 * _43) - (_23 * _32 * _44) - (_24 * _33 * _42);
	_12 = (_12 * _34 * _43) + (_13 * _32 * _44) + (_14 * _33 * _42) - (_12 * _33 * _44) - (_13 * _34 * _42) - (_14 * _32 * _43);
	_13 = (_12 * _23 * _44) + (_13 * _24 * _42) + (_14 * _22 * _43) - (_12 * _24 * _43) - (_13 * _22 * _44) - (_14 * _23 * _42);
	_14 = (_12 * _24 * _33) + (_13 * _22 * _34) + (_14 * _23 * _32) - (_12 * _23 * _34) - (_13 * _24 * _32) - (_14 * _22 * _33);

	//2行目
	_21 = (_21 * _34 * _43) + (_23 * _31 * _44) + (_24 * _33 * _41) - (_21 * _33 * _44) - (_23 * _34 * _41) - (_24 * _31 * _43);
	_22 = (_11 * _33 * _44) + (_13 * _34 * _41) + (_14 * _31 * _43) - (_11 * _34 * _43) - (_13 * _31 * _44) - (_14 * _33 * _41);
	_23 = (_11 * _24 * _43) + (_13 * _21 * _44) + (_14 * _23 * _41) - (_11 * _23 * _44) - (_13 * _24 * _41) - (_14 * _21 * _43);
	_24 = (_11 * _23 * _34) + (_13 * _24 * _31) + (_14 * _21 * _33) - (_11 * _24 * _33) - (_13 * _21 * _34) - (_14 * _23 * _31);

	//3行目
	_31 = (_21 * _32 * _44) + (_22 * _34 * _41) + (_24 * _31 * _42) - (_21 * _34 * _42) - (_22 * _31 * _44) - (_24 * _32 * _41);
	_32 = (_11 * _34 * _42) + (_12 * _31 * _44) + (_14 * _32 * _41) - (_11 * _32 * _44) - (_12 * _34 * _41) - (_14 * _31 * _42);
	_33 = (_11 * _22 * _44) + (_12 * _24 * _41) + (_14 * _21 * _42) - (_11 * _24 * _42) - (_12 * _21 * _44) - (_14 * _22 * _41);
	_34 = (_11 * _24 * _32) + (_12 * _21 * _34) + (_14 * _22 * _31) - (_11 * _22 * _34) - (_12 * _24 * _31) - (_14 * _21 * _32);

	//4行目
	_41 = (_21 * _33 * _42) + (_22 * _31 * _43) + (_23 * _32 * _41) - (_21 * _32 * _43) - (_22 * _33 * _41) - (_23 * _31 * _42);
	_42 = (_11 * _32 * _43) + (_12 * _33 * _41) + (_13 * _31 * _42) - (_11 * _33 * _42) - (_12 * _31 * _43) - (_13 * _32 * _41);
	_43 = (_11 * _23 * _42) + (_12 * _21 * _43) + (_13 * _22 * _41) - (_11 * _22 * _43) - (_12 * _23 * _41) - (_13 * _21 * _42);
	_44 = (_11 * _22 * _33) + (_12 * _23 * _31) + (_13 * _21 * _32) - (_11 * _23 * _32) - (_12 * _21 * _33) - (_13 * _22 * _31);

	_11 = _11 / det; _21 = _21 / det; _31 = _31 / det; _41 = _41 / det;
	_12 = _12 / det; _22 = _22 / det; _32 = _32 / det; _42 = _42 / det;
	_13 = _13 / det; _23 = _23 / det; _33 = _33 / det; _43 = _43 / det;
	_14 = _14 / det; _24 = _24 / det; _34 = _34 / det; _44 = _44 / det;	

	//D3DXMatrixInverse(this, NULL, this);
}

void Matrix::RotationRadianX(float radian)
{
	float Sin = sinf(radian);
	float Cos = cosf(radian);

	_11 = 1.0f; _12 = 0.0f; _13 = 0.0f; _14 = 0.0f;
	_21 = 0.0f; _22 = Cos;  _23 = Sin;  _24 = 0.0f;
	_31 = 0.0f; _32 = -Sin; _33 = Cos;  _34 = 0.0f;
	_41 = 0.0f; _42 = 0.0f; _43 = 0.0f; _44 = 1.0f;
}

void Matrix::RotationRadianY(float radian)
{
	float Sin = sinf(radian);
	float Cos = cosf(radian);

	_11 = Cos;  _12 = 0.0f; _13 = -Sin; _14 = 0.0f;
	_21 = 0.0f; _22 = 1.0f; _23 = 0.0f; _24 = 0.0f;
	_31 = Sin;  _32 = 0.0f; _33 = Cos;  _34 = 0.0f;
	_41 = 0.0f; _42 = 0.0f; _43 = 0.0f; _44 = 1.0f;
}

void Matrix::RotationRadianZ(float radian)
{
	float Sin = sinf(radian);
	float Cos = cosf(radian);

	_11 = Cos;  _12 = Sin;  _13 = 0.0f; _14 = 0.0f;
	_21 = -Sin; _22 = Cos;  _23 = 0.0f; _24 = 0.0f;
	_31 = 0.0f; _32 = 0.0f; _33 = 1.0f; _34 = 0.0f;
	_41 = 0.0f; _42 = 0.0f; _43 = 0.0f; _44 = 1.0f;
}

void Matrix::RotationDegreeX(float degree)
{
	float Sin = sinf(degree * DegToRed);
	float Cos = cosf(degree * DegToRed);

	_11 = 1.0f; _12 = 0.0f; _13 = 0.0f; _14 = 0.0f;
	_21 = 0.0f; _22 = Cos;  _23 = Sin;  _24 = 0.0f;
	_31 = 0.0f; _32 = -Sin; _33 = Cos;  _34 = 0.0f;
	_41 = 0.0f; _42 = 0.0f; _43 = 0.0f; _44 = 1.0f;
}

void Matrix::RotationDegreeY(float degree)
{
	float Sin = sinf(degree * DegToRed);
	float Cos = cosf(degree * DegToRed);

	_11 = Cos;  _12 = 0.0f; _13 = -Sin; _14 = 0.0f;
	_21 = 0.0f; _22 = 1.0f; _23 = 0.0f; _24 = 0.0f;
	_31 = Sin;  _32 = 0.0f; _33 = Cos;  _34 = 0.0f;
	_41 = 0.0f; _42 = 0.0f; _43 = 0.0f; _44 = 1.0f;
}

void Matrix::RotationDegreeZ(float degree)
{
	float Sin = sinf(degree * DegToRed);
	float Cos = cosf(degree * DegToRed);

	_11 = Cos;  _12 = Sin;  _13 = 0.0f; _14 = 0.0f;
	_21 = -Sin; _22 = Cos;  _23 = 0.0f; _24 = 0.0f;
	_31 = 0.0f; _32 = 0.0f; _33 = 1.0f; _34 = 0.0f;
	_41 = 0.0f; _42 = 0.0f; _43 = 0.0f; _44 = 1.0f;
}

void Matrix::Transelate(float x, float y, float z)
{
	_41 = x;
	_42 = y;
	_43 = z;
}

void Matrix::Scale(float x, float y, float z)
{
	_11 = x;
	_22 = y;
	_33 = z;
}

void Matrix::DebugDraw(const char *name)
{
	printf("******************************************************************************\n");
	printf("%s\n", name);
	printf("Matrix : _11 = %f : _12 = %f : _13 = %f : _14 = %f\n", _11, _12, _13, _14);
	printf("Matrix : _21 = %f : _22 = %f : _23 = %f : _24 = %f\n", _21, _22, _23, _24);
	printf("Matrix : _31 = %f : _32 = %f : _33 = %f : _34 = %f\n", _31, _32, _33, _34);
	printf("Matrix : _41 = %f : _42 = %f : _43 = %f : _44 = %f\n", _41, _42, _43, _44);
}

Vector3D Matrix::GetAxisZ()
{
	return Vector3D(-_13, _23, _33);
}

Vector3D Matrix::GetAxisY()
{
	return Vector3D(_12, _22, _32);
}

Vector3D Matrix::GetAxisX()
{
	return Vector3D(_11, -_21, -_31);
}

Matrix Matrix::GetInverse()
{
	/*
	float det = 0;
	Matrix m;

	det += (_11 * _22 * _33 * _44) + (_11 * _23 * _34 * _42) + (_11 * _24 * _32 * _43);
	det += (_12 * _21 * _34 * _43) + (_12 * _23 * _31 * _44) + (_12 * _24 * _33 * _41);
	det += (_13 * _21 * _32 * _44) + (_13 * _22 * _34 * _41) + (_13 * _24 * _31 * _42);
	det += (_14 * _21 * _33 * _42) + (_14 * _22 * _31 * _43) + (_14 * _23 * _32 * _41);

	det -= (_11 * _22 * _34 * _43) + (_11 * _23 * _32 * _44) + (_11 * _24 * _33 * _42);
	det -= (_12 * _21 * _33 * _44) + (_12 * _23 * _34 * _41) + (_12 * _24 * _31 * _43);
	det -= (_13 * _21 * _34 * _42) + (_13 * _22 * _31 * _44) + (_13 * _24 * _32 * _41);
	det -= (_14 * _21 * _32 * _43) + (_14 * _22 * _33 * _41) + (_14 * _23 * _31 * _42);

	//1行目
	m._11 = (_22 * _33 * _44) + (_23 * _34 * _42) + (_24 * _32 * _43) - (_22 * _34 * _43) - (_23 * _32 * _44) - (_24 * _33 * _42);
	m._12 = (_12 * _34 * _43) + (_13 * _32 * _44) + (_14 * _33 * _42) - (_12 * _33 * _44) - (_13 * _34 * _42) - (_14 * _32 * _43);
	m._13 = (_12 * _23 * _44) + (_13 * _24 * _42) + (_14 * _22 * _43) - (_12 * _24 * _43) - (_13 * _22 * _44) - (_14 * _23 * _42);
	m._14 = (_12 * _24 * _33) + (_13 * _22 * _34) + (_14 * _23 * _32) - (_12 * _23 * _34) - (_13 * _24 * _32) - (_14 * _22 * _33);

	//2行目
	m._21 = (_21 * _34 * _43) + (_23 * _31 * _44) + (_24 * _33 * _41) - (_21 * _33 * _44) - (_23 * _34 * _41) - (_24 * _31 * _43);
	m._22 = (_11 * _33 * _44) + (_13 * _34 * _41) + (_14 * _31 * _43) - (_11 * _34 * _43) - (_13 * _31 * _44) - (_14 * _33 * _41);
	m._23 = (_11 * _24 * _43) + (_13 * _21 * _44) + (_14 * _23 * _41) - (_11 * _23 * _44) - (_13 * _24 * _41) - (_14 * _21 * _43);
	m._24 = (_11 * _23 * _34) + (_13 * _24 * _31) + (_14 * _21 * _33) - (_11 * _24 * _33) - (_13 * _21 * _34) - (_14 * _23 * _31);

	//3行目
	m._31 = (_21 * _32 * _44) + (_22 * _34 * _41) + (_24 * _31 * _42) - (_21 * _34 * _42) - (_22 * _31 * _44) - (_24 * _32 * _41);
	m._32 = (_11 * _34 * _42) + (_12 * _31 * _44) + (_14 * _32 * _41) - (_11 * _32 * _44) - (_12 * _34 * _41) - (_14 * _31 * _42);
	m._33 = (_11 * _22 * _44) + (_12 * _24 * _41) + (_14 * _21 * _42) - (_11 * _24 * _42) - (_12 * _21 * _44) - (_14 * _22 * _41);
	m._34 = (_11 * _24 * _32) + (_12 * _21 * _34) + (_14 * _22 * _31) - (_11 * _22 * _34) - (_12 * _24 * _31) - (_14 * _21 * _32);

	//4行目
	m._41 = (_21 * _33 * _42) + (_22 * _31 * _43) + (_23 * _32 * _41) - (_21 * _32 * _43) - (_22 * _33 * _41) - (_23 * _31 * _42);
	m._42 = (_11 * _32 * _43) + (_12 * _33 * _41) + (_13 * _31 * _42) - (_11 * _33 * _42) - (_12 * _31 * _43) - (_13 * _32 * _41);
	m._43 = (_11 * _23 * _42) + (_12 * _21 * _43) + (_13 * _22 * _41) - (_11 * _22 * _43) - (_12 * _23 * _41) - (_13 * _21 * _42);
	m._44 = (_11 * _22 * _33) + (_12 * _23 * _31) + (_13 * _21 * _32) - (_11 * _23 * _32) - (_12 * _21 * _33) - (_13 * _22 * _31);

	Matrix temp = m * (1 / det);

	return temp;
	*/

	Matrix temp;
	D3DXMatrixInverse(&temp, NULL, this);
	return temp;
}

void Matrix::operator = (const Matrix &matrix)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m[i][j] = matrix.m[i][j];
		}
	}
}

void Matrix::operator = (const D3DXMATRIX &d3dxMatrix)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m[i][j] = d3dxMatrix.m[i][j];
		}
	}
}

Matrix Matrix::operator * (float num)
{
	Matrix temp;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			temp.m[i][j] *= num;
		}
	}

	return temp;
}

Matrix Matrix::operator * (const Matrix &matrix)
{
	Matrix temp;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			temp.m[i][j] = 0.0f;

			for (int k = 0; k < 4; k++)
			{
				temp.m[i][j] += m[i][k] * matrix.m[k][j];
			}
		}
	}

	return temp;
}

Matrix Matrix::operator * (const D3DXMATRIX &matrix)
{
	Matrix temp;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			temp.m[i][j] = 0.0f;

			for (int k = 0; k < 4; k++)
			{
				temp.m[i][j] += m[i][k] * matrix.m[k][j];
			}
		}
	}

	return temp;
}

Matrix Matrix::Mult(Matrix &matrix, Matrix &matrix2)
{
	return matrix * matrix2;
}