#ifndef _MATH_H_
#define _MATH_H_

#include "Direct3D11.h"

#define PI 3.141592653589793f

class Matrix;

//êîäwÉNÉâÉX
class Math
{
public:
	static float ChangeToDegree(float radian);
	static float ChangeToRadian(float degree);
	static unsigned int ChangeTo16(unsigned int num);
	static float VelocityToFrameM(float meter);
	static float VelocityToFrameK(float kilo);

private:

};

struct SVector2D
{
	union
	{
		float v[2];
		struct
		{
			float x;
			float y;
		};
	};
};

struct SVector3D
{
	union 
	{
		float v[3];
		struct 
		{
			float x;
			float y;
			float z;
		};
	};
};

struct SVector4D
{
	union
	{
		float v[4];
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};
	};
};

class Vector2D : public D3DXVECTOR2
{
public:
	Vector2D();
	~Vector2D();
	Vector2D(const Vector2D &v);
	Vector2D(D3DXVECTOR2 &v);
	Vector2D(float x, float y);
	Vector2D operator +  (const Vector2D &v) const;
	Vector2D operator -  (const Vector2D &v) const;
	//Vector2D operator *  (const Matrix &matrix) const;
	void	 operator += (const Vector2D &v);
	void	 operator -= (const Vector2D &v);
	Vector2D operator *  (float f) const;
	Vector2D operator /  (float f) const;
	void	 operator *= (float &f);
	void	 operator /= (float &f);
	float	 Length();
	float	 LengthSq();
	void	 SetNormalize();
	Vector2D GetNormalize() const;
	static float Dot(const Vector2D& v1, const Vector2D& v2);
	static float Cross(const Vector2D &v1, const Vector2D &v2);
	static float Distance(const Vector2D &v1, const Vector2D &v2);
	void DebugDraw(const char *name) const;

private:

};

class Vector3D : public D3DXVECTOR3
{
public:
	Vector3D();
	Vector3D(const Vector3D &v);
	Vector3D(float x, float y, float z);
	Vector3D(const D3DXVECTOR3 &v);
	Vector3D operator +  (const Vector3D &v) const;
	Vector3D operator -  (const Vector3D &v) const;
	Vector3D operator *  (const Matrix &matrix) const;
	Vector3D operator *  (const D3DXMATRIX &d3dxMatrix) const;
	void	 operator += (const Vector3D &v);
	void	 operator -= (const Vector3D &v);
	Vector3D operator *  (float f) const;
	Vector3D operator /  (float f) const;
	void	 operator *= (float &f);
	void	 operator /= (float &f);
	float    Dot(const Vector3D &v) const;
	Vector3D Cross(const Vector3D &v) const;
	float	 Length();
	float	 LengthSq();
	void	 SetNormalize();
	Vector3D GetNormalize() const;
	static Vector3D Lerp(Vector3D start, Vector3D end, float percentage);
	static Vector3D Matrix3x3(const Vector3D &v, const Matrix &matrix);
	static Vector3D Cross(const Vector3D &v1, const Vector3D &v2);
	static float Dot(const Vector3D& v1, const Vector3D& v2);
	static float Distance(const Vector3D &v1, const Vector3D &v2);
	void DebugDraw(const char *name) const;
};

class Vector4D : public D3DXVECTOR4
{
public:
	Vector4D();
	Vector4D(const Vector4D &v);
	Vector4D(float x, float y, float z, float w);
	Vector4D(const D3DXVECTOR4 &v);
	~Vector4D();
	Vector4D operator * (const Matrix &matrix) const;
	Vector4D operator * (const D3DXMATRIX &d3dxMatrix) const;
	D3DXVECTOR4 Change();
	void DebugDraw(const char *name) const;
};

class Matrix : public D3DXMATRIX
{
public:
	Matrix();
	Matrix(const Matrix &matrix);
	Matrix(const D3DXMATRIX &d3dxMatrix);
	void Indentity();
	void SetInverse();
	void SetAxisX(float _11, float _21, float _31, float _41);
	void SetAxisY(float _12, float _22, float _32, float _42);
	void SetAxisZ(float _13, float _23, float _33, float _43);
	void SetAxisW(float _14, float _24, float _34, float _44);
	void RotationRadianX(float radian);
	void RotationRadianY(float radian);
	void RotationRadianZ(float radian);
	void RotationDegreeX(float degree);
	void RotationDegreeY(float degree);
	void RotationDegreeZ(float degree);
	void Transelate(float x, float y, float z);
	void Scale(float x, float y, float z);
	void DebugDraw(const char *name) const;
	Vector3D GetAxisZ();
	Vector3D GetAxisY();
	Vector3D GetAxisX();
	Matrix GetInverse();

	void operator = (const Matrix &matrix);
	void operator = (const D3DXMATRIX &d3dxMatrix);
	Matrix operator * (float num);
	Matrix operator * (const Matrix &matrix);
	Matrix operator * (const D3DXMATRIX &matrix);
	static Matrix Mult(Matrix &matrix, Matrix &matrix2);

private:

};


#endif