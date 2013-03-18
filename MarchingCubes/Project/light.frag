#version 150 core

in vec4 pass_Color;
in vec3 pass_Normal;

out vec4 out_Color;

void main(void)
{
	out_Color = pass_Color;
}
