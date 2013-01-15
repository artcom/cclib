uniform float deltaTime;

void main (
	in 	float2 texID : WPOS,
	out float4 newPosition : COLOR0,
	out float4 newInfo : COLOR1,
	out float3 newVelocity : COLOR2
){
	float3 position =(float3) texRECT (positionTexture, texID);
	float3 velocity = texRECT (velocityTexture, texID);
	float3 acceleration = float3(0,0,0);
	
	for(int i = 0; i < forces.length;i++){
		acceleration = acceleration + forces[i].force(position,velocity,texID,deltaTime);
	}
	
	velocity = velocity + acceleration * (deltaTime * 60);
	
	for(int i = 0; i < constraints.length;i++){
		velocity = constraints[i].constraint(velocity, position,texID, deltaTime);
	}
	
	for(int i = 0; i < impulses.length;i++){
		velocity += impulses[i].impulse(position,velocity, texID, deltaTime);
	}
	newVelocity = velocity;
	
	float4 lastInfo = texRECT(infoTexture, texID);
	float myAge = lastInfo.x;
	int myStep = (int)lastInfo.w;
	
	newInfo = float4(
		lastInfo.x + deltaTime,
		lastInfo.y,
		lastInfo.z,
		lastInfo.w
	);
	
	if(myAge >= lastInfo.y && lastInfo.z == 0.0)position = float3(1000000,0,0);
	newPosition = float4(position + deltaTime * velocity,1); 
}
	           