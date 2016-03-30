#pragma warning(disable : 3206)

//全てのメッシュ共通情報
cbuffer CommonInfo : register(b0)
{
	float4 g_LightDir;  //ライトの方向ベクトル
	float4 g_Intensity; //ディレクショナルライト情報（ライトの明るさ）
	float4 g_Eye;		//カメラ位置
};

//変換行列やライト
cbuffer global_0:register(b1)
{
	matrix g_mW;		//ワールド行列
	matrix g_mWVP;		//ワールドから射影までの変換行列
};

//マテリアル
cbuffer global_1:register(b2)
{
	float4 g_Ambient; //アンビエント光
	float4 g_Diffuse; //拡散反射(色）
	float4 g_Specular; //鏡面反射
};

//バーテックスシェーダー出力構造体
struct VS_OUTPUT
{	
	float4 Pos : SV_POSITION;
	float3 Light : TEXCOORD0;
	float3 Normal : TEXCOORD1;
	float3 EyeVector : TEXCOORD2;
};

//バーテックスシェーダー
VS_OUTPUT VS_NoTexture(float4 Pos : POSITION, float4 Normal : NORMAL)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	//射影変換（ワールド→ビュー→プロジェクション）
	output.Pos = mul(Pos, g_mWVP);

	//法線をモデルの姿勢に合わせる(モデルが回転すれば法線も回転させる）
	output.Normal = mul(Normal, (float3x3)g_mW);

	//ディレクショナルライト
	output.Light = -g_LightDir.xyz;

	//視線ベクトル　ワールド空間上での頂点から視点へ向かうベクトル
	float4 PosWorld = mul(Pos, g_mW);
	output.EyeVector = normalize(PosWorld - g_Eye).xyz;

	return output;
}

//ピクセルシェーダー
float4 PS_NoTexture( VS_OUTPUT input ) : SV_Target
{
	//環境光　項
	float4 ambient = g_Ambient;

	//拡散反射光　項
	float NL = max(dot(input.Normal, input.Light), 0.0f);
	float Half = NL * 0.5f + 0.5f;
	float4 diffuse = g_Diffuse * (Half * Half);

	//鏡面反射光　項
	float3 reflect = normalize(2 * NL*input.Normal - input.Light);
	float4 specular = pow(max(dot(reflect, input.EyeVector), 0.0f), 4) * g_Specular;

	//フォンモデル最終色　３つの項の合計
	float4 color = ambient + diffuse + specular;

	color.rgb *= g_Intensity.rgb; //ライトの色反映
	color *= g_Intensity.w; //ライト強度を反映
	color.a = g_Diffuse.w; //アルファ値反映

	return color;
}