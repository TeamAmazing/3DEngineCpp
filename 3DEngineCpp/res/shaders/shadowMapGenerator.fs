#version 120

void main()
{
	float depth = gl_FragCoord.z;

	float dx = dFdx(depth);
	float dy = dFdy(depth);
	float moment2 = depth * depth + 0.25 * (dx * dx + dy * dy);

	//should not be used in basic shadow map generation
	//PCF gl_FragColor = vec4(gl_FragCoord.z);
	gl_FragColor = vec4(1.0 - depth,1.0 - moment2, 0.0, 0.0);
}
