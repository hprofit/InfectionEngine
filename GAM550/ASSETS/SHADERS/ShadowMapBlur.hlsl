cbuffer BlurBuffer
{
    int BlurAmount;
};

[numthreads(N, 1, 1)]
void CShader( int3 groupThreadID : SV_GroupThreadID, int3 dispatchThreadID : SV_DispatchThreadID)
{
	// fill local thread storage to reduce bandwidth. Blur N N pixels, needs N + 2 * BlurRadius for Blur radius
	
	// thread group runs N threads. To get the extra 2*BlurRadius pixels, have 2*BlurRadius threads sample an extra pixel.
    if (groupThreadID.x < BlurAmount)
    {
		// clamp out of bound samples that occur at image borders
        int x = max(dispatchThreadID.x - BlurAmount, 0);
        gCache[groupThreadID.x] = gInput[int2(x, dispatchThreadID.y)];
    }
    if (groupThreadID.x >= N - BlurAmount)
    {
		// clamp out of bound samples that occur at image borders
        int x = min(dispatchThreadID.x + BlurAmount, gInput.Length.x - 1);
        gCache[groupThreadID.x + 2 * BlurAmount] = gInput[int2(x, dispatchThreadID.y)];
    }

	// clamp out of bound samples that occur at image borders
    gCache[groupThreadID.x + BlurAmount] = gInput[min(dispatchThreadID.xy, gInput.Length.xy - 1)];

	// wait for all threads to finish.
    GroupMemoryBarrierWithGroupSync();
	
	// Now blur each pixel.
    float4 blurColor = float4(0, 0, 0, 0);
	
	[unroll]
    for (int i = -BlurAmount; i <= BlurAmount; ++i)
    {
        int k = groupThreadID.x + BlurAmount + i;
		
        blurColor += gWeights[i + BlurAmount] * gCache[k];
    }
	
    gOutput[dispatchThreadID.xy] = blurColor;
}