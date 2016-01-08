#ifndef _SHADER_H_
#define _SHADER_H_

#include "..\System\Math.h"

class Shader
{
public:
	Shader();
	~Shader();
	HRESULT InitVertex();
	HRESULT InitPixel();
	HRESULT InitConstantBuffer();


private:


};

#endif