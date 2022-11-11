//#vertexShader
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in float texIndex;

out vec4 v_ColorPass;
out vec2 v_TexCoordPass;
out float v_TexIndexPass;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * position;

	v_ColorPass = color;
	v_TexCoordPass = texCoord;
	v_TexIndexPass = (texIndex);
}

//#geometryShader
#version 330 core

in vec4 v_ColorPass[];
in vec2 v_TexCoordPass[];
in float v_TexIndexPass[];

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;


void main()
{
	for (int i = 0; i < 3; i++) {
		gl_Position = gl_in[i].gl_Position;
		v_Color = v_ColorPass[i];
		v_TexCoord = v_TexCoordPass[i];
		v_TexIndex = v_TexIndexPass[i];
		EmitVertex();
	}
	EndPrimitive();
}

//#fragmentShader
#version 330 core

in vec2 v_TexCoord;
in vec4 v_Color;
in float v_TexIndex;

out vec4 color;

uniform sampler2D u_Texture[8];

void main()
{
	vec4 tex_Color = texture2D(u_Texture[int(v_TexIndex)], v_TexCoord);
	color = v_Color * tex_Color;
}
