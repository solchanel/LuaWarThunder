#include "main.hpp"

sol::function LuaAPI::RegisterLuaFunction(std::string function_name) {
	sol::function fn = G::Lua.globals()[function_name];
	return fn;
}

sol::table LuaAPI::CreateNamespace(std::string ns_name) {
	return G::Lua.create_named_table(ns_name);
}

std::optional<sol::table> LuaAPI::GetLuaTable(std::string var_name) {
	sol::table r = G::Lua.globals()[var_name];
	if (!r.valid())
		return std::nullopt;
	return std::optional<sol::table>(r);
}

struct LuaLocalManager
{
	bool is_local_valid() { return G::LocalPlayer ? true : false; }
	CBaseEntity *get_local() const { return G::LocalPlayer ? G::LocalPlayer : nullptr; }
	int get_max_clients()
	{
		return O::List->MaxPlayers;
	}
	CBaseEntity *get_ent(const int index) const
	{
		return O::List->Entities->EntList[index];
	}
	sol::object get(sol::stack_object key, sol::this_state L)
	{
		sol::optional<int> numeric_key = key.as<sol::optional<int>>();
		if (numeric_key)
		{
			if(O::List && O::List->Entities->EntList[*numeric_key] && *numeric_key < O::List->MaxPlayers)
				return sol::object(L, sol::in_place, O::List->Entities->EntList[*numeric_key]);
		}
		return sol::object(L, sol::in_place, sol::lua_nil);
	}
};

void LuaAPI::RegisterTypes()
{
	auto LuaRenderer = G::Lua["Renderer"].get_or_create<sol::table>();

	LuaRenderer.set_function("DrawString", &LuaRenderer::DrawString);
	LuaRenderer.set_function("DrawLine", &LuaRenderer::DrawLine);
	LuaRenderer.set_function("DrawCircle", &LuaRenderer::DrawCircle);
	LuaRenderer.set_function("DrawCircleFilled", &LuaRenderer::DrawCircleFilled);

	G::Lua.new_usertype<GameInfo>("GameInfo",
		"is_in_hangar", &GameInfo::IsInHangar,
		"is_in_flight", &GameInfo::IsInFlight
		);

	G::Lua.new_usertype<Vector>("Vector", sol::constructors<Vector(), Vector(float, float, float), Vector(float, float)>(),
		"x", &Vector::x,
		"y", &Vector::y,
		"z", &Vector::z,
		"dot", &Vector::Dot,
		"len", &Vector::Length,
		"len2d", &Vector::Length2D,
		"normalize", &Vector::Normalize,
		"is_zero", &Vector::IsZero,
		sol::meta_function::addition, &Vector::operator+,
		sol::meta_function::subtraction, &Vector::operator-,
		sol::meta_function::to_string, &Vector::ToString,
		sol::meta_function::length, &Vector::Length,
		sol::meta_function::index, [](Vector &v, const int index) { if (index < 0 || index > 2) return 0.0f; return v[index]; },
		sol::meta_function::new_index, [](Vector &v, const int index, double x) { if (index < 0 || index > 2) return; v[index] = x; },
		sol::meta_function::equal_to, &Vector::operator==
		);

	G::Lua.new_usertype<Angle>("Angle", sol::constructors<Angle(), Angle(float, float, float)>(),
		"x", &Angle::x,
		"y", &Angle::y,
		"z", &Angle::z,
		"len", &Angle::Length,
		"normalize", &Angle::Normalize,
		"clamp", &Angle::Clamp,
		"is_zero", &Angle::IsZero,
		sol::meta_function::addition, &Angle::operator+,
		sol::meta_function::subtraction, &Angle::operator-,
		sol::meta_function::to_string, &Angle::ToString,
		sol::meta_function::length, &Angle::Length
		);

	G::Lua.new_usertype<CViewAngles>("ViewAngles",
		"set", &CViewAngles::Set,
		"get", &CViewAngles::Get
		);

	G::Lua.new_usertype<Unit>("Unit",
		"get_position", &Unit::GetPosition,
		"get_info", &Unit::GetUnitInfo,
		"get_velocity", &Unit::GetVelocity,
		"get_weapons", &Unit::GetUnitWeapons,
		"bbmin", &Unit::bbmin,
		"bbmax", &Unit::bbmax
		);

	G::Lua.new_usertype<UnitInfo>("UnitInfo",
		"shortname", &UnitInfo::ShortName,
		"fullname", &UnitInfo::FullName,
		"classname", &UnitInfo::ClassName,
		"zoom_scale", &UnitInfo::ZoomCrosshairScale,
		"zoom_multi", &UnitInfo::ZoomMultipler,
		"zoom_shadow_multi", &UnitInfo::ZoomShadowMulti
		);

	G::Lua.new_usertype<CBaseEntity>("CBaseEntity",
		"get_unit", &CBaseEntity::GetUnit,
		"get_state", &CBaseEntity::GetState,
		"get_team", &CBaseEntity::GetTeam,
		"get_respawn_time", &CBaseEntity::GetRespawnTime,
		"is_alive", &CBaseEntity::IsAlive,
		"is_bot", &CBaseEntity::IsBot,
		"is_enemy", &CBaseEntity::IsEnemy,
		"is_friendly", &CBaseEntity::IsFriendly
		);

	G::Lua.new_usertype<LuaLocalManager>("wt_global",
		sol::default_constructor,
		"is_local_valid", &LuaLocalManager::is_local_valid,
		"get_local", &LuaLocalManager::get_local,
		"max_clients", sol::property(&LuaLocalManager::get_max_clients),
		sol::meta_function::index, &LuaLocalManager::get
		);

	G::Lua["wt_global"]		= LuaLocalManager{};
	G::Lua["max_clients"]	= []() { return O::List->MaxPlayers; };

	G::Lua["game"] = O::Game;
	G::Lua["viewangles"] = O::ViewAngles;
}
