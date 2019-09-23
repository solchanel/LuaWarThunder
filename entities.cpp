#include "main.hpp"

uint32_t CBaseEntity::GetTeam()
{
	if (!this)
		return 0;
	return Team;
}

//std::string CBaseEntity::GetName()
//{
//	if (!this)
//		return "null";
//	return std::string(this->Name, 64);
//}

bool CBaseEntity::IsEnemy()
{
	if (GetTeam() != G::LocalPlayer->GetTeam())
		return true;
	return false;
}

bool CBaseEntity::IsFriendly()
{
	if (GetTeam() == G::LocalPlayer->GetTeam())
		return true;
	return false;
}

bool CBaseEntity::IsAlive()
{
	return (State == EntityState::ALIVE);
}

EntityState CBaseEntity::GetState()
{
	if (!this)
		return EntityState::NONE;
	return (EntityState)State;
}

bool CBaseEntity::IsBot()
{
	if (!this)
		return false;
	return BotFlag;
}

float CBaseEntity::GetRespawnTime()
{
	if (!this)
		return 0.f;
	return TimeToRespawn;
}

Vector Unit::GetPosition()
{
	if (!this) return { 0.f, 0.f, 0.f };
	return this->Position;
}

UnitWeapons *Unit::GetUnitWeapons()
{
	if (!this) return nullptr;
	return this->Weapons;
}

UnitInfo *Unit::GetUnitInfo()
{
	if (!this) return nullptr;
	return this->Info;
}

const char *UnitInfo::GetName()
{
	if (!this) return "";
	return this->FullName;
}

const char *UnitInfo::GetShortName()
{
	if (!this) return "";
	return this->ShortName;
}

//Vector CBaseEntity::GetPosition()
//{
//	if (!this)
//		return {0.f, 0.f, 0.f};
//	return e_Unit->Position;
//}

Vector Unit::GetVelocity()	// :TODO:
{
	if (!this) return { 0.f, 0.f, 0.f };
	return this->ground_velocity;
}

Unit *CBaseEntity::GetUnit()
{
	if (!this || !e_Unit)
		return nullptr;
	return e_Unit;
}