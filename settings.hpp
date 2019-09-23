#pragma once

#define SETTINGS_DIR "gajjin_spy"

template<typename T>
class SettingProperty {
public:
	SettingProperty(std::string _category, std::string _name, T *_value) : category(_category), name(_name), value(_value) { }
	std::string category, name;
	T *value;
};

class CSettings {
public:
	CSettings() { Setup(); }
	~CSettings()
	{
		Save();
		ints.clear();
		bools.clear();
		floats.clear();
	}

	void Load();
	void Save();
	void Setup();

private:
	void SetupValue(int& value, int def, std::string category, std::string name);
	void SetupValue(float& value, float def, std::string category, std::string name);
	void SetupValue(bool& value, bool def, std::string category, std::string name);

protected:
	std::vector< SettingProperty<int>* > ints;
	std::vector< SettingProperty<bool>* > bools;
	std::vector< SettingProperty<float>* > floats;
};

extern CSettings *Settings;