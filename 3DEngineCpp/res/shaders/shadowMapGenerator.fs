#version 120

void main()
{
	//should not be used in basic shadow map generation
	//PCF gl_FragColor = vec4(gl_FragCoord.z);
	gl_FragColor = vec4(gl_FragCoord.z, gl_FragCoord.z *gl_FragCoord.z, 0.0, 0.0);
}
