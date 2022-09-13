//#vertexShader
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
//layout(location = 2) in vec4 color;

//out vec4 v_Color;
out vec2 v_TexCoord;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * position;

	//v_Color = color;
	v_TexCoord = texCoord;
}

//#fragmentShader
#version 330 core

in vec2 v_TexCoord;
//in vec4 v_Color;
out vec4 color;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
	//color = vec4(1.0, 1.0, 0.0, 1.0);
	//color = u_Color;
	//color = v_Color;
	vec4 tex_Color = texture(u_Texture, v_TexCoord);
	color = tex_Color;
}
