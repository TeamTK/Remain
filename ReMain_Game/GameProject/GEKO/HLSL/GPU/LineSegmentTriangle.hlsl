
//三角形の情報と結果を返す用
struct TriangleInfo
{
	float3 v1;
	float3 v2;
	float3 v3;
	float3 normal;
	float3 contactPos;
	bool isHit;
};

//線分の情報
cbuffer lineInfo: register(b0)
{
	float4 g_Start;
	float4 g_End;
};

StructuredBuffer<TriangleInfo> BufferInput : register(t0); //受け取り用
RWStructuredBuffer<TriangleInfo> BufferOut : register(u0); //果を返す用

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

[numthreads(1, 1, 1)]

void CS(uint3 id : SV_DispatchThreadID)
{
	float3 v1 = BufferInput[id.x].v1;
	float3 v2 = BufferInput[id.x].v2;
	float3 v3 = BufferInput[id.x].v3;
	float3 normal = BufferInput[id.x].normal;

	float3 start = (float3)g_Start;
	float3 end = (float3)g_End;

	BufferOut[id.x].isHit = false;

	//ポリゴンの法線との内積を求める
	float d1 = dot(start - v1, normal);
	float d2 = dot(end - v1, normal);

	//d1とd2が符号が同じなら交差していない
	if (d1 * d2 > 0) return;

	//始点からポリゴンまでの距離と線分の長さの比率を求める
	float t = d1 / (d1 - d2);
	if (t < 0) return;

	//線分と三角形の接触点を求める
	float3 pointPos = start + (end - start) * t;
	
	if (!TriangleIntersect(pointPos, v1, v2, v3, normal)) return;

	//結果を格納
	BufferOut[id.x].v1 = v1;
	BufferOut[id.x].v2 = v2;
	BufferOut[id.x].v3 = v3;
	BufferOut[id.x].normal = normal;
	BufferOut[id.x].contactPos = pointPos;
	BufferOut[id.x].isHit = true;
}


















