#version 120

varying vec4 vPositionWorld;
varying vec4 vNormalWorld; 

struct lightStruct
{
	vec3 position;
	vec3 color;
	bool spotlight;
	vec3 A;
	float theta;
	float alpha;
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
	vec3 E = normalize(vec3(vPositionWorld) - vec3(view[3][0], view[3][1], view[3][2]));
	vec3 I = ka;
	
	for (int i = 0; i < NUM_LIGHTS; i++) {
		L = normalize(lights[i].position - vec3(vPositionWorld));
		R = 2 * N * dot(L, N) - L;
		float spotCoef = 1;
		if (lights[i].spotlight) {
			if (dot(-L, lights[i].A) < cos(degrees(lights[i].theta))) {spotCoef = 0;}
			else {spotCoef = pow(dot(-L, lights[i].A), lights[i].alpha);}
		}
		I += lights[i].color * spotCoef * (kd * max(0, dot(L, N)) + ks * pow(max(0, dot(R, E)), s));
	}
	
	gl_FragColor = vec4(I, 1.0f);
}