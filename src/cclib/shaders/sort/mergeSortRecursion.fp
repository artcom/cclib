float2 main(
	float2 _Current : TEXCOORD0,
	uniform samplerRECT _SortData : register(s0),
	uniform int2 _Size,
	uniform int _Step,
	uniform int _Count
) : COLOR
{
	float2 currentSample = (float2)texRECT(_SortData, (float2)_Current);

	float i = (_Current.y - 0.5) * _Size.x + _Current.x;

	float iMod = fmod((float)(i / _Step), (float)_Count);

	if (iMod >= 1.0 && iMod < _Count - 1.0){
		float b = (fmod((float)iMod, 2.0) > 1.0 ? 1.0 : -1.0);

		float otherI = i + (b * _Step);
		float2 otherPos = float2(fmod(otherI, _Size.x), floor(otherI / _Size.x) + 0.5);

		float2 otherSample = (float2)texRECT(_SortData, otherPos);

		if (b >= 0){
			//return max(currentSample, otherSample);
			return currentSample.x > otherSample.x ? currentSample : otherSample;
		} else {
			//return min(currentSample, otherSample);
			return currentSample.x < otherSample.x ? currentSample : otherSample;
		}

		//return b * max(b * currentSample, b * otherSample);
	} else
		return currentSample;
}