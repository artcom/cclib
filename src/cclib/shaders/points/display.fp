uniform samplerRECT infoTexture : TEXUNIT0;

void main(
	in float4 iColor : COLOR,
	in float2 iTexCoord : TEXCOORD0,
	out float4 oColor : COLOR0
){
	float4 myValues = texRECT(infoTexture, iTexCoord);
	float myAlpha = saturate(1 - myValues.x / myValues.y);
	//oColor = float4(iColor.xyz, iColor.w * pow(myAlpha,4));
	oColor = iColor;
	//oColor = float4(1,1,1, 1);
}