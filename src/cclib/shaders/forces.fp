uniform samplerRECT positionTexture;
uniform samplerRECT velocityTexture;
uniform samplerRECT infoTexture;

interface Force{
	float3 force(float3 thePosition, float3 theVelocity, float2 theTexID, float theDeltaTime);
};

struct Gravity : Force{
	float3 gravity;
	
	float strength;
	
	float3 force(float3 thePosition, float3 theVelocity, float2 theTexID, float theDeltaTime){
		return float3(gravity) * strength; 
	}
};

struct ViscousDrag : Force{
	float coefficient;
	
	float strength;
	
	float3 force(float3 thePosition, float3 theVelocity, float2 theTexID, float theDeltaTime){
		return theVelocity * theDeltaTime * -coefficient * strength; 
	}
};

struct Attractor : Force{
	float3 position;
	float strength;
	float radius;
	
	float3 force(float3 thePosition, float3 theVelocity, float2 theTexID, float theDeltaTime){
		float3 force = position - thePosition;
        float dist = length(force);
		
		if (dist < radius) {
			float myFallOff = 1f - dist / radius;
			float myForce = myFallOff * myFallOff * strength;
			//force -= theVelocity;
            force = force * myForce / dist;
            return force;
        } else {
        	return float3(0,0,0);
        }
	}
};

/* 
 * Force to move particles on a terrain
 */
struct TerrainForce : Force{
	float strength;
	samplerRECT texture;
	
	float2 textureSize;
	float3 scale;
	float3 offset;
	
	float exponent;

	float3 force(float3 thePosition, float3 theVelocity, float2 theTexID, float theDeltaTime){

		float3 fPosition = thePosition + theVelocity * theDeltaTime;
		
		float2 terrainPos = fPosition.xz / scale.xz + offset.xz;
		float height = texRECT(texture, terrainPos);
			
		float displacement = fPosition.y - height * scale.y + offset.y + theVelocity.y;
		
		return float3(0,clamp(-displacement,-1,1),0);
	}
};

struct NoiseHeightmapForce : Force{
	float noiseScale;
	float strength;
	float height;
	float3 noiseOffset;
	
	float3 force(float3 thePosition, float3 theVelocity, float2 theTexID, float theDeltaTime){
		float3 fPosition = thePosition + theVelocity * theDeltaTime;
		float3 noisePosition = (fPosition) * noiseScale + noiseOffset;
		
		float displacement =  fPosition.y - noise(noisePosition.xz) * height +  + theVelocity.y;
		return float3(0,clamp(-displacement,-1,1),0) * strength;
	}
};

struct NoiseForceField : Force{
	float noiseScale;
	float strength;
	float3 noiseOffset;
	
	float noiseLengthScales[3];
	float noiseGains[3];
	
	float3 force(float3 thePosition, float3 theVelocity, float2 theTexID, float theDeltaTime){
		float3 noisePosition = (thePosition + theVelocity) * noiseScale + noiseOffset;
		float3 result = float3(
			snoise(noisePosition),
			snoise(noisePosition+100),
			snoise(noisePosition+200)
		);
		noisePosition = (thePosition + theVelocity) * noiseScale + noiseOffset.yzx;
		result += float3(
			snoise(noisePosition),
			snoise(noisePosition+100),
			snoise(noisePosition+200)
		);
		//result *= 50 * theDeltaTime;
		return result * strength;
	}
};

/*
struct NoiseForceField : Force{
	float noiseScale;
	float strength;
	float3 noiseOffset;
	
	float noiseLengthScales[3];
	float noiseGains[3];
	
	float noise0(float3 p) {
		return snoise(p);
	}

	float noise1(float3 p) {
		return snoise(float3(p.y + 31.416, p.z - 47.853, p.x + 12.793));
	}

	float noise2(float3 p) {
		return snoise(float3(p.z - 233.145, p.x - 113.408, p.y - 185.31));
	}
	
	float3 force(float3 thePosition, float3 theVelocity, float2 theTexID, float theDeltaTime){
		float3 noisePosition = (thePosition + theVelocity) * noiseScale + noiseOffset;
		float3 result = float3(0,0,0);
		
		 //float d=distance_and_normal(x, y, z, normal);
		// add turbulence octaves that respect boundaries, increasing upwards
		for (int i = 0; i < noiseLengthScales.length; i++) {
			result += float3(noise0(noisePosition), noise1(noisePosition), noise2(noisePosition)) * noiseGains[i];
		}

		return result * strength;
	}
};
*/

struct CurveForceFieldFollow : Force{
	float strength;
	float prediction;
	
	float radius;
	
	float offset;
	float scale;
	float outputScale;
	
	float useNoiseTexture;
	samplerRECT noiseTexture;
	float noiseTextureSize;

	float3 curveAtPoint(float x){

		float y=0;
		float z=0;

		if(useNoiseTexture == 1)
		{
			// convert x from modelview space to texture space
			float x_ = ((noiseTextureSize - x) * noiseTextureSize) / (noiseTextureSize*2.);

			float4 noise = texRECT(noiseTexture, float2(x_,0));

			y = outputScale * (noise.x - 0.25);
			z = outputScale * (noise.y - 0.25);
		}
		else
		{
			y = outputScale * (snoise(float2(x * scale + offset,0)));
			z = outputScale * (snoise(float2(x * scale + offset + 100, 0)));
		}

		return float3(x, y, z);
	}
	
	float3 flowAtPoint(float3 position) {
		float3 result = float3(0,0,0);
		
		float3 myCurvePoint = curveAtPoint(position.x);
		float curveDistance = distance(myCurvePoint, position);
		
		if(curveDistance > radius * 2){
			result = (myCurvePoint - position) / curveDistance;
		
		}else if(curveDistance > radius && curveDistance <= radius * 2){
			float blend = (curveDistance - radius) / radius;
			result = result * (1 - blend) + (myCurvePoint-position) / curveDistance * blend;
		}
	
		return result;
	}
	
	// called from from velocity.fp
	float3 force(float3 thePosition, float3 theVelocity, float2 theTexID, float theDeltaTime){

		float3 futurePosition = thePosition + theVelocity * prediction;
		float3 result = flowAtPoint(futurePosition) * strength;
		return result;
	}
};
/*
///////////////////////////////
//
// SPHERE CURVE FORCE FIELD
//
///////////////////////////////

struct NoiseSphereForceField : ForceField{
	float noiseRadius;
	float3 position;
	
	float3 flowAtPoint(float3 thePosition, float3 noiseOffset, float noiseScale) {
		float3 noisePosition = thePosition * noiseScale + noiseOffset;
		float3 result = float3(snoise(noisePosition), snoise(noisePosition+100), snoise(noisePosition+200));
		
		float sphereDistance = distance(position,thePosition);
		
		if(sphereDistance > noiseRadius * 2){
			result = (position - thePosition) / sphereDistance;
		}else if(sphereDistance > noiseRadius && sphereDistance <= noiseRadius * 2){
			float blend = (sphereDistance - noiseRadius) / noiseRadius;
			result = result * (1 - blend) + (position-thePosition) / sphereDistance * blend;
		}
	
		
		return result;
	}
};

struct SphereForceFieldFollow : Force{
	float noiseScale;
	float strength;
	float prediction;
	float3 noiseOffset;
	
	NoiseSphereForceField noiseSphereField;
	
	float3 force(float3 thePosition, float3 theVelocity, float2 theTexID, float theDeltaTime){
		float3 noisePosition = (thePosition + normalize(theVelocity) * prediction);
		float3 result = noiseSphereField.flowAtPoint(noisePosition, noiseOffset, noiseScale);
		result *= 100 * theDeltaTime * strength;
		return result;
	}
};*/

struct TextureForceFieldXZ : Force{
	samplerRECT texture;
	
	float2 textureSize;
	float2 textureScale;
	float2 textureOffset;
	
	float strength;
	
	float3 force(float3 thePosition, float3 theVelocity, float2 theTexID, float theDeltaTime){
		float2 texturePos = thePosition.xz / textureScale.xy + textureOffset.xy;
		float3 force = texRECT(texture, texturePos);
		force *= 2;
		force -= 1;
		force.z = force.y;
		force.y = 0;
		
		return force * strength;
	}
};

struct TextureForceFieldXY : Force{
	samplerRECT texture;
	
	float2 textureSize;
	float2 textureScale;
	float2 textureOffset;
	
	float strength;
	
	float3 force(float3 thePosition, float3 theVelocity, float2 theTexID, float theDeltaTime){
		float2 texturePos = thePosition.xy / textureScale.xy + textureOffset.xy;
		//texturePos.y = textureSize.y - texturePos.y;
		float3 force = texRECT(texture, texturePos);
		force *= 2;
		force -= 1;
		force.z = 0;
		
		return force * strength;
	}
};

struct Texture3DForceField : Force{
	sampler3D texture;
	
	float3 textureScale;
	float3 textureOffset;
	
	float strength;
	
	float3 force(float3 thePosition, float3 theVelocity, float2 theTexID, float theDeltaTime){
		float3 texturePos = (thePosition - textureOffset) / textureScale;
		float3 force = tex3D(texture, texturePos);
		if(texturePos.x >= 1 || texturePos.y >= 1 || texturePos.z >= 1 || texturePos.x <= 0 || texturePos.y <= 0 || texturePos.z <= 0)force = float3(0,0,0);
		return force * strength;
	}
};

struct FluidForceField : Force{
	samplerRECT texture;
	
	float2 textureSize;
	float2 textureScale;
	float2 textureOffset;
	
	float strength;
	
	float3 force(float3 thePosition, float3 theVelocity, float2 theTexID, float theDeltaTime){
		float2 texturePos = thePosition.xy / textureScale.xy + textureOffset.xy;
		//texturePos.y = textureSize.y - texturePos.y;
		float3 force = texRECT(texture, texturePos);
		force.z = 0;
		
		return force * strength;
	}
};

struct YForceBlend : Force{
	Force force1;
	Force force2;
	
    float y1;
    float y2;
	float blendWidth;

    float strength; // unused but needs to stay in there

	float3 force(float3 thePosition, float3 theVelocity, float2 theTexID, float theDeltaTime){
        
        float myBlend = 1.0;
        float y = thePosition.y;
       
        float range_half = (y2 - y1) * 0.5;
        float center = y1 + range_half;
        float dist = distance(center, y);
            
        myBlend = lerp(0.0, 1.0, (dist - range_half) / blendWidth); 
        myBlend = clamp(myBlend, 0.0, 1.0);

	    return lerp(
			force1.force(thePosition, theVelocity, theTexID, theDeltaTime),
			force2.force(thePosition, theVelocity, theTexID, theDeltaTime),
			myBlend);  // * strength;
	}
};

struct ForceBlend : Force{
	sampler2D texture;
	
	Force force1;
	Force force2;
	
	float2 dimension;
	
	float strength;
	float blend;

	float minBlend;
	float maxBlend;
	
	float3 force(float3 thePosition, float3 theVelocity, float2 theTexID, float theDeltaTime){
		float2 myTexID = theTexID / dimension;
		float myBlend = tex2D(texture, myTexID).x;
		myBlend = clamp(myBlend, 0, maxBlend);
		myBlend *= step(1 - myBlend, 1 - minBlend);
		return lerp(
			force1.force(thePosition, theVelocity, theTexID, theDeltaTime),
			force2.force(thePosition, theVelocity, theTexID, theDeltaTime),
			myBlend * blend // * applyX * applyY
		) * strength;
	}
};

struct TextureForceBlend : Force{
	samplerRECT texture;
	float2 textureScale;
	float2 textureOffset;
	
	Force force1;
	Force force2;
	
	float strength;
	float blend;
	
	float3 force(float3 thePosition, float3 theVelocity, float2 theTexID, float theDeltaTime){
		float2 texturePos = (thePosition.xy * float2(1,-1)) / textureScale + textureOffset.xy;
		
		return lerp(
			force1.force(thePosition, theVelocity, theTexID, theDeltaTime),
			force2.force(thePosition, theVelocity, theTexID, theDeltaTime),
			texRECT(texture, texturePos).x * blend // * applyX * applyY
		) * strength;
	}
};

struct IDTextureForceBlend : Force{
	samplerRECT texture;
	
	Force force1;
	Force force2;
	
	float strength;
	float blend;
	float power;
	
	
	float3 force(float3 thePosition, float3 theVelocity, float2 theTexID, float theDeltaTime){
		return lerp(
			force1.force(thePosition, theVelocity, theTexID, theDeltaTime),
			force2.force(thePosition, theVelocity, theTexID, theDeltaTime),
			texRECT(texture, theTexID).x * pow(blend,power) // * applyX * applyY
		) * strength;
	}
};

struct IDTextureBlendForce : Force{
	samplerRECT texture;
	
	Force force1;
	
	float strength;
	float power;
	
	float blendRangeStart;
	float blendRangeEnd;
	
	float3 force(float3 thePosition, float3 theVelocity, float2 theTexID, float theDeltaTime){
		float blend = texRECT(texture, theTexID).x;
		blend = smoothstep(blendRangeStart, blendRangeEnd, blend);
		return 
			force1.force(thePosition, theVelocity, theTexID, theDeltaTime) *
			texRECT(texture, theTexID).x * pow(blend,power) * strength;
	}
};


struct TexCoordTextureBlendForce : Force{
	samplerRECT texCoordsTexture;
	samplerRECT texture;
	
	Force force1;
	
	float strength;
	float power;
	
	float2 scale;
	float2 offset;
	
	int channel;
	
	float3 force(float3 thePosition, float3 theVelocity, float2 theTexID, float theDeltaTime){
		float4 texCoords = texRECT(texCoordsTexture, theTexID);
		return 
			force1.force(thePosition, theVelocity, theTexID, theDeltaTime) *
			pow(texRECT(texture, texCoords.xy * scale + offset).x,power) * strength;
	}
};

struct TimeForceBlend : Force{
	Force force1;
	Force force2;
	
	float strength;
	
	float start;
	float end;
	
	float power;
	
	samplerRECT blendInfos;
	
	float3 force(float3 thePosition, float3 theVelocity, float2 theTexID, float theDeltaTime){
		float4 timeInfo = texRECT(infoTexture, theTexID);
		float4 blendInfo = texRECT(blendInfos, float2(timeInfo.w,0));
		float time = timeInfo.x;
		time -= start;
		float timeBlend = clamp(time, 0, end - start) / (end - start);
		
		return lerp(
			force1.force(thePosition, theVelocity, theTexID, theDeltaTime),
			force2.force(thePosition, theVelocity, theTexID, theDeltaTime),
			clamp(pow(timeBlend,power), blendInfo.r,blendInfo.g)
		) * strength;
	}
};

struct SaudiTimeForceBlend : Force{
	Force force1;
	Force force2;
	Force force3;
	Force force4;
	
	float strength;
	
	float start;
	float end;
	
	float power;
	
	samplerRECT blendInfos;
	
	float3 force(float3 thePosition, float3 theVelocity, float2 theTexID, float theDeltaTime){
		float4 timeInfo = texRECT(infoTexture, theTexID);
		float4 blendInfo = texRECT(blendInfos, float2(timeInfo.w + 0.5,0.5));
		float timeBlend = timeInfo.x / timeInfo.y;
		//time -= start;
		//float timeBlend = clamp(time, 0, end - start) / (end - start);
		
		return lerp(
			force1.force(thePosition, theVelocity, theTexID, theDeltaTime) +
			force2.force(thePosition, theVelocity, theTexID, theDeltaTime) +
			force3.force(thePosition, theVelocity, theTexID, theDeltaTime),
			force4.force(thePosition, theVelocity, theTexID, theDeltaTime),
			clamp(pow(timeBlend,power), blendInfo.r,blendInfo.g)
		) * strength;
	}
};

struct CombinedForce : Force{
	Force forces[];
	
	float strength;

	float3 force(float3 thePosition, float3 theVelocity, float2 theTexID, float theDeltaTime){
		float3 result = float3(0,0,0);
		
		for(int i = 0; i < forces.length;i++){
		result += float3(1,0,0);
			result += forces[i].force(thePosition, theVelocity, theTexID, theDeltaTime);
		}
		return result * strength;
	}
};

struct StateSystem : Force{
	Force forces[];
	samplerRECT stateInfos;
	
	float strength;
	
	float3 force(float3 thePosition, float3 theVelocity, float2 theTexID, float theDeltaTime){
		float3 stateInfo = texRECT(stateInfos, theTexID);
		
		Force force1 = forces[(int)stateInfo.x];
		Force force2 = forces[(int)stateInfo.y];
		
		return lerp(
			force1.force(thePosition, theVelocity, theTexID, theDeltaTime),
			force2.force(thePosition, theVelocity, theTexID, theDeltaTime),
			stateInfo.z
		) * strength;
	}
};

struct TargetForce : Force{

	samplerRECT targetPositionTexture;
	
	float3 center;
	float scale;
	float strength;
	float lookAhead;
	float maxForce;
	float nearDistance;
	float nearMaxForce;

	float3 force(float3 thePosition, float3 theVelocity, float2 theTexID, float theDeltaTime){
	
		float4 targetInfos = texRECT (targetPositionTexture, theTexID);
		float3 target = targetInfos.xyz * scale + center;
		float targetStrength = targetInfos.w * strength;
		
		if(target.x == 0.0)return float3(0.0);
		//float factor = (-dot(normalize(target - thePosition),normalize(theVelocity)) + 1) / 2;
		float3 force = target - (thePosition + theVelocity * theDeltaTime * lookAhead * targetStrength);
		
		float distance = length(force);
		if(nearMaxForce > 0 && distance < nearDistance && distance > nearMaxForce){
			return force / distance * nearMaxForce * targetStrength;
		}
		if(maxForce > 0 && distance > maxForce){
			return force / distance * maxForce * targetStrength;
		}
		return force * targetStrength;// / (theDeltaTime * 60);
	}
};

struct MultiTargetForce : Force{

	samplerRECT targetPositionTexture;
	samplerRECT stateIDTexture;
	samplerRECT stateInfoTexture;
	
	float3 center;
	float strength;
	float lookAhead;
	float maxForce;
	
	float3 force(float3 thePosition, float3 theVelocity, float2 theTexID, float theDeltaTime){
	
		float4 stateIDs = texRECT(stateIDTexture, theTexID);
		float4 stateInfos = texRECT(stateInfoTexture, theTexID);
		
		float blend = stateInfos.z;
		
		float4 target1 = float4(0.0);
		
		if(stateInfos.x < 0){
			blend = 1.0;
		} else {
			target1.xyz = texRECT (targetPositionTexture, stateIDs.xy) + center;
			target1.w = 1.0;
		}
		
		float4 target2 = float4(0.0);
		if(stateInfos.y < 0){
			blend = 0.0;
		} else {
			target2.xyz = texRECT (targetPositionTexture, stateIDs.zw) + center;
			target2.w = 1.0;
		}
		
		float3 target = lerp(
			target1.xyz,
			target2.xyz,
			blend
		);
		
		//float factor = (-dot(normalize(target - thePosition),normalize(theVelocity)) + 1) / 2;
		float3 force = target - (thePosition + theVelocity * theDeltaTime * lookAhead * strength);
		
		float distance = length(force);
		if(maxForce > 0 && distance > maxForce)force = force / distance * maxForce;
		
		return force * strength * saturate(target1.w + target2.w);// / (theDeltaTime * 60);
	}
};

struct NearestTargetForce : Force{

	samplerRECT targetPositionTexture;
	
	float2 textureSize;
	float2 textureScale;
	float2 textureOffset;
	
	float targetTime;
	
	float strength;

	float3 force(float3 thePosition, float3 theVelocity, float2 theTexID, float theDeltaTime){
	
		float3 target = texRECT (targetPositionTexture, theTexID);
		float myUseTarget = target.z > 0 && target.z < targetTime;
		target = (target) * float3(textureScale,0) + float3(textureOffset,0);
		float3 force = target - thePosition;
	/*
		float distance = length(force);
		float goalLength = 2 * length(theVelocity);
		if(distance > goalLength)force *= goalLength / distance;
		*/	
		force *= 1;
		force -= theVelocity;
		return force * strength / (theDeltaTime * 60) * myUseTarget;
	}
};

struct Springs : Force{
	samplerRECT[] idTextures;
	samplerRECT[] infoTextures;
	
	float springConstant;
	float strength;
	
	float3 springForce(float3 thePosition1, float3 thePosition2, float theRestLength, float theForceRestLength){
		float3 delta = thePosition2 - thePosition1;
		float deltalength = length(delta);
		delta /= max(1,deltalength);
		float springForce = (deltalength - theRestLength) * springConstant * 0.1 * (deltalength > theRestLength || theForceRestLength > 0);
		return delta * springForce;
	}
	
	// constrain a particle to be a fixed distance from another particle
	float3 force(float3 thePosition, float3 theVelocity, float2 theTexID, float theDeltaTime){
		float3 force = 0;
		
		for(int i = 0; i < idTextures.length;i++){
			int4 ids = texRECT(idTextures[i], theTexID);
		
			// get positions of neighbouring particles
			float3 position1 = texRECT(positionTexture, ids.xy);
			float3 position2 = texRECT(positionTexture, ids.zw);
			
			float4 infos = texRECT(infoTextures[i], theTexID);
			float restLength1 = infos.x;
			float restLength2 = infos.y;
			float forceRestLength1 = infos.z;
			float forceRestLength2 = infos.w;
		
			force += springForce(thePosition, position1, restLength1, forceRestLength1) * (ids.x >= 0);
			force += springForce(thePosition, position2, restLength2, forceRestLength2) * (ids.z >= 0);
			
			//continue;
		}

		return force * strength;
	}
};

struct DampedSprings : Force{
	samplerRECT[] idTextures;
	samplerRECT[] infoTextures;
	
	float restLength;
	float springConstant;
	float springDamping;
	
	float strength;
	
	float3 springForce(float3 thePosition1, float3 thePosition2, float3 theVelocity1, float3 theVelocity2, float theRestLength, float theForceRestLength){
		float3 deltaPosition = thePosition1 - thePosition2;
        float3 deltaVelocity = theVelocity1 - theVelocity2;
		
		float myDistance = length(deltaPosition);
		
		deltaPosition /= max(1,myDistance);
		//deltaPosition *= myDistance > 0;

        float springForce = - (myDistance - theRestLength) * springConstant * (myDistance > theRestLength);
            
       	float dampingForce = -springDamping * dot(deltaPosition, deltaVelocity);
        return deltaPosition * (springForce + dampingForce);
	}
	
	// constrain a particle to be a fixed distance from another particle
	float3 force(float3 thePosition, float3 theVelocity, float2 theTexID, float theDeltaTime){
		
		float3 force = 0;
		
		//for(int i = 0; i < idTextures.length;i++){
			float4 ids = texRECT(idTextures[0], theTexID);
		
			// get positions of neighbouring particles
			float3 position1 = texRECT(positionTexture, ids.xy);
			float3 position2 = texRECT(positionTexture, ids.zw);
			
			// get velocities of neighbouring particles
			float3 velocity1 = texRECT(velocityTexture, ids.xy);
			float3 velocity2 = texRECT(velocityTexture, ids.zw);
			
			float4 infos = texRECT(infoTextures[0], theTexID);
			float restLength1 = infos.x;
			float restLength2 = infos.y;
			float forceRestLength1 = infos.z;
			float forceRestLength2 = infos.w;
		
			force = force + springForce(thePosition, position1, theVelocity, velocity1, restLength1, forceRestLength1) * (ids.x >= 0);
			force = force + springForce(thePosition, position2, theVelocity, velocity2, restLength2, forceRestLength2) * (ids.z >= 0);
		//}

		return force * strength;
	}
	
	
};

struct AnchoredSprings : Force{
	samplerRECT anchorPositionTexture;
	
	float restLength;
	float springConstant;
	float springDamping;
	
	float strength;
	
	float3 springForce(float3 thePosition1, float3 thePosition2, float3 theVelocity1, float3 theVelocity2){
		float3 deltaPosition = thePosition1 - thePosition2;
        float3 deltaVelocity = theVelocity1 - theVelocity2;
		
		float myDistance = length(deltaPosition);
		
		deltaPosition /= max(1,myDistance);

        float springForce = - (myDistance - restLength) * springConstant;
            
       	float dampingForce = -springDamping * dot(deltaPosition, deltaVelocity);
        return deltaPosition * (springForce + dampingForce);
	}
	
	float3 force(float3 thePosition, float3 theVelocity, float2 theTexID, float theDeltaTime){
		float4 anchor = texRECT(anchorPositionTexture, theTexID);
		
		if(anchor.w == 0)return float3(0,0,0);
		return springForce(thePosition, anchor.xyz, theVelocity, float3(0,0,0)) * anchor.w * strength;
	}
};

uniform Force forces[];
