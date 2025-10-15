#version 330

layout (location = 0) in vec3 a_Position;
uniform mat4 u_ModelMatrix;
uniform mat4 u_DirectionalLightTransformMatrix; 

void main()
{
	gl_Position = u_DirectionalLightTransformMatrix * u_ModelMatrix * vec4(a_Position, 1.0);
}
