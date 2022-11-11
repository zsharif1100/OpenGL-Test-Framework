#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Window.h"

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Shader.h"

#include "Camera/Camera.h"

#include "Primitives/Quad.h"
#include "Primitives/Cube.h"

//namespace SCONST {
//	static constexpr float MULTIPLIER = 1.0f;
//
//	static constexpr float SCREEN_WIDTH = 1920.0f * MULTIPLIER;
//	static constexpr float SCREEN_HEIGHT = 1080.0f * MULTIPLIER;
//	static constexpr float SCREEN_RATIO = SCREEN_WIDTH / SCREEN_HEIGHT;
//
//	static constexpr float RSCREEN_WIDTH = SCREEN_HEIGHT * (4.0f / 3.0f);
//	static constexpr float RSCREEN_HEIGHT = SCREEN_HEIGHT;
//	static constexpr float RSCREEN_RATIO = RSCREEN_WIDTH / RSCREEN_HEIGHT;
//
//	static constexpr float IMGUI_WIDTH = 400.0f; //SCREEN_WIDTH - RSCREEN_WIDTH;
//	static constexpr float IMGUI_HEIGHT = SCREEN_HEIGHT;
//	static constexpr float IMGUI_RATIO = IMGUI_WIDTH / IMGUI_HEIGHT;
//
//	static constexpr float RSCREEN_WIDTH_BEGIN = IMGUI_WIDTH;
//	static constexpr float RSCREEN_WIDTH_END = SCREEN_WIDTH;
//	static constexpr float RSCREEN_HEIGHT_BEGIN = 0;
//	static constexpr float RSCREEN_HEIGHT_END = SCREEN_HEIGHT;
//	//static constexpr glm::vec2 RSCREEN_MIDDLE = glm::vec2(RSCREEN_WIDTH_BEGIN + RSCREEN_WIDTH / 2, 
//	//													RSCREEN_HEIGHT_BEGIN + RSCREEN_HEIGHT / 2);
//}


enum class Material {
	Default = 0,
	LightSource = 1,
	Reflective = 2
};


class Renderer
{
private:
	static void BeginScene();
public:

	static void Init();
	static void InitData();
	static void ResetData();
	static void Shutdown();

	static void BeginScene(const Camera& camera);

	//static void DrawTriangle(const Triangle& tri);
	static void DrawQuad(const Quad& quad, Material material = Material::Default);
	static void DrawCube(const Cube& cube, Material material = Material::Reflective);
	static void DrawCubeLight(const Cube& cube);
	static void EndScene();


	static void Clear();
	static void Draw(const Shader& shader, const VertexArray& vao, const IndexBuffer& ibo);

};