void main(
	in float4 value0 : TEXCOORD0,
	in float4 value1 : TEXCOORD1,
	in float4 value2 : TEXCOORD2,
	out float4 color0 : COLOR0,
	out float4 color1 : COLOR1,
	out float4 color2 : COLOR2
){
	color0 = value0;
	color1 = value1;
	color2 = value2;
}
