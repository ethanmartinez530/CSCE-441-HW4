#version 120

attribute vec3 vPositionModel; // in object space
attribute vec3 vNormalModel; // in object space

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

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

varying vec3 color;

void main()
{
	vec3 L[NUM_LIGHTS], R[NUM_LIGHTS], Lp[NUM_LIGHTS], Lo[NUM_LIGHTS];
	vec3 N, E, I;
	gl_Position = projection * view * model * vec4(vPositionModel, 1.0);
	
	N = vec3(model * vec4(vNormalModel, 1.0));
	E = normalize(vec3(view[3][0], view[3][1], view[3][2]) - vec3(model * vec4(vPositionModel, 1.0)));
	for (int i = 0; i < NUM_LIGHTS; i++) {
		L[i] = normalize(lights[i].position - vec3(model * vec4(vPositionModel, 1.0)));
		Lp[i] = N * dot(L[i], N);
		Lo[i] = L[i] - Lp[i];
		R[i] = Lp[i] - Lo[i];
	}

	I = ka;
	for (int i = 0; i < NUM_LIGHTS; i++) {
		I += lights[i].color * (kd * max(0, dot(L[i], N)) + ks * pow(max(0, dot(R[i], E)), s));
	}
	color = I;
	//color = vec3(1.0f, 0.0f, 0.0f);
}
