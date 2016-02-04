
//�O�p�`�̏��ƌ��ʂ�Ԃ��p
struct TriangleInfo
{
	float3 v1;
	float3 v2;
	float3 v3;
	float3 normal;
	float3 contactPos;
	bool isHit;
};

//�����̏��
cbuffer lineInfo: register(b0)
{
	float4 g_Start;
	float4 g_End;
};

StructuredBuffer<TriangleInfo> BufferInput : register(t0); //�󂯎��p
RWStructuredBuffer<TriangleInfo> BufferOut : register(u0); //�ʂ�Ԃ��p

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

	//�|���S���̖@���Ƃ̓��ς����߂�
	float d1 = dot(start - v1, normal);
	float d2 = dot(end - v1, normal);

	//d1��d2�������������Ȃ�������Ă��Ȃ�
	if (d1 * d2 > 0) return;

	//�n�_����|���S���܂ł̋����Ɛ����̒����̔䗦�����߂�
	float t = d1 / (d1 - d2);
	if (t < 0) return;

	//�����ƎO�p�`�̐ڐG�_�����߂�
	float3 pointPos = start + (end - start) * t;
	
	if (!TriangleIntersect(pointPos, v1, v2, v3, normal)) return;

	//���ʂ��i�[
	BufferOut[id.x].v1 = v1;
	BufferOut[id.x].v2 = v2;
	BufferOut[id.x].v3 = v3;
	BufferOut[id.x].normal = normal;
	BufferOut[id.x].contactPos = pointPos;
	BufferOut[id.x].isHit = true;
}


















