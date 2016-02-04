
//�O�p�`�̏��ƌ��ʂ�Ԃ��p
struct ShpereTriangleInfo
{
	float3 v1;
	float3 v2;
	float3 v3;
	float3 normal;
	float3 contactPos;
	float dist;
	bool isHit;
};

//�����̏��
cbuffer SphereInfo: register(b0)
{
	float4 g_Point;
	float g_Radius;
};

StructuredBuffer<ShpereTriangleInfo> BufferInput : register(t0); //�󂯎��p
RWStructuredBuffer<ShpereTriangleInfo> BufferOut : register(u0); //�ʂ�Ԃ��p

float LengthSq(float3 v)
{
	return (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
}

bool TriangleIntersect(float3 pointPos, float3 v1, float3 v2, float3 v3, float3 normal)
{
	float3 cross1 = cross(v2 - v1, pointPos - v1);
	float3 cross2 = cross(v3 - v2, pointPos - v2);
	float3 cross3 = cross(v1 - v3, pointPos - v3);

	//�S�Ă̊O�ς̌��������������Ȃ�_�͎O�p�`��ɗL��
	if (dot(cross1, normal) > 0.0f) return false;
	if (dot(cross2, normal) > 0.0f) return false;
	if (dot(cross3, normal) > 0.0f) return false;

	return true;
}

float3 PointOnLineSegmentNearestPoint(float3 start, float3 end, float3 pointPos)
{
	float3 v = end - start;
	float l = LengthSq(v);

	//�����̎n�_�ƏI�_�������ꏊ
	if (l == 0) return start;

	float3 vp = pointPos - start;
	float t = dot(v, vp) / l;

	if (t < 0.0f) return start; //�n�_��艺�Ȃ���n�_
	if (t > 1.0f) return end;	//�I�_����Ȃ���I�_	

	//t=<1 t>=0 �̏ꍇ�͐�����ɋ߂��_������ 
	return start + v * t;
}

[numthreads(1, 1, 1)]

void CS(uint3 id : SV_DispatchThreadID)
{
	float3 v1 = BufferInput[id.x].v1;
	float3 v2 = BufferInput[id.x].v2;
	float3 v3 = BufferInput[id.x].v3;
	float3 normal = BufferInput[id.x].normal;

	float3 shperePoint = (float3)g_Point;
	float radius = g_Radius;

	BufferOut[id.x].isHit = false;

	//�O�p�`�̒��_���狅�̒��S�܂ł̃x�N�g��
	float3 VP = shperePoint - v1;

	//�_����O�p�`�܂ł̋���
	float dist = abs(dot(VP, normal));
	if (dist > radius) return;

	//�_����O�p�`�ɐ����ɉ��낵���n�_�����߂�i�O�p�`��̓_�̈ʒu���Z�o�j
	float3 TrianglePoint = shperePoint - (normal * dist);

	if (TriangleIntersect(TrianglePoint, v1, v2, v3, normal))
	{
		BufferOut[id.x].v1 = v1;
		BufferOut[id.x].v2 = v2;
		BufferOut[id.x].v3 = v3;
		BufferOut[id.x].normal = normal;
		BufferOut[id.x].contactPos = TrianglePoint;
		BufferOut[id.x].dist = dist;
		BufferOut[id.x].isHit = true;
	}

	float l;
	float lengihSq;
	float3 pos;

	//��1
	TrianglePoint = PointOnLineSegmentNearestPoint(v1, v2, shperePoint);
	l = lengihSq = LengthSq(shperePoint - TrianglePoint);
	pos = TrianglePoint;

	//��2
	TrianglePoint = PointOnLineSegmentNearestPoint(v2, v3, shperePoint);
	lengihSq = LengthSq(shperePoint - TrianglePoint);
	if (l > lengihSq)
	{
		l = lengihSq;
		pos = TrianglePoint;
	}

	//��3
	TrianglePoint = PointOnLineSegmentNearestPoint(v3, v1, shperePoint);
	lengihSq = LengthSq(shperePoint - TrianglePoint);
	if (l > lengihSq)
	{
		l = lengihSq;
		pos = TrianglePoint;
	}

	if (l <= radius * radius)
	{
		BufferOut[id.x].v1 = v1;
		BufferOut[id.x].v2 = v2;
		BufferOut[id.x].v3 = v3;
		BufferOut[id.x].normal = normal;
		BufferOut[id.x].contactPos = TrianglePoint;
		BufferOut[id.x].dist = sqrt(l);
		BufferOut[id.x].isHit = true;
	}
}


















