//#vertexShader
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in float texIndex;

out vec3 v_PosPass;
out vec4 v_ColorPass;
out vec2 v_TexCoordPass;
out float v_TexIndexPass;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * position;

	v_PosPass = position.xyz;

	v_ColorPass = color;
	v_TexCoordPass = texCoord;
	v_TexIndexPass = (texIndex);
}

//#geometryShader
#version 330 core

in vec3 v_PosPass[];
in vec4 v_ColorPass[];
in vec2 v_TexCoordPass[];
in float v_TexIndexPass[];

out vec3 v_FragPos;
out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;
out vec3 g_Normal;

layout (triangles) in;
layout(triangle_strip, max_vertices = 3) out;

void main()
{
	vec3 p0 = v_PosPass[0];
	vec3 p1 = v_PosPass[1];
	vec3 p2 = v_PosPass[2];

	g_Normal = normalize(cross(p1 - p0, p2 - p0));

	for (int i = 0; i < 3; i++) {
		gl_Position = gl_in[i].gl_Position;

		v_FragPos = v_PosPass[i];
		v_Color = v_ColorPass[i];
		v_TexCoord = v_TexCoordPass[i];
		v_TexIndex = v_TexIndexPass[i];
		EmitVertex();
	}

	EndPrimitive();
}

//#fragmentShader
#version 330 core

in vec3 v_FragPos;
in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;

in vec3 g_Normal;

out vec4 color;

uniform sampler2D u_Texture[8];

uniform vec3 u_CameraPos;
uniform vec3 u_LightPos;
uniform vec3 u_LightColor;

void main()
{
	float ambientStrength = 0.1f;
	vec3 ambient = vec3(0.05f, 0.05f, 0.05f) + ambientStrength * u_LightColor;

	vec3 lightDir = normalize(u_LightPos - v_FragPos);

	float diff = max(dot(g_Normal, lightDir), 0.0f);
	vec3 diffuse = diff * u_LightColor;

	float specularStrength = 0.5f;
	vec3 viewDir = normalize(u_CameraPos - v_FragPos);
	vec3 reflectDir = reflect(-lightDir, g_Normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 16);
	vec3 specular = specularStrength * spec * u_LightColor;

	vec3 light = ambient + diffuse + specular;

	vec4 tex_Color = texture2D(u_Texture[int(v_TexIndex)], v_TexCoord);
	color = v_Color * vec4(light, 1.0f)* tex_Color;
}