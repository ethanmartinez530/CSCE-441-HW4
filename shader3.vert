#version 120
// Silhouette shader: only light up fragments with close to a 90 deg. angle b/w normal and eye
// Theta = arccos( (n dot e) / (|n| x |e|) )

attribute vec3 vPositionModel; // in object space
attribute vec3 vNormalModel; // in object space

varying vec4 vPositionWorld; // in world space
varying vec4 vNormalWorld; // in world space

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
	gl_Position = projection * view * model * vec4(vPositionModel, 1.0);
	vPositionWorld = model * vec4(vPositionModel, 1.0);
	vPositionWorld /= vPositionWorld.w;
	vNormalWorld = model * vec4(vNormalModel, 1.0);	// replace model with modelInvTrans once I get it working
	vNormalWorld /= vNormalWorld.w;
}
