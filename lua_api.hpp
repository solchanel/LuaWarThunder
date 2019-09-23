#pragma once

namespace LuaAPI 
{
	template<typename... Args> void LuaRegisterCFunction(std::string fn_name, Args&& ...params) {
		G::Lua.set_function(fn_name, std::forward<Args>(params)...);
	}

	template<typename T> void SetLuaVariable(std::string var_name, T Param) {
		G::Lua.globals()[var_name] = Param;
	}

	template<typename T> std::optional<T> GetLuaVariable(std::string var_name) {
		auto r = G::Lua.globals()[var_name];
		if (!r.is_valid())
			return std::nullopt;
		return std::optional<T>(r);
	}

	template<typename... Args> void CreateLuaClass(std::string class_name, Args&& ...params) {
		G::Lua.set(class_name, std::forward<Args>(params)...);
	}

	template <typename... Args> void CreateLuaTable(std::string var_name, Args&& ...params) {
		G::Lua.globals()[var_name] = G::Lua.create_table_with(std::forward<Args>(params)...);
	}

	template<typename Sig, typename ...Args> typename std::function<Sig>::result_type LuaCallback(std::string cb_name, Args&& ...params) {
		sol::function fn = G::Lua.globals()[cb_name];
		if (!fn.valid())
			return;
		std::function<Sig> sfx = fn;
		return sfx(std::forward<Args>(params)...);
	}

	std::optional<sol::table>	GetLuaTable(std::string var_name);
	sol::table					CreateNamespace(std::string ns_name);
	sol::function				RegisterLuaFunction(std::string function_name);
}