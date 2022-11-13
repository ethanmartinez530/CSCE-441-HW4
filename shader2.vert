#version 120
// Phong shading - color calculation is done in frag file

attribute vec3 vPositionModel; // in object space
attribute vec3 vNormalModel; // in object space

// Varying so they can be sent to shader2.frag
varying vec4 vPositionWorld; // in world space
varying vec4 vNormalWorld; // in world space

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 modelInvTrans;

void main()
{
	// Convert to world coords
	vPositionWorld = model * vec4(vPositionModel, 1.0);
	vNormalWorld = model * vec4(vNormalModel, 1.0);	// replace model with modelInvTrans once I get it working

	gl_Position = projection * view * model * vec4(vPositionModel, 1.0);
}
