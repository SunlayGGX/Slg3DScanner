#ifndef MINIPHONGCLOUD_HLSL_INCLUDED
#define MINIPHONGCLOUD_HLSL_INCLUDED



cbuffer param
{ 
	float4x4 matWorldViewProj;   // la matrice totale 
	float4 vCamera; 			// la position de la caméra
	float4 lightVector; 		// la position de la source d'éclairage (Point)
	
	float4 vATotal; 			// total ambiant value
	float4 vDTotal; 			// total ambiant value
	float4 vSTotal; 			// total ambiant value
	
	float4x4 matWorld;		// matrice de transformation dans le monde 
}


struct OutputVS
{
	float4 position : SV_Position;
	
	float3 eyeVector : TEXCOORD0;
};


OutputVS mainVS(float3 position : POSITION)
{
	OutputVS output;

	output.position = mul(float4(position.xyz, 1.f), matWorldViewProj);
	
	output.eyeVector = normalize(output.position - vCamera);

	return output;
}

float4 mainPS(OutputVS incoming) : SV_Target
{
	float4 finalColor = vATotal;
	return finalColor;
}

#endif