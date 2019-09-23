#include "../main.hpp"
#include "ImFont.hpp"

#include <sstream>
#include <mutex>

Renderer *Renderer::m_pInstance;

void LuaRenderer::DrawCircleFilled(sol::table pos, float radius, uint32_t color)
{
	Renderer::GetInstance()->DrawCircleFilled({ pos.get<float>(1), pos.get<float>(2) }, radius, color);
}

void LuaRenderer::DrawLine(sol::table from, sol::table to, uint32_t color, float thickness = 1.0f)
{
	Renderer::GetInstance()->DrawLine({ from.get<float>(1), from.get<float>(2) }, { to.get<float>(1), to.get<float>(2) }, color, thickness);
}

void LuaRenderer::DrawCircle(sol::table pos, float radius, uint32_t color, float thickness = 1.0f)
{
	Renderer::GetInstance()->DrawCircle({ pos.get<float>(1), pos.get<float>(2) }, radius, color, thickness);
}

float LuaRenderer::DrawString(sol::table pos, float size, uint32_t color, std::string msg)
{
	return Renderer::GetInstance()->DrawString({ pos.get<float>(1), pos.get<float>(2) }, size, color, msg.c_str());
}

Renderer::Renderer()
{

}

Renderer::~Renderer()
{

}

void Renderer::Initialize()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	std::cout << "Renderer Initialization" << std::endl;
	// This is my example of font initializing
	// First font used for UI
	// Second for rendering, 32.0f is pixel size, not font size.
	//io.Fonts->AddFontFromMemoryTTF(g_fRubik, sizeof(g_fRubik), 16.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());
	//m_pFont = io.Fonts->AddFontFromMemoryTTF(g_fRubik, sizeof(g_fRubik), 32.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());
	//m_pFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdana.ttf", 16.0f);

	//io.Fonts->AddFontFromMemoryTTF(g_fRubik, sizeof(g_fRubik), 16.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());
	//m_pFont = io.Fonts->AddFontFromMemoryTTF(g_fRubik, sizeof(g_fRubik), 32.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());
	m_pFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\verdana.ttf", 16.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	//m_pFont = GImGui->Font;
}

void Renderer::BeginScene()
{
	//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	//ImGui::Begin("BackBuffer", reinterpret_cast<bool*>(true), ImVec2(0, 0), 0.0f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs);

	//ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
	ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.0f, 0.0f, 0.0f, 0.0f });
	ImGui::Begin("D3D11_Buffer", reinterpret_cast<bool*>(true), ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs);

	ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y), ImGuiCond_Always);
}

std::once_flag flag;
void Renderer::DrawScene()
{
	ImGuiIO& io = ImGui::GetIO();

#define TOP_OFFSET 80
	DrawStringEx(m_pFont, ("Frametime: " + std::to_string((int)io.Framerate)), ImVec2(10, TOP_OFFSET), 15.0f, 0xFFFFFFFF);

	LuaAPI::LuaCallback<void()>("WarThunder_DrawScene");

	if (O::Game->IsInFlight)
	{
		if (Vars.Visuals.Showbots)
		{
			unsigned int max_players = (unsigned int)O::Game->Bots.MaxPlayers;

			for (unsigned i = 0; i < max_players; i++)
			{
				auto unit = O::Game->Bots.Entities->EntList[i];

				if (!unit || unit->GetPosition().IsZero())
					continue;

				if (unit == G::LocalPlayer->GetUnit())
					continue;

				Vector s_pos;
				if (M::DirectXWorldToScreen(unit->GetPosition(), s_pos))
				{
					auto  target_vec = G::LocalPlayer->GetUnit()->GetPosition() - unit->GetPosition();
					float dist = target_vec.Length();
					float box_height = (100.f * 30.f) / dist;

					DrawTank3DBox(unit->RotationMatrix, unit->GetPosition(), unit->bbmax, unit->bbmin, ImGui::GetColorU32(ImVec4(0.f, 204.f, 51.f, 1.f)));

					if (Vars.Visuals.ShortName || Vars.Visuals.FullName)
					{
						if (Vars.Visuals.DrawDistance)
						{
							if (Vars.Visuals.ShortName)
								DrawString({ s_pos.x, s_pos.y - (box_height / 2) }, 15.0f, 0xFFFFFFFF, "%s (%d)", unit->GetUnitInfo()->GetShortName(), (int)dist);
							if (Vars.Visuals.FullName)
								DrawString({ s_pos.x, s_pos.y - (box_height / 2) }, 15.0f, 0xFFFFFFFF, "%s (%d)", unit->GetUnitInfo()->GetName(), (int)dist);
						}
						else
						{
							if (Vars.Visuals.ShortName)
								DrawString({ s_pos.x, s_pos.y - (box_height / 2) }, 15.0f, 0xFFFFFFFF, "%s", unit->GetUnitInfo()->GetShortName());
							if (Vars.Visuals.FullName)
								DrawString({ s_pos.x, s_pos.y - (box_height / 2) }, 15.0f, 0xFFFFFFFF, "%s", unit->GetUnitInfo()->GetName());
						}
					}
					else
					{
						if (Vars.Visuals.DrawDistance)
							DrawString({ s_pos.x, s_pos.y - (box_height / 2) }, 15.0f, 0xFFFFFFFF, "%d", (int)dist);
					}
				}
			}
		}

		unsigned int max_players = (unsigned int)O::List->MaxPlayers;

		for (unsigned i = 0; i < max_players; i++)
		{
			auto ent = O::List->Entities->EntList[i];

			if (!ent || !G::LocalPlayer || G::LocalPlayer == ent || !ent->IsAlive() || ent->GetUnit()->GetPosition().IsZero())
				continue;

			if (!Vars.Visuals.ShowFriendly && ent->IsFriendly())
				continue;

			if (!Vars.Visuals.ShowEnemies && ent->IsEnemy())
				continue;

			Vector s_pos;
			if (M::DirectXWorldToScreen(ent->GetUnit()->GetPosition(), s_pos))
			{
				auto  target_vec = G::LocalPlayer->GetUnit()->GetPosition() - ent->GetUnit()->GetPosition();
				float dist = target_vec.Length();
				float box_height = (100.f * 30.f) / dist;

				//Angle Angles;
				//M::MatrixAngles(ent->GetUnit()->RotationMatrix, Angles);

				//DrawString({ s_pos.x, s_pos.y - (box_height / 2) }, 15.0f, 0xFFFFFFFF, "Angles: %f %f %f", Angles[0], Angles[1], Angles[2]);

				if (ent->IsEnemy())
					DrawTank3DBox(ent->GetUnit()->RotationMatrix, ent->GetUnit()->GetPosition(), ent->GetUnit()->bbmax, ent->GetUnit()->bbmin, ImGui::GetColorU32(ImVec4(0.f, 204.f, 51.f, 1.f)));
				if (ent->IsBot())
					DrawTank3DBox(ent->GetUnit()->RotationMatrix, ent->GetUnit()->GetPosition(), ent->GetUnit()->bbmax, ent->GetUnit()->bbmin, ImGui::GetColorU32(ImVec4(255.f, 153.f, 0.f, 1.f)));
				if (ent->IsFriendly())
					DrawTank3DBox(ent->GetUnit()->RotationMatrix, ent->GetUnit()->GetPosition(), ent->GetUnit()->bbmax, ent->GetUnit()->bbmin, ImGui::GetColorU32(ImVec4(100.f, 204.f, 51.f, 1.f)));

				if (Vars.Visuals.WeaponTrace && ent->IsEnemy())
					DrawBulletTrace(ent->GetUnit()->GetUnitWeapons()->GetWeaponAngle(0), ent->GetUnit()->GetPosition(), ImGui::GetColorU32(ImVec4(100.f, 204.f, 51.f, 1.f)));

				if (Vars.Visuals.ShortName || Vars.Visuals.FullName)
				{
					if (Vars.Visuals.DrawDistance)
					{
						if (Vars.Visuals.ShortName)
							DrawString({ s_pos.x, s_pos.y - (box_height / 2) }, 15.0f, 0xFFFFFFFF, "%s (%d)", ent->GetUnit()->GetUnitInfo()->GetShortName(), (int)dist);
						if (Vars.Visuals.FullName)
							DrawString({ s_pos.x, s_pos.y - (box_height / 2) }, 15.0f, 0xFFFFFFFF, "%s (%d)", ent->GetUnit()->GetUnitInfo()->GetName(), (int)dist);
					}
					else
					{
						if (Vars.Visuals.ShortName)
							DrawString({ s_pos.x, s_pos.y - (box_height / 2) }, 15.0f, 0xFFFFFFFF, "%s", ent->GetUnit()->GetUnitInfo()->GetShortName());
						if (Vars.Visuals.FullName)
							DrawString({ s_pos.x, s_pos.y - (box_height / 2) }, 15.0f, 0xFFFFFFFF, "%s", ent->GetUnit()->GetUnitInfo()->GetName());
					}
				}
				else
				{
					if (Vars.Visuals.DrawDistance)
						DrawString({ s_pos.x, s_pos.y - (box_height / 2) }, 15.0f, 0xFFFFFFFF, "%d", (int)dist);
				}
			}
		}
	}
}

void Renderer::EndScene()
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	window->DrawList->PushClipRectFullScreen();

	ImGui::End();
	ImGui::PopStyleColor();
}


float Renderer::DrawString(const ImVec2& pos, float size, uint32_t color, const char *msg, ...)
{
	va_list va_alist;
	char buf[1024];
	va_start(va_alist, msg);
	size_t n = std::vsnprintf(buf, sizeof(buf), msg, va_alist);
	va_end(va_alist);
	return DrawStringEx(this->m_pFont, { buf, n }, pos, size, color, false);
}

float Renderer::DrawStringEx(ImFont* pFont, const ImVec2& pos, float size, uint32_t color, const char *msg, ...)
{
	va_list va_alist;
	char buf[1024];
	va_start(va_alist, msg);
	std::vsnprintf(buf, sizeof(buf), msg, va_alist);
	va_end(va_alist);
	return DrawStringEx(pFont, std::string(buf, sizeof(buf)), pos, size, color, false);
}

float Renderer::DrawStringEx(ImFont* pFont, const std::string& text, const ImVec2& pos, float size, uint32_t color, bool center)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	float a = (color >> 24) & 0xff;
	float r = (color >> 16) & 0xff;
	float g = (color >> 8) & 0xff;
	float b = (color) & 0xff;

	std::stringstream steam(text);
	std::string line;

	float y = 0.0f;
	int i = 0;

	while (std::getline(steam, line))
	{
		ImVec2 textSize = pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());

		if (center)
		{
			window->DrawList->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) + 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, a / 255)), line.c_str());
			window->DrawList->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) - 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, a / 255)), line.c_str());
			window->DrawList->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) + 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, a / 255)), line.c_str());
			window->DrawList->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) - 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, a / 255)), line.c_str());

			window->DrawList->AddText(pFont, size, ImVec2(pos.x - textSize.x / 2.0f, pos.y + textSize.y * i), ImGui::GetColorU32(ImVec4(r / 255, g / 255, b / 255, a / 255)), line.c_str());
		}
		else
		{
			window->DrawList->AddText(pFont, size, ImVec2((pos.x) + 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, a / 255)), line.c_str());
			window->DrawList->AddText(pFont, size, ImVec2((pos.x) - 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, a / 255)), line.c_str());
			window->DrawList->AddText(pFont, size, ImVec2((pos.x) + 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, a / 255)), line.c_str());
			window->DrawList->AddText(pFont, size, ImVec2((pos.x) - 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, a / 255)), line.c_str());

			window->DrawList->AddText(pFont, size, ImVec2(pos.x, pos.y + textSize.y * i), ImGui::GetColorU32(ImVec4(r / 255, g / 255, b / 255, a / 255)), line.c_str());
		}

		y = pos.y + textSize.y * (i + 1);
		i++;
	}

	return y;
}

void Renderer::DrawBulletTrace(const Angle weapon_angle, const Vector pos, uint32_t color)
{
	Vector forward;

	//weapon_angle.Clamp();
	M::AngleVectors(weapon_angle, &forward);

	Vector src3d = (pos - Vector(0.f, 0.f, 0.f));
	Vector dst3d = src3d + (forward * 800.f);

	Vector src, dst;
	//if (!M::DirectXWorldToScreen(src3d, src) || !M::DirectXWorldToScreen(dst3d, dst))
	//	return;
	M::DirectXWorldToScreen(src3d, src);
	M::DirectXWorldToScreen(dst3d, dst);

	DrawLine({ src.x, src.y }, { dst.x, dst.y }, color);
	//DrawLine({ dst.x - 3, dst.y - 3 }, { 6, 6 }, color);
}

void Renderer::DrawTank3DBox(const Vector Matrix3x3[3], Vector pos, Vector bbmin, Vector bbmax, uint32_t color)
{
	Vector ax[6];
	ax[0] = Matrix3x3[0] * bbmin[0]; // back
	ax[1] = Matrix3x3[1] * bbmin[1]; // down
	ax[2] = Matrix3x3[2] * bbmin[2]; // right
	ax[3] = Matrix3x3[0] * bbmax[0]; // front
	ax[4] = Matrix3x3[1] * bbmax[1]; // up
	ax[5] = Matrix3x3[2] * bbmax[2]; // left

	Vector temp[6];
	temp[0] = pos + ax[2];
	temp[1] = pos + ax[5];
	temp[2] = temp[0] + ax[3];
	temp[3] = temp[1] + ax[3];
	temp[4] = temp[0] + ax[0];
	temp[5] = temp[1] + ax[0];

	Vector v[8];
	v[0] = temp[2] + ax[1];
	v[1] = temp[2] + ax[4];
	v[2] = temp[3] + ax[4];
	v[3] = temp[3] + ax[1];
	v[4] = temp[4] + ax[1];
	v[5] = temp[4] + ax[4];
	v[6] = temp[5] + ax[4];
	v[7] = temp[5] + ax[1];

	Vector p1, p2;
	for (unsigned i = 0; i < 4; i++)
	{
		M::DirectXWorldToScreen(v[i], p1);
		M::DirectXWorldToScreen(v[(i + 1) & 3], p2);
		DrawLine(p1, p2, color * 4);	// red

		M::DirectXWorldToScreen(v[4 + i], p1);
		M::DirectXWorldToScreen(v[4 + ((i + 1) & 3)], p2);
		DrawLine(p1, p2, color);

		M::DirectXWorldToScreen(v[i], p1);
		M::DirectXWorldToScreen(v[4 + i], p2);
		DrawLine(p1, p2, color);
	}
}

void Renderer::DrawTank(const Vector Matrix3x3[3], Vector pos, Vector bbmin, Vector bbmax, uint32_t color)
{
	Vector points[] =
	{
		Vector(bbmin.x, bbmin.y, bbmin.z),
		Vector(bbmin.x, bbmax.y, bbmin.z),
		Vector(bbmax.x, bbmax.y, bbmin.z),
		Vector(bbmax.x, bbmin.y, bbmin.z),
		Vector(bbmax.x, bbmax.y, bbmax.z),
		Vector(bbmin.x, bbmax.y, bbmax.z),
		Vector(bbmin.x, bbmin.y, bbmax.z),
		Vector(bbmax.x, bbmin.y, bbmax.z)
	};

	for (auto &p : points)
	{
		p.x = p.Dot(Matrix3x3[0]);
		p.y = p.Dot(Matrix3x3[1]);
		p.z = p.Dot(Matrix3x3[2]);

		p += pos;
	}

	Draw3DBox(points, color);
}

void Renderer::Draw3DBox(Vector* boxVectors, uint32_t color)
{
	Vector boxVectors0, boxVectors1, boxVectors2, boxVectors3,
		boxVectors4, boxVectors5, boxVectors6, boxVectors7;

	//M::DirectXWorldToScreen(boxVectors[0], boxVectors0);
	//M::DirectXWorldToScreen(boxVectors[1], boxVectors1);
	//M::DirectXWorldToScreen(boxVectors[2], boxVectors2);
	//M::DirectXWorldToScreen(boxVectors[3], boxVectors3);
	//M::DirectXWorldToScreen(boxVectors[4], boxVectors4);
	//M::DirectXWorldToScreen(boxVectors[5], boxVectors5);
	//M::DirectXWorldToScreen(boxVectors[6], boxVectors6);
	//M::DirectXWorldToScreen(boxVectors[7], boxVectors7);

	if (M::DirectXWorldToScreen(boxVectors[0], boxVectors0) &&
		M::DirectXWorldToScreen(boxVectors[1], boxVectors1) &&
		M::DirectXWorldToScreen(boxVectors[2], boxVectors2) &&
		M::DirectXWorldToScreen(boxVectors[3], boxVectors3) &&
		M::DirectXWorldToScreen(boxVectors[4], boxVectors4) &&
		M::DirectXWorldToScreen(boxVectors[5], boxVectors5) &&
		M::DirectXWorldToScreen(boxVectors[6], boxVectors6) &&
		M::DirectXWorldToScreen(boxVectors[7], boxVectors7))
	{

		ImVec2 lines[12][2];
		//bottom of box
		lines[0][0] = { boxVectors0.x, boxVectors0.y };
		lines[0][1] = { boxVectors1.x, boxVectors1.y };
		lines[1][0] = { boxVectors1.x, boxVectors1.y };
		lines[1][1] = { boxVectors2.x, boxVectors2.y };
		lines[2][0] = { boxVectors2.x, boxVectors2.y };
		lines[2][1] = { boxVectors3.x, boxVectors3.y };
		lines[3][0] = { boxVectors3.x, boxVectors3.y };
		lines[3][1] = { boxVectors0.x, boxVectors0.y };

		lines[4][0] = { boxVectors0.x, boxVectors0.y };
		lines[4][1] = { boxVectors6.x, boxVectors6.y };

		// top of box
		lines[5][0] = { boxVectors6.x, boxVectors6.y };
		lines[5][1] = { boxVectors5.x, boxVectors5.y };
		lines[6][0] = { boxVectors5.x, boxVectors5.y };
		lines[6][1] = { boxVectors4.x, boxVectors4.y };
		lines[7][0] = { boxVectors4.x, boxVectors4.y };
		lines[7][1] = { boxVectors7.x, boxVectors7.y };
		lines[8][0] = { boxVectors7.x, boxVectors7.y };
		lines[8][1] = { boxVectors6.x, boxVectors6.y };

		lines[9][0] = { boxVectors5.x, boxVectors5.y };
		lines[9][1] = { boxVectors1.x, boxVectors1.y };

		lines[10][0] = { boxVectors4.x, boxVectors4.y };
		lines[10][1] = { boxVectors2.x, boxVectors2.y };

		lines[11][0] = { boxVectors7.x, boxVectors7.y };
		lines[11][1] = { boxVectors3.x, boxVectors3.y };

		for (int i = 0; i < 12; i++)
			DrawLine(lines[i][0], lines[i][1], color);
	}
}

void Renderer::DrawBox(const ImVec2& pos, float w, float h, uint32_t color)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	float a = (color >> 24) & 0xff;
	float r = (color >> 16) & 0xff;
	float g = (color >> 8) & 0xff;
	float b = (color) & 0xff;

	window->DrawList->AddRect({ pos.x - w, pos.y }, { pos.x + w, pos.y + h }, ImGui::GetColorU32(ImVec4(r / 255, g / 255, b / 255, a / 255)));
}


void Renderer::DrawLine(const ImVec2& from, const ImVec2& to, uint32_t color, float thickness)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	//float a = (color >> 24) & 0xff;
	//float r = (color >> 16) & 0xff;
	//float g = (color >> 8) & 0xff;
	//float b = (color) & 0xff;

	window->DrawList->AddLine(from, to, color/*ImGui::GetColorU32(ImVec4(pow(r / 255, g / 255, b / 255, a / 255))*/, thickness);
}

void Renderer::DrawCircle(const ImVec2& position, float radius, uint32_t color, float thickness)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	float a = (color >> 24) & 0xff;
	float r = (color >> 16) & 0xff;
	float g = (color >> 8) & 0xff;
	float b = (color) & 0xff;

	window->DrawList->AddCircle(position, radius, ImGui::GetColorU32(ImVec4(r / 255, g / 255, b / 255, a / 255)), 12, thickness);
}

void Renderer::DrawCircleFilled(const ImVec2& position, float radius, uint32_t color)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	float a = (color >> 24) & 0xff;
	float r = (color >> 16) & 0xff;
	float g = (color >> 8) & 0xff;
	float b = (color) & 0xff;

	window->DrawList->AddCircleFilled(position, radius, ImGui::GetColorU32(ImVec4(r / 255, g / 255, b / 255, a / 255)), 12);
}

Renderer* Renderer::GetInstance()
{
	if (!m_pInstance)
		m_pInstance = new Renderer();

	return m_pInstance;
}