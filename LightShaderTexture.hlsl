struct ConstantData
{
    float4x4 WorldViewProj;
    float4x4 World;
    float4 ViewerPos;
};

cbuffer ConstBuf : register(b0) {
    ConstantData ConstData;
}

struct LightData
{
    float4 Direction;
    float4 Color;
    float4 KaSpecPowKsX; // Enhancing the specular of the Materials
};

cbuffer LightBuf : register(b1)
{
    LightData Lights;
}

struct VS_IN {
    float4 pos : POSITION;
    float4 tex : TEXCOORD;
    float4 normal : NORMAL;
};

struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD0;
    float4 normal : NORMAL;
    float4 worldPos : TEXCOORD1;
};

Texture2D DiffuseMap : register(t0);
SamplerState Sampler : register(s0);

PS_IN VSMain(VS_IN input)
{
    PS_IN output = (PS_IN)0;

    output.pos = mul(float4(input.pos.xyz, 1.0f), ConstData.WorldViewProj);
    output.tex = input.tex.xy;
    output.normal = mul(float4(input.normal.xyz, 1.0f), ConstData.World);
    output.worldPos = mul(float4(input.pos.xyz, 1.0f), ConstData.World);

    return output;
}

float4 PSMain(PS_IN input, in bool isFrontFace : SV_IsFrontFace) : SV_Target
{
    float4 color = {0.0f, 1.0f, 0.0f, 1.0f};
//if (!iSFrontFace)
//{
    color = DiffuseMap.Sample(Sampler, float2(input.tex.x, 1.0f - input.tex.y));
    //}
    clip(color.a - 0.01f); // Discards the current pixel if the specified value is less than zero. to test for alpha behavior
    float3 lightDir = Lights.Direction.xyz;
    float3 normal = normalize(input.normal.xyz);
    float3 kd = color.xyz; // diffuse value for main color of material

    float3 diffuse = saturate(dot(lightDir, normal)) * kd; // saturate - Clamps the specified value within the range of 0 to 1.

    float3 ambient = kd * Lights.KaSpecPowKsX.x; // global background light

    float3 viewDir = normalize(ConstData.ViewerPos.xyz - input.worldPos.xyz);
    float3 refVec = normalize(reflect(lightDir, normal));

    float3 spec = pow(max(0, dot(-viewDir, refVec)), Lights.KaSpecPowKsX.y) * Lights.KaSpecPowKsX.z;
    color = float4(Lights.Color.xyz * (diffuse + ambient + spec), 1.0f);
    return color;
}

