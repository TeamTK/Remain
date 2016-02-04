
//三角形の情報と結果を返す用
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

//線分の情報
cbuffer SphereInfo: register(b0)
{
	float4 g_Point;
	float g_Radius;
};

StructuredBuffer<ShpereTriangleInfo> BufferInput : register(t0); //受け取り用
RWStructuredBuffer<ShpereTriangleInfo> BufferOut : register(u0); //果を返す用

float LengthSq(float3 v)
{
	return (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
}

bool TriangleIntersect(float3 pointPos, float3 v1, float3 v2, float3 v3, float3 normal)
{
	float3 cross1 = cross(v2 - v1, pointPos - v1);
	float3 cross2 = cross(v3 - v2, pointPos - v2);
	float3 cross3 = cross(v1 - v3, pointPos - v3);

	//全ての外積の向きが同じ方向なら点は三角形上に有る
	if (dot(cross1, normal) > 0.0f) return false;
	if (dot(cross2, normal) > 0.0f) return false;
	if (dot(cross3, normal) > 0.0f) return false;

	return true;
}

float3 PointOnLineSegmentNearestPoint(float3 start, float3 end, float3 pointPos)
{
	float3 v = end - start;
	float l = LengthSq(v);

	//線分の始点と終点が同じ場所
	if (l == 0) return start;

	float3 vp = pointPos - start;
	float t = dot(v, vp) / l;

	if (t < 0.0f) return start; //始点より下ならを始点
	if (t > 1.0f) return end;	//終点より上ならを終点	

	//t=<1 t>=0 の場合は線分上に近い点がある 
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

	//三角形の頂点から球の中心までのベクトル
	float3 VP = shperePoint - v1;

	//点から三角形までの距離
	float dist = abs(dot(VP, normal));
	if (dist > radius) return;

	//点から三角形に垂直に下ろした地点を求める（三角形上の点の位置を算出）
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

	//辺1
	TrianglePoint = PointOnLineSegmentNearestPoint(v1, v2, shperePoint);
	l = lengihSq = LengthSq(shperePoint - TrianglePoint);
	pos = TrianglePoint;

	//辺2
	TrianglePoint = PointOnLineSegmentNearestPoint(v2, v3, shperePoint);
	lengihSq = LengthSq(shperePoint - TrianglePoint);
	if (l > lengihSq)
	{
		l = lengihSq;
		pos = TrianglePoint;
	}

	//辺3
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


















