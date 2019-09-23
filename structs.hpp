#pragma once

class Unit;
class AmmoInfo;
class WeaponInfo;
class RoundData;
class MPlayer;
class UnitInfo;
class MPlayerList;
class ScreenInfo;
class Game;
class LobbyInfo;
class MouseInfo;
class MousePos;
class Entities;
class BotList;
class BotEntities;

struct VMatrix
{
	float m[4][4];

	inline float* operator[](int i)
	{
		return m[i];
	}

	inline const float* operator[](int i) const
	{
		return m[i];
	}
};

class MousePos {
public:
	char pad_0000[20]; //0x0000
	unsigned int MouseFlag; //0x0014
	unsigned int MouseFlagState;
	float MouseX; //0x001C
	float MouseY; //0x0020
};

class CViewAngles {
public:
	void Set(Angle &angles)
	{
		if (!this) return;

		aim_angles.x = M_PI * angles.x / 180.f;
		aim_angles.y = M_PI * angles.y / 180.f;
	}
	Angle Get() const
	{
		Angle angles(0.f, 0.f, 0.f);

		if (!this) return angles;

		angles.x = aim_angles.x * 180.f / M_PI;
		angles.y = aim_angles.y * 180.f / M_PI;

		return angles;
	}
	ImVec2 aim_angles;
};

class MouseInfo {
public:
	char pad_0000[16]; //0x0000
	MousePos *MouseState; //0x0010
};

class LobbyInfo
{
public:
	char pad_0000[96]; //0x0000
	uint8_t HasSession; //0x0060
	char pad_0061[719]; //0x0061
	float CountDown; //0x0330
	char pad_0334[268]; //0x0334
}; //Size: 0x0440

struct D3DX11Matricies
{
	DirectX::XMMATRIX ViewMatrix;
	DirectX::XMMATRIX WorldMatrix;
	DirectX::XMMATRIX ProjectionMatrix;
};

class ScreenInfo {
public:
	char pad_0x0000[0x10]; //0x0000
	__int32 Width; //0x0010 
	__int32 Height; //0x0014
};

typedef enum : uint8_t {
	NONE = 0,
	MENU = 1,
	ALIVE = 2,
	DEAD = 3,
	LIMBO = 8,
} EntityState;

class CBaseEntity
{
private:
	char pad_0000[168]; //0x0000
	int64_t UID; //0x00A8
	char Name[64]; //0x00B0
	char pad_00F0[200]; //0x00F0
	uint32_t BotFlag; //0x01B8
	char pad_01BC[76]; //0x01BC
	int8_t Team; //0x0208
	char pad_0209[39]; //0x0209
	int8_t CountryID; //0x0230
	char pad_0231[39]; //0x0231
	int8_t MemberID; //0x0258
	char pad_0259[431]; //0x0259
	float TimeToRespawn; //0x0408
	char pad_040C[156]; //0x040C
	int8_t State; //0x04A8
	char pad_04A9[551]; //0x04A9
	Unit *e_Unit; //0x06D0
	char pad_06D8[1848]; //0x06D8

public:

	//std::string GetName();
	bool IsFriendly();
	bool IsEnemy();
	bool IsAlive();
	//int GetState();
	uint32_t GetTeam();
	EntityState GetState();
	//int GetFlags();
	float GetRespawnTime();
	Unit *GetUnit();
	bool IsBot();
	//bool IsVisible();
	//bool IsDormant();
	//Vector GetPosition();
}; //Size: 0x0e10

class UnitInfo
{
public:
	const char *GetName();
	const char *GetShortName();

	char *ClassName; //0x0000
	char *ModelName; //0x0008
	char *BlkName; //0x0010
	char *FullName; //0x0018
	char *ShortName; //0x0020
	char *VehicleType; //0x0028
	char *N000012D2; //0x0030
	char *NationName; //0x0038

	char pad_0040[404]; //0x0040
	float ZoomCrosshairScale; // 0x01D4
	float ZoomMultipler; //0x01D8
	char pad_01DC[16]; //0x01DC
	float ZoomShadowMulti; //0x01EC
}; //Size: 0x0040

class RoundData
{
public:
	float mass; //0x0000 
	float caliber; //0x0004 
	float distance; //0x0008 
	float length; //0x000C 
	float N00002EBC; //0x0010 
	float N00002EBD; //0x0014 
	float N00002EBe; //0x0018 
	float N00003003; //0x001C 
	float unk2; //0x0020 
	float velocity; //0x0024
};

class AmmoInfo
{
public:
	RoundData* RoundData; //0x0000 
	char pad_0008[172]; //0x0008
	uint32_t Ammo; //0x00B4
	uint32_t AmmoWasted; //0x00B8
	char pad_00BC[12]; //0x00BC
};

class WeaponInfo
{
public:
	char pad_0x0000[0x60]; //0x0000
	__int32 Decrement; //0x0060 
	char pad_0x0064[4]; //0x0064
	AmmoInfo AmmoInfo[5]; //0x0068 
};

class Weapon
{
public:

	char pad_0000[124]; //0x0000
	Vector bbmin; //0x007C
	char pad_0088[36]; //0x0088
	Vector bbmax; //0x00AC
	Vector Turret_X_RotationMatrix[3]; //0x00B8
	char pad_00DC[12]; //0x00DC
	Vector Turret_Y_RotationMatrix[3]; //0x00E8
	char pad_010C[324]; //0x010C
	class WeaponInfo *WeaponInfo; //0x0250
	char pad_0258[541]; //0x0258
	uint8_t CurrentSlot; //0x0471
	char pad_0472[66]; //0x0472
	Vector Position; //0x04B4
	char pad_04C0[384]; //0x04C0
};

class UnitWeapons
{
public:
	RoundData *GetRoundInfo(unsigned index) {
		return Weapons[0]->WeaponInfo->AmmoInfo[index].RoundData;
	}
	__int32 GetRoundAmmount(unsigned index) {
		return Weapons[0]->WeaponInfo->AmmoInfo[index].Ammo;
	}

	Angle GetWeaponAngle(unsigned index) {
		Angle x_weapon, y_weapon;
		M::MatrixAngles(Weapons[index]->Turret_X_RotationMatrix, x_weapon);
		M::MatrixAngles(Weapons[index]->Turret_Y_RotationMatrix, y_weapon);

		return Angle(x_weapon.x, y_weapon.y, 0.f);
	}

	char pad_0000[688]; //0x0000
	Weapon** Weapons; //0x02B0
	__int64 WeaponCount; //0x02B8
};

class UnitRef {
public:
};

class Unit
{
public:
	bool operator ==(const Unit &unit)
	{
		return this == &unit;
	}

	Vector GetPosition();
	UnitInfo *GetUnitInfo();
	UnitWeapons *GetUnitWeapons();

	Vector GetVelocity();

	UnitRef *GetUnitRef()
	{
		if (!this) return nullptr;
		return this->unit_ref;
	}

	char pad_0000[16]; //0x0000
	UnitRef *unit_ref; //0x0010
	char pad_0018[496]; //0x0018
	Vector bbmax; //0x0208
	Vector bbmin; //0x0214
	char pad_0220[1300]; //0x0220
	Vector RotationMatrix[3]; //0x0734
	Vector Position; //0x0758
	char pad_0764[1588]; //0x0764
	UnitInfo *Info; //0x0D98
	char pad_0DA0[168]; //0x0DA0
	UnitWeapons *Weapons; //0x0E48
	char pad_0E28[13856]; //0x0E28
	float gunnerTargetingQuality; //0x4448
	char pad_444C[36]; //0x444C
	float gunnerTargetingSpeedYawMult; //0x4470
	char pad_4474[36]; //0x4474
	float gunnerTargetingSpeedPitchMult; //0x4498
	char pad_449C[23620]; //0x449C
	Vector ground_velocity; //0xA0E0
}; //Size: 0x0E18

#define MAX_CLIENTS 64
class Entities
{
public:
	class CBaseEntity *EntList[MAX_CLIENTS]; //0x0000
}; //Size: 0x0200

class MPlayerList
{
public:
	Entities *Entities; //0x0000 
	unsigned char MaxPlayers; //0x0008 
};

class BotEntities
{
public:
	class Unit *EntList[MAX_CLIENTS]; //0x0000
}; //Size: 0x0200

class BotList
{
public:
	BotEntities *Entities; //0x0000 
	unsigned char MaxPlayers; //0x0008 
};

class GameInfo
{
public:
	char pad_0000[212]; //0x0000
	uint8_t IsInFlight; //0x00D4
	char pad_00D5[1]; //0x00D5
	uint8_t IsInHangar; //0x00D6
	char pad_00D7[657]; //0x00D7
	BotList Bots; //0x0368
	char pad_0370[3200]; //0x0370
}; //Size: 0x0888

class PlayerView
{
public:
	float timescale; //0x0000
	char pad_0004[28]; //0x0004
	bool AutoTarget; //0x0020
	bool AutoTargetLock; //0x0021
	char pad_0022[31]; //0x0022
	bool ThirdPerson; //0x0041
	char pad_0042[527]; //0x0042
	bool PlaneAimIndicatorToPlane; // 0x0251
	bool PlaneAimIndicator; //0x0252
	char pad_0253[9]; //0x0253
	bool PenetrationCrosshair; //0x025C
	bool UnitGlow; //0x025D
	bool DrawTankDistance; //0x025E
	char pad_025F[63]; //0x025F
	bool DrawBombIndicator; //0x029E
}; //Size: 0x03CE