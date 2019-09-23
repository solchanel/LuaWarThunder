#pragma once
#include "math.hpp"

class Renderer
{
public:
	void Initialize();

	void BeginScene();
	void DrawScene();
	void EndScene();

	float DrawStringEx(ImFont* pFont, const std::string& text, const ImVec2& position, float size, uint32_t color, bool center = false);
	float DrawStringEx(ImFont* pFont, const ImVec2& pos, float size, uint32_t color, const char *msg, ...);
	float DrawString(const ImVec2& pos, float size, uint32_t color, const char *msg, ...);

	void DrawLine(const ImVec2& from, const ImVec2& to, uint32_t color, float thickness = 1.0f);
	void DrawCircle(const ImVec2& position, float radius, uint32_t color, float thickness = 1.0f);
	void DrawCircleFilled(const ImVec2& position, float radius, uint32_t color);

	void DrawBulletTrace(Angle weapon_angle, Vector pos, uint32_t color);

	void DrawBox(const ImVec2& pos, float w, float h, uint32_t color);
	void Draw3DBox(Vector* boxVectors, uint32_t color);
	void DrawTank(const Vector Matrix3x3[3], const Vector pos, const Vector bbmin, const Vector bbmax, uint32_t color);
	void DrawTank3DBox(const Vector Matrix3x3[3], const Vector pos, const Vector bbmin, const Vector bbmax, uint32_t color);

	static Renderer *GetInstance();

private:
	Renderer();
	~Renderer();

	ImFont *m_pFont;
	static Renderer *m_pInstance;
};


namespace LuaRenderer
{
	float DrawString(sol::table pos, const float size, const uint32_t color, const std::string msg);
	void DrawLine(sol::table from, sol::table to, uint32_t color, float thickness);
	void DrawCircle(sol::table pos, float radius, uint32_t color, float thickness);
	void DrawCircleFilled(sol::table pos, float radius, uint32_t color);
};