#version 120

attribute vec3 vPositionModel; // in object space
attribute vec3 vNormalModel; // in object space

vec4 vPositionWorld; // in world space
vec4 vNormalWorld; // in world space

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 modelInvTrans;

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
	vPositionWorld = model * vec4(vPositionModel, 1.0);
	vPositionWorld /= vPositionWorld.w;
	vNormalWorld = model * vec4(vNormalModel, 1.0);	// replace model with modelInvTrans once I get it working
	vNormalWorld /= vNormalWorld.w;
	gl_Position = projection * view * model * vec4(vPositionModel, 1.0);
	
	vec3 L, R;
	vec3 N = vec3(vNormalWorld);
	vec3 E = normalize(vec3(vPositionWorld) - vec3(view[3][0], view[3][1], view[3][2]));
	color = ka;
	
	for (int i = 0; i < NUM_LIGHTS; i++) {
		L = normalize(lights[i].position - vec3(vPositionWorld));
		R = 2 * N * dot(L, N) - L;
		color += lights[i].color * (kd * max(0, dot(L, N)) + ks * pow(max(0, dot(R, E)), s));
	}
}
