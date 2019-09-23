#include "main.hpp"
#include <ShlObj.h>
#include <string>

CSettings *Settings;

void CSettings::Setup()
{
	// Visuals
	SetupValue(Vars.Visuals.Enabled, false, "Visuals", "Enabled");
	SetupValue(Vars.Visuals.DrawDistance, false, "Visuals", "DrawDistance");
	SetupValue(Vars.Visuals.DrawBox, false, "Visuals", "DrawBox");
	SetupValue(Vars.Visuals.Thirdperson, true, "Visuals", "Thirdperson");
	SetupValue(Vars.Visuals.ShortName, false, "Visuals", "ShortName");
	SetupValue(Vars.Visuals.FullName, false, "Visuals", "FullName");
	SetupValue(Vars.Visuals.ZoomMulti, 0.f, "Visuals", "ZoomMultipler");
	SetupValue(Vars.Visuals.ScopeShadow, 0.f, "Visuals", "ShadowMultipler");
	SetupValue(Vars.Visuals.Showbots, false, "Visuals", "ShowBots");
	SetupValue(Vars.Visuals.ShowEnemies, false, "Visuals", "ShowEnemies");
	SetupValue(Vars.Visuals.ShowFriendly, false, "Visuals", "ShowFriends");
	SetupValue(Vars.Visuals.WeaponTrace, false, "Visuals", "DrawTraces");

	// Aimbot
	SetupValue(Vars.Aimbot.Enabled, false, "Aimbot", "Enabled");
	SetupValue(Vars.Aimbot.AimFriendly, false, "Aimbot", "AimFriendly");
	SetupValue(Vars.Aimbot.Fov, 0.f, "Aimbot", "Fov");
	SetupValue(Vars.Aimbot.Key, 0, "Aimbot", "Key");

	// Triggerbot
	SetupValue(Vars.Triggerbot.Enabled, false, "Triggerbot", "Enabled");
	SetupValue(Vars.Triggerbot.Filter.Green, false, "Triggerbot", "FilterGreen");
	SetupValue(Vars.Triggerbot.Filter.Yellow, false, "Triggerbot", "FilterYellow");
	SetupValue(Vars.Triggerbot.Filter.Red, false, "Triggerbot", "FilterRed");
	SetupValue(Vars.Triggerbot.Key, 0, "Triggerbot", "Key");

	// Misc
	SetupValue(Vars.Misc.DrawDistanceInScope, false, "Misc", "DrawInGameDistance");
	SetupValue(Vars.Misc.DrawPenetrationIndicator, false, "Misc", "DrawPenetration");
	SetupValue(Vars.Misc.DrawPlaneIndicator, false, "Misc", "DrawPlaneIndicator");
	SetupValue(Vars.Misc.DrawBombIndicator, false, "Misc", "DrawBombIndicator");
	SetupValue(Vars.Misc.ArcadeAutoTargeting, false, "Misc", "ArcadeAutoTargeting");
	SetupValue(Vars.Misc.UnitGlow, false, "Misc", "UnitGlow");
	SetupValue(Vars.Misc.HideBoxWhileGlow, false, "Misc", "HideBoxWhileGlow");
	SetupValue(Vars.Misc.Timescale, 0.f, "Misc", "Timescale");
}

void CSettings::Save()
{
	static TCHAR path[MAX_PATH];
	std::string dir, file;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path)))
	{
		dir = std::string(path) + "\\" + SETTINGS_DIR + "\\";
		file = std::string(path) + "\\" + SETTINGS_DIR + "\\settings.ini";
	}

	CreateDirectory(dir.c_str(), NULL);

	for (auto value : ints)
		WritePrivateProfileString(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());
	for (auto value : floats)
		WritePrivateProfileString(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());
	for (auto value : bools)
		WritePrivateProfileString(value->category.c_str(), value->name.c_str(), *value->value ? "true" : "false", file.c_str());
}

void CSettings::Load()
{
	static TCHAR path[MAX_PATH];
	std::string dir, file;
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path)))
	{
		dir = std::string(path) + "\\" + SETTINGS_DIR + "\\";
		file = std::string(path) + "\\" + SETTINGS_DIR + "\\settings.ini";
	}

	CreateDirectory(dir.c_str(), NULL);

	char value_l[32] = { '\0' };
	for (auto value : ints) {
		GetPrivateProfileString(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = std::atof(value_l);
	}
	for (auto value : floats) {
		GetPrivateProfileString(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = std::atof(value_l);
	}
	for (auto value : bools) {
		GetPrivateProfileString(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = !strcmp(value_l, "true");
	}
}

void CSettings::SetupValue(int& value, int def, std::string category, std::string name)
{
	value = def;
	ints.push_back(new SettingProperty< int >(category, name, &value));
}

void CSettings::SetupValue(float& value, float def, std::string category, std::string name)
{
	value = def;
	floats.push_back(new SettingProperty< float >(category, name, &value));
}

void CSettings::SetupValue(bool& value, bool def, std::string category, std::string name)
{
	value = def;
	bools.push_back(new SettingProperty< bool >(category, name, &value));
}