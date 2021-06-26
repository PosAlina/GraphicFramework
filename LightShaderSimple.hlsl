struct ConstantData
{
    float4x4 WorldViewProj;
    float4x4 World;
    float4 ViewerPos;
//};
//
//
//
//struct LightData
//{
    float4 Direction;
    float4 Color;
    float4 KaSpecPowKsX; // Enhancing the specular of the Materials
};

//cbuffer LightBuf : register(b1)
//{
//    LightData Lights;
//}

cbuffer ConstBuf : register(b0) {
    ConstantData ConstData;
}

struct VS_IN {
    float4 pos : POSITION;
    float4 col : COLOR;
    float4 normal : NORMAL;
};

struct PS_IN
{
    float4 pos : SV_POSITION;
    float4 col : COLOR;
    float4 normal : NORMAL;
    float4 worldPos : TEXCOORD1;
};

//Texture2D DiffuseMap : register(t0);
//SamplerState Sampler : register(s0);

PS_IN VSMain(VS_IN input)
{
    PS_IN output = (PS_IN)0;

    output.pos = mul(float4(input.pos.xyz, 1.0f), ConstData.WorldViewProj);
    output.col = input.col;
    output.normal = mul(float4(input.normal.xyz, 1.0f), ConstData.World);
    output.worldPos = mul(float4(input.pos.xyz, 1.0f), ConstData.World);

    return output;
}

float4 PSMain(PS_IN input, in bool isFrontFace : SV_IsFrontFace) : SV_Target
{
    float4 color = input.col;
    clip(color.a - 0.01f); // Discards the current pixel if the specified value is less than zero. to test for alpha behavior
    float3 lightDir = ConstData.Direction.xyz;
    float3 normal = normalize(input.normal.xyz);

    float3 ambientLightColor = color.xyz; // diffuse value for main color of material
    //float3 ambientLightColor = float3(0.8f, 0.8f, 0.8f);
    float3 ambient = ambientLightColor * Lights.KaSpecPowKsX.x; // global background light

    //float3 diffuse = saturate(dot(lightDir, normal))* ambientLightColor; // saturate - Clamps the specified value within the range of 0 to 1.
   // float diff = max(dot(lightDir, normal), 0.0f);
    float3 diffuse = saturate(dot(lightDir, normal)) * ambientLightColor;// ambientLightColor* diff;

    float3 viewDir = normalize(ConstData.ViewerPos.xyz - input.worldPos.xyz);
    float3 refVec = normalize(reflect(lightDir, normal));

    float3 specular = pow(max(0, dot(-viewDir, refVec)), ConstData.KaSpecPowKsX.y) * ConstData.KaSpecPowKsX.z;
    //color = float4(Lights.Color.xyz * (diffuse + ambient + spec), 1.0f);
   // float specularStrength = 0.5f;
    //float spec = pow(max(0.0f, dot(viewDir, refVec)), 32);
   // float3 specular = specularStrength * spec * ambientLightColor;

    return float4(color.xyz * (ambient.xyz + diffuse.xyz + specular.xyz), 1.0f);
}

