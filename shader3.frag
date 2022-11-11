#version 120

varying vec3 color;
varying vec4 vPositionWorld;
varying vec4 vNormalWorld; 

struct lightStruct
{
	vec3 position;
	vec3 color;
};

#define NUM_LIGHTS 2

uniform lightStruct lights[NUM_LIGHTS];

uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float s;
uniform mat4 view;


void main()
{	
	vec3 L, R;
	vec3 N = vec3(vNormalWorld);
	vec3 E = {view[3][0], view[3][1], view[3][2]};
	float theta = degrees(acos(dot(N, E) / (length(N) * length(E))));

	// Check if angle between eye and normal is within threshold
	if (theta <= 105 && theta >= 75) {gl_FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);}
	else {gl_FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);}
}

// Uniform: ka, kd, ks, s, view
// Attributes: vPositionModel, vNormalModel
// L, N, R, E depend on attributes