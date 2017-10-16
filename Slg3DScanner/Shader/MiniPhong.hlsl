#ifndef MINIPHONG_HLSL_INCLUDED
#define MINIPHONG_HLSL_INCLUDED



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
	float3 normal : TEXCOORD0;
	float2 uv : TEXCOORD1;
	
	float3 eyeVector : TEXCOORD2;
};


OutputVS mainVS(float4 position : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	OutputVS output;

	output.position = mul(position, matWorldViewProj);
	output.normal = mul(normal, matWorld);
	output.uv = uv;
	
	output.eyeVector = normalize(output.position - vCamera);

	return output;
}

float4 mainPS(OutputVS incoming) : SV_Target
{
	float4 finalColor = vATotal;
	
	float thetaAngle = dot(incoming.normal, lightVector.xyz);
	
	if(thetaAngle > 0.f)
	{
		float3 specularIntermediary = normalize(2 * thetaAngle * incoming.normal - lightVector.xyz);
		
		float specularCoeff = pow(dot(specularIntermediary, incoming.eyeVector), 12);
		
		finalColor += vDTotal * thetaAngle + vSTotal * specularCoeff;
	}

	return finalColor;
}

#endif