// Copyright (c) 2004-2005 Lutz Latta
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.


// Supporting functions for adapting generic sorting algorithm to distance sorting.
//
// Note: Packing of two 16-bit halfs into one float is used.
// This works only perfect with texture sizes up to 1024.
// With size 2048 the higher coordinates shift from xxxx.5 to integer values,
// then the algorithm should be changed to store only integers and always add 0.5.

float2 main(float2 _TexCoord : TEXCOORD0,
	uniform samplerRECT _SortTexture : register(s0),
	uniform samplerRECT _PositionTexture : register(s1),
	uniform float3 _ViewerPosition
) : COLOR
{
	float sortIndex = texRECT(_SortTexture, _TexCoord.xy).y;
	half2 particleIndex = unpack_2half(sortIndex);

	float3 particlePos = (float3)texRECT(_PositionTexture, particleIndex);

	float3 delta = _ViewerPosition - particlePos;
	float distanceSqr = dot(delta, delta);

	// Prevent unused, far-away particles from destroying comparisons in sorting.
	if (distanceSqr > 1e6 || isnan(distanceSqr))
		distanceSqr = 1e6;

	return float2(distanceSqr, sortIndex);
}