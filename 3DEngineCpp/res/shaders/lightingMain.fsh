#include "sampling.glh"

float calcShadowAmount(sampler2D shadowMap, vec4 shadowMapCoordsInitial)
{
	//"not quite scaling but kind of scaling the vector coordinate" - Malena
	vec3 shadowMapCoords = (shadowMapCoordsInitial.xyz/shadowMapCoordsInitial.w)* vec3(0.5) + vec3(0.5);

	//.z is the distance from the light to the vertex being checked (.xy) due to the matrix being transformed. -1/1024 for bias to resolve achne.
	return SampleShadowMapPCF(shadowMap, shadowMapCoords.xy, shadowMapCoords.z - R_shadowBias, R_shadowTexelSize.xy);
}

void main()
{
	vec3 directionToEye = normalize(C_eyePos - worldPos0);
	vec2 texCoords = CalcParallaxTexCoords(dispMap, tbnMatrix, directionToEye, texCoord0, dispMapScale, dispMapBias);
	
	vec3 normal = normalize(tbnMatrix * (255.0/128.0 * texture2D(normalMap, texCoords).xyz - 1));
    
	vec4 lightingAmt = CalcLightingEffect(normal, worldPos0) * calcShadowAmount(R_shadowMap, shadowMapCoords0);

    gl_FragColor = texture2D(diffuse, texCoords) * lightingAmt;
}
