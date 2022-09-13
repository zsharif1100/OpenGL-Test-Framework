#version 330 core

//in vec4 frag_color;
out vec4 color;

uniform vec4 u_Color;

void main() 
{
	color = u_Color;
}