#version 120

varying vec4 vNormalWorld; 
uniform mat4 view;


void main()
{	
	// Find angle between normal and eye
	vec3 N = vec3(vNormalWorld);
	vec3 E = {view[3][0], view[3][1], view[3][2]};
	float theta = degrees(acos(dot(N, E) / (length(N) * length(E))));

	// Check if angle between normal and eye is within threshold
	if (theta <= 105 && theta >= 75) {gl_FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);}
	else {gl_FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);}
}