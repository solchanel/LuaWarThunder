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


