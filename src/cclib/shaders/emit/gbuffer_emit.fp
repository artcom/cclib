uniform samplerRECT positionTexture;
uniform samplerRECT velocityTexture;
uniform samplerRECT infoTexture;
uniform samplerRECT colorTexture;
uniform samplerRECT geometryTexture;
uniform samplerRECT geometryColorTexture;

uniform float2 gBufferSize;
uniform float3 randomSeed;
uniform float emitProb;
uniform float minLifeTime;
uniform float maxLifeTime;
uniform float lifeTimeSpreadPow;
uniform float channelBlend;

uniform float4x4 inverseView;

uniform float illuminationMin;
uniform float illuminationMax;
uniform float illuminationPow;
uniform float illuminationAmount;

uniform float occlusionMin;
uniform float occlusionMax;
uniform float occlusionPow;
uniform float occlusionAmount;

uniform float shadowMin;
uniform float shadowMax;
uniform float shadowPow;
uniform float shadowAmount;

uniform float colorStart;
uniform float colorEnd;
uniform float colorRandom;

uniform float colorSaturation;
uniform float colorBrightness;
uniform float colorAlpha;

float rand(float2 n){
  return fract(sin(dot(n.xy, randomSeed.xy))* randomSeed.z);
}

float4 hsv_to_rgb(float h, float s, float v, float a) {
	float c = v * s;
	h = mod((h * 6.0), 6.0);
	float x = c * (1.0 - abs(mod(h, 2.0) - 1.0));
	vec4 color;
 
	if (0.0 <= h && h < 1.0) {
		color = vec4(c, x, 0.0, a);
	} else if (1.0 <= h && h < 2.0) {
		color = vec4(x, c, 0.0, a);
	} else if (2.0 <= h && h < 3.0) {
		color = vec4(0.0, c, x, a);
	} else if (3.0 <= h && h < 4.0) {
		color = vec4(0.0, x, c, a);
	} else if (4.0 <= h && h < 5.0) {
		color = vec4(x, 0.0, c, a);
	} else if (5.0 <= h && h < 6.0) {
		color = vec4(c, 0.0, x, a);
	} else {
		color = vec4(0.0, 0.0, 0.0, a);
	}
 
	color.rgb += v - c;
 
	return color;
}

float colorCorrection(float theValue, float theMin, float theMax, float theCurve){
	theValue-= theMin;
	theValue /= theMax - theMin;
	theValue = clamp(theValue, 0.0, 1.0);
	return pow(theValue, theCurve);
}

void main (
	in 	float2 texID : WPOS,
	out float4 newPosition : COLOR0,
	out float4 newInfo : COLOR1,
	out float3 newVelocity : COLOR2,
	out float4 newColor : COLOR3
){
	//float3 position = mul(inverseView, texRECT (positionTexture, texID));
	float3 position =  texRECT (positionTexture, texID);
	float4 velocity = texRECT (velocityTexture, texID);
	
	newVelocity = velocity;
	newColor = texRECT (colorTexture, texID);
	newInfo = texRECT(infoTexture, texID);
	
    float myAge = newInfo.x;
	
    if(myAge >= newInfo.y && newInfo.z == 0.0){
		position = float3(1000000,0,0);
		
		if(rand(texID) < emitProb){
			
            float2 myTexCoord = float2(rand(texID + float2(1234,0)), rand(texID + float2(3456,0)));
            float4 gBufferPosition = texRECT(geometryTexture, myTexCoord.xy * gBufferSize.xy);
            float4 gBufferColor = texRECT(geometryColorTexture, myTexCoord.xy * gBufferSize.xy);
            float myBrightNess = lerp(gBufferColor.r, gBufferColor.g, channelBlend);

            float myIllumination = colorCorrection(gBufferColor.r, illuminationMin, illuminationMax, illuminationPow);
            float myOcclusion = colorCorrection(gBufferColor.g, occlusionMin, occlusionMax, occlusionPow);
            float myShadow = colorCorrection(gBufferColor.b, shadowMin, shadowMax, shadowPow);

            if(length(gBufferPosition) > 1) {// && myBrightNess >= 0.5){
                float lifeTimeRand = rand(texID + float2(5678,0));
                float lifeTime = 4; // minLifeTime;
                if(lifeTimeRand > lifeTimeSpreadPow) {
                    lifeTime = maxLifeTime;
                }

                position = gBufferPosition;
                newInfo.x = 0;
                newInfo.y = lifeTime;
                newInfo.z = 0;
                newVelocity = float3(0,0,0);//rand(texID + float2(3000,0)

                float myColorBlend = lerp(gBufferColor.r, rand(texID + float2(2345,0)), colorRandom);
                myColorBlend = lerp(colorStart, colorEnd, myColorBlend);

                newColor = hsv_to_rgb(
                        myColorBlend,
                        rand(texID + float2(2346, 0.0)) * 0.1 + colorSaturation, 
                        colorBrightness, colorAlpha) * lerp(1.0, myIllumination, illuminationAmount) *
                                                       lerp(1.0, myOcclusion, occlusionAmount) *
                                                       lerp(1.0, myShadow, shadowAmount);  
            }
        }
    }
	
	//float2 myTexCoord = float2(rand(texID + float2(3000,0)),rand(texID + float2(4000,0)));
	//position = tex2D(geometryTexture,myTexCoord);
	position.x = position.x + 0.001 * (rand(texID + float2(3000, 0)) - 0.5);
	position.y = position.y + 0.001 * (rand(texID + float2(4000, 0)) - 0.5);
    newPosition = float4(position,1); 
}
