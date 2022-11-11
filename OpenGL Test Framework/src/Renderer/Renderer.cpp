#include "Renderer.h"

#include <GL/glew.h>
#include "GLErrorManager.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <array>
#include <vector>


static const int c_MaxVerts = 1024;
static const int c_MaxTexSlots = 8;
static const int c_MaterialCount = 3;

struct RendererData {
	float data[c_MaxVerts] = { 0 };
	float* vertPtr = &data[0];


	std::array<Shader, c_MaterialCount> shaders;

	VertexBuffer vbo;
	VertexBufferLayout layout;
	VertexArray vao;

	std::array<std::vector<unsigned int>, c_MaterialCount> iboVectors;

	std::vector<unsigned int> iboVecDefault;
	std::vector<unsigned int> iboVecLighting;

	glm::vec3 cameraPos;
	glm::mat4 cameraVP;

	uint32_t vertCount = 0;
	uint32_t quadCount = 0;
	std::array<uint32_t, c_MaterialCount> quadCounts = { 0 };

	uint32_t texIndex = 0;

	Texture defaultTex;
	std::array<int, c_MaxTexSlots> texSlots = { 0 };

	std::vector<std::pair<glm::vec3, glm::vec3>> lightSources;

	RendererData()
		:shaders{Shader("../../../OpenGL Test Framework/res/shaders/default.shader"),
				Shader("../../../OpenGL Test Framework/res/shaders/lightSource.shader"),
				Shader("../../../OpenGL Test Framework/res/shaders/lighting.shader")},

		vbo(data, c_MaxVerts * sizeof(float), GL_DYNAMIC_DRAW),
		defaultTex()
	{
		for (int i = 0; i < c_MaterialCount; i++) {
			iboVectors[i].reserve(c_MaxVerts * 2);
		}

		lightSources.reserve(8);

		layout.Push<float>(3); //position
		layout.Push<float>(4); //color
		layout.Push<float>(2); //Texture coord
		layout.Push<float>(1); //Tex slot

		vao.AddBuffer(vbo, layout);
	}

	float NextTexSlot(const Texture& tex) {
		const int texID = tex.GetID();
		float texSlot = 0.0f;

		if (texID != defaultTex.GetID())
		{
			auto found = std::find(texSlots.begin(), texSlots.end(), texID);

			if (found != texSlots.end()) {
				//tex id found
				texSlot = (float)std::distance(texSlots.begin(), found);
			}
			else {
				//tex id not found
				texSlots[texIndex] = texID;
				tex.Bind(texIndex);
				texSlot = (float)texIndex;
				texIndex++;
			}
		}
		return texSlot;
	}

	void AddQuadVBO(const glm::vec4& posVert, const glm::vec4& colVert, 
		const glm::vec2& texVert, const float& texSlot)
	{

		for (int i = 0; i < 3; i++) {
			*vertPtr = posVert[i];
			vertPtr++;
		}

		for (int i = 0; i < 4; i++) {
			*vertPtr = colVert[i];
			vertPtr++;
		}

		for (int i = 0; i < 2; i++) {
			*vertPtr = texVert[i];
			vertPtr++;
		}
		*vertPtr = texSlot;
		vertPtr++;

		vertCount++;
	}

	void AddQuadIBO(Material material) {
		unsigned int firstVert = vertCount - 4;
		std::vector<unsigned int>&  iboVec = iboVectors[(int)material];

		iboVec.push_back(firstVert + 0);
		iboVec.push_back(firstVert + 1);
		iboVec.push_back(firstVert + 2);

		iboVec.push_back(firstVert + 2);
		iboVec.push_back(firstVert + 3);
		iboVec.push_back(firstVert + 0);

		quadCounts[(int)material]++;
	}

	void AddLight(glm::vec3 pos, glm::vec3 color) {
		lightSources.push_back(std::make_pair(pos, color));
		glm::vec3& lightPos = lightSources[0].first;
		glm::vec3& lightColor = lightSources[0].second;

		shaders[(int)Material::Reflective].SetUniform3f("u_LightPos", lightPos.x, lightPos.y, lightPos.z);
		shaders[(int)Material::Reflective].SetUniform3f("u_LightColor", lightColor.x, lightColor.y, lightColor.z);
	}

	void UpdateCamera(const Camera& camera) {
		cameraVP = camera.GetViewProj();
		cameraPos = camera.GetPos();

		for (int i = 0; i < c_MaterialCount; i++) {
			shaders[i].SetUniformMat4f("u_MVP", cameraVP);
		}

		shaders[(int)Material::Reflective].SetUniform3f("u_CameraPos", 
			cameraPos.x, cameraPos.y, cameraPos.z);
	}

	void ClearData() {
		vertPtr = &data[0];
		quadCount = 0;
		vertCount = 0;
		quadCounts = { 0 };
		lightSources.clear();
		texIndex = 0;

		texSlots = { 0 };
		defaultTex.Bind(0);

		texSlots[texIndex] = defaultTex.GetID();
		texIndex++;

		for (int i = 0; i < c_MaterialCount; i++) {
			iboVectors[i].clear();
		}
	}
};

static std::unique_ptr<RendererData> s_Data;

void error_callback(int error, const char* description) {
	fprintf(stderr, "error: %s\n", description);
}

void Renderer::Init()
{
	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) {
		std::cout << "GLFW Initialization failed!" << std::endl;
	}

}

void Renderer::InitData() {
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); //src RGBA is set to src alpha and dest RGBA is set to 1 - src alpha.
	GLCall(glEnable(GL_DEPTH_TEST));

	s_Data = std::make_unique<RendererData>();

	static int samplers[c_MaxTexSlots] = { 0 };

	for (int i = 0; i < c_MaxTexSlots; i++)
		samplers[i] = i;

	for (int i = 0; i < c_MaterialCount; i++) {
		s_Data->shaders[i].SetUniform1iv("u_Texture", c_MaxTexSlots, samplers);
	}
}

void Renderer::ResetData()
{
	s_Data.reset();
}

void Renderer::Shutdown()
{
	glfwTerminate();
}

void Renderer::BeginScene(const Camera& camera)
{
	s_Data->ClearData();
	s_Data->UpdateCamera(camera);
}

void Renderer::BeginScene()
{
	s_Data->ClearData();
}

void Renderer::EndScene()
{
	unsigned int replaceSize = s_Data->quadCount * 4 * s_Data->layout.GetStride();
	s_Data->vbo.updateBuffer(s_Data->data, replaceSize, 0);

	for (int i = 0; i < c_MaterialCount; i++) {
		IndexBuffer ibo(s_Data->iboVectors[i].data(), s_Data->quadCounts[i] * 6);

		Renderer::Draw(s_Data->shaders[i], s_Data->vao, ibo);
	}
}

void Renderer::DrawQuad(const Quad& quad, Material material)
{
	unsigned int quadVertCount = s_Data->layout.GetCount() * 4;
	unsigned int vertCount = s_Data->quadCount * quadVertCount;

	if (vertCount > c_MaxVerts - quadVertCount
		|| s_Data->texIndex > c_MaxTexSlots) {
		Renderer::EndScene();
		Renderer::BeginScene();
	}

	float texSlot = s_Data->NextTexSlot(quad.GetTexture());

	const glm::vec4& colVert = quad.GetColor();
	for (int vertex = 0; vertex < 4; vertex++) {
		const glm::vec4& posVert = quad.GetPosVert(vertex);
		const glm::vec2& texVert = quad.GetTexVert(vertex);

		s_Data->AddQuadVBO(posVert, colVert,texVert, texSlot);
	}

	s_Data->AddQuadIBO(material);
	s_Data->quadCount++;
}

void Renderer::DrawCube(const Cube& cube, Material material)
{
	unsigned int vertCount = s_Data->quadCount * s_Data->layout.GetCount() * 8;

	if (vertCount > c_MaxVerts - s_Data->layout.GetCount() * 8
		|| s_Data->texIndex > c_MaxTexSlots) {
		Renderer::EndScene();
		Renderer::BeginScene();
	}

	float texSlot = s_Data->NextTexSlot(cube.GetTexture());

	const glm::vec4& colVert = cube.GetColor();
	
	for (int face = 0; face < 6; face++) {

		unsigned int currentVert = s_Data->vertCount;

		const int* vertexOrder = cube.GetOrder(face);

		for (int vertex = 0; vertex < 4; vertex++) {
			const glm::vec4& posVert = cube.GetPosVert(vertexOrder[vertex]);
			const glm::vec2& texVert = cube.GetTexVert(vertex);

			s_Data->AddQuadVBO(posVert, colVert, texVert, texSlot);
		}

		s_Data->quadCount++;

		s_Data->AddQuadIBO(material);
		s_Data->quadCount++;
	}
}

void Renderer::DrawCubeLight(const Cube& cube)
{
	s_Data->AddLight(cube.GetPos(), cube.GetColor());
	DrawCube(cube, Material::LightSource);
}

void Renderer::Clear()
{
	//GLCall(glClearColor(0.53f, 0.81f, 0.92f, 1.0f)); //light blue
	GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::Draw(const Shader& shader, const VertexArray& vao, const IndexBuffer& ibo)
{
	shader.Bind();
	vao.Bind();
	ibo.Bind();

	GLCall(glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr))
}