#version 150 core

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 normalMatrix;

in vec3 in_Position;
in vec4 in_Color;
in vec3 in_Normal;

out vec4 pass_Color;
out vec3 pass_Normal;

void main(void)
{
	gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);	
	pass_Normal = vec3(0,0,1);
	//pass_Normal = in_Normal;
	
	pass_Color = vec4(in_Normal, 1.0);
	//pass_Color = in_Color;
}
