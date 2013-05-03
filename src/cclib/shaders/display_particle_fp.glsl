//uniform sampler2DRect infoTexture;
uniform sampler2D colorSampler;

void main(){
	//vec4 myValues = texture2DRect(infoTexture, gl_TexCoord[0].xy);
	//float myAlpha = clamp(1.0 - myValues.x / myValues.y, 0.0, 1.0);
	//oColor = float4(iColor.xyz, iColor.w * pow(myAlpha,4));
	gl_FragColor = gl_Color * texture2D(colorSampler,gl_TexCoord[0].xy);//
	//gl_FragColor = vec4(1.0,0.0,0.0, 1.0);
}