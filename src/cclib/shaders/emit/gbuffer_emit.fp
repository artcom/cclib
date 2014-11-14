uniform samplerRECT positionTexture;
uniform samplerRECT velocityTexture;
uniform samplerRECT infoTexture;
uniform samplerRECT colorTexture;

uniform sampler2D geometryTexture;
uniform sampler2D geometryColorTexture;

uniform float3 randomSeed;
uniform float emitProb;
uniform float minLifeTime;
uniform float maxLifeTime;
uniform float positionJitter;

float rand(float2 n){
  return fract(sin(dot(n.xy, randomSeed.xy)) * randomSeed.z);
}

void main (
	in 	float2 texID : WPOS,
	out float4 newPosition : COLOR0,
	out float4 newInfo : COLOR1,
	out float3 newVelocity : COLOR2,
	out float4 newColor : COLOR3
){
	float3 position =  texRECT (positionTexture, texID);
	float4 velocity = texRECT (velocityTexture, texID);
	
	newVelocity = velocity;
	newColor = texRECT (colorTexture, texID); // make it stop working
	newInfo = texRECT(infoTexture, texID); // WORKS

    float myAge = newInfo.x;
	
    if(myAge >= newInfo.y && newInfo.z == 0.0){
		position = float3(1000000, 0, 0);
		
		if(rand(texID) < emitProb) {
			
            float2 myTexCoord = float2(rand(texID + float2(1234,0)), rand(texID + float2(3456,0)));

            float4 gBufferPosition = tex2D(geometryTexture, myTexCoord);
            float4 gBufferColor = tex2D(geometryColorTexture, myTexCoord);

            if (gBufferPosition.a > 0) {
                float lifeTimeRand = rand(texID + float2(5678,0));
                float lifeTime =  minLifeTime + (maxLifeTime - minLifeTime) * lifeTimeRand;

                position = gBufferPosition;
                newInfo.x = 0;
                newInfo.y = lifeTime;
                newInfo.z = 0;
                newVelocity = float3(0, 0, 0);

                newColor = gBufferColor;

                // float myColorBlend = lerp(gBufferColor.r, rand(texID + float2(2345,0)), colorRandom);
                // myColorBlend = lerp(colorStart, colorEnd, myColorBlend);

                // newColor = hsv_to_rgb(
                //         myColorBlend,
                //         rand(texID + float2(2346, 0.0)) * 0.1 + colorSaturation, 
                //         colorBrightness, colorAlpha) * lerp(1.0, myIllumination, illuminationAmount) *
                //                                        lerp(1.0, myOcclusion, occlusionAmount) *
                //                                        lerp(1.0, myShadow, shadowAmount);  
            }
        }
    }

    // add a slight jitter to avoid moiree effects
	position.x = position.x + positionJitter * (rand(texID + float2(3000, 0)) - 0.5);
	position.y = position.y + positionJitter * (rand(texID + float2(4000, 0)) - 0.5);
    newPosition = float4(position, 1); 
}
