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

void main()
{
	// Convert to world coords
	vPositionWorld = model * vec4(vPositionModel, 1.0);
	vNormalWorld = model * vec4(vNormalModel, 1.0);	// replace model with modelInvTrans once I get it working

	gl_Position = projection * view * model * vec4(vPositionModel, 1.0);
}
