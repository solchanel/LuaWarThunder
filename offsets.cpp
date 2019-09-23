#include "main.hpp"

ScreenInfo		*O::Screen;
GameInfo		*O::Game;
MPlayerList		*O::List;
D3DX11Matricies *O::Matrices;
LobbyInfo		*O::Lobby;
PlayerView		*O::ClientView;
MouseInfo		*O::MouseState;
CViewAngles		*O::ViewAngles;

void O::Initialize()
{
	HMODULE hAces = GetModuleHandle(NULL);

	std::cout << "Game: 0x" << std::hex << *U::GetOffsetX64<DWORD64 *>(U::FindPattern<DWORD64>(hAces, "48 8B 15 ?? ?? ?? ?? 48 8B 82 ?? ?? ?? ?? 80 B8 ?? ?? ?? ?? 00"), OFFSET_OP::MOV) << std::endl;
	std::cout << "LocalMPlayer: 0x" << std::hex << *U::GetOffsetX64<DWORD64 *>(U::FindPattern<DWORD64>(hAces, "4C 39 15 ?? ?? ?? ?? 45 8B DA"), OFFSET_OP::MOV) << std::endl;
	std::cout << "MPlayerList: 0x" << std::hex << *U::GetOffsetX64<DWORD64 *>(U::FindPattern<DWORD64>(hAces, "48 8B 05 ?? ?? ?? ?? 48 63 D2 48 8B 3C"), OFFSET_OP::CMP) << std::endl;
	std::cout << "ScreenInfo: 0x" << std::hex << *U::GetOffsetX64<DWORD64 *>(U::FindPattern<DWORD64>(hAces, "48 89 1D ?? ?? ?? ?? 0F 14 C1"), OFFSET_OP::LEA) << std::endl;
	std::cout << "Matrices: 0x" << std::hex << U::GetOffsetX64<DWORD64 *>(U::FindPattern<DWORD64>(hAces, "4C 8D 3D ? ? ? ? BF ? ? ? ? 48 8B E9 C1 C7 00 41 8B DE 8B F7"), OFFSET_OP::LEA) << std::endl;
	std::cout << "ClientView: 0x" << std::hex << U::GetOffsetX64<DWORD64 *>(U::FindPattern<DWORD64>(hAces, "48 8B 05 ?? ?? ?? ?? 0F B6 40 4A C3"), OFFSET_OP::LEA) << std::endl;
	std::cout << "MouseState: 0x" << std::hex << U::GetOffsetX64<DWORD64 *>(U::FindPattern<DWORD64>(hAces, "48 89 05 ? ? ? ? 45 33 C0 48 8D 54 24"), OFFSET_OP::LEA) << std::endl;
	std::cout << "ViewAngles: 0x" << std::hex << U::GetOffsetX64<DWORD64 *>(U::FindPattern<DWORD64>(hAces, "48 8D 3D ? ? ? ? F3 0F 10 15 ? ? ? ? 84 D2 0F 28 C3"), OFFSET_OP::LEA) << std::endl;

	std::cout << "Addresses:" << std::endl;

	std::cout << "Game: 0x" << std::hex << U::GetOffsetX64<DWORD64 *>(U::FindPattern<DWORD64>(hAces, "48 8B 15 ?? ?? ?? ?? 48 8B 82 ?? ?? ?? ?? 80 B8 ?? ?? ?? ?? 00"), OFFSET_OP::MOV) << std::endl;
	std::cout << "LocalMPlayer: 0x" << std::hex << U::GetOffsetX64<DWORD64 *>(U::FindPattern<DWORD64>(hAces, "4C 39 15 ?? ?? ?? ?? 45 8B DA"), OFFSET_OP::MOV) << std::endl;
	std::cout << "MPlayerList: 0x" << std::hex << U::GetOffsetX64<DWORD64 *>(U::FindPattern<DWORD64>(hAces, "48 8B 05 ?? ?? ?? ?? 48 63 D2 48 8B 3C"), OFFSET_OP::CMP) << std::endl;
	std::cout << "ScreenInfo: 0x" << std::hex << U::GetOffsetX64<DWORD64 *>(U::FindPattern<DWORD64>(hAces, "48 89 1D ?? ?? ?? ?? 0F 14 C1"), OFFSET_OP::LEA) << std::endl;
	std::cout << "Matrices: 0x" << std::hex << U::GetOffsetX64<DWORD64 *>(U::FindPattern<DWORD64>(hAces, "4C 8D 3D ? ? ? ? BF ? ? ? ? 48 8B E9 C1 C7 00 41 8B DE 8B F7"), OFFSET_OP::LEA) << std::endl;
	std::cout << "ClientView: 0x" << std::hex << U::GetOffsetX64<DWORD64 *>(U::FindPattern<DWORD64>(hAces, "48 8B 05 ?? ?? ?? ?? 0F B6 40 4A C3"), OFFSET_OP::LEA) << std::endl;
	std::cout << "MouseState: 0x" << std::hex << U::GetOffsetX64<DWORD64 *>(U::FindPattern<DWORD64>(hAces, "48 89 05 ? ? ? ? 45 33 C0 48 8D 54 24"), OFFSET_OP::LEA) << std::endl;
	std::cout << "ViewAngles: 0x" << std::hex << U::GetOffsetX64<DWORD64 *>(U::FindPattern<DWORD64>(hAces, "48 8D 3D ? ? ? ? F3 0F 10 15 ? ? ? ? 84 D2 0F 28 C3"), OFFSET_OP::LEA) << std::endl;

	O::Game = (GameInfo *)*U::GetOffsetX64<DWORD64 *>(U::FindPattern<DWORD64>(hAces, "48 8B 15 ?? ?? ?? ?? 48 8B 82 ?? ?? ?? ?? 80 B8 ?? ?? ?? ?? 00"), OFFSET_OP::MOV);
	G::LocalPlayer = (CBaseEntity *)*U::GetOffsetX64<DWORD64 *>(U::FindPattern<DWORD64>(hAces, "4C 39 15 ?? ?? ?? ?? 45 8B DA"), OFFSET_OP::CMP);
	O::List = (MPlayerList *)U::GetOffsetX64<DWORD64 *>(U::FindPattern<DWORD64>(hAces, "48 8B 05 ?? ?? ?? ?? 48 63 D2 48 8B 3C"), OFFSET_OP::CMP);
	O::Screen = (ScreenInfo *)*U::GetOffsetX64<DWORD64 *>(U::FindPattern<DWORD64>(hAces, "48 89 1D ?? ?? ?? ?? 0F 14 C1"), OFFSET_OP::MOV);
	O::Matrices = (D3DX11Matricies *)U::GetOffsetX64<DWORD64 *>(U::FindPattern<DWORD64>(hAces, "4C 8D 3D ? ? ? ? BF ? ? ? ? 48 8B E9 C1 C7 00 41 8B DE 8B F7"), OFFSET_OP::LEA);
	O::ClientView = (PlayerView *)*U::GetOffsetX64<DWORD64 *>(U::FindPattern<DWORD64>(hAces, "48 8B 05 ?? ?? ?? ?? 0F B6 40 4A C3"), OFFSET_OP::MOV);
	O::MouseState = (MouseInfo *)*U::GetOffsetX64<DWORD64 *>(U::FindPattern<DWORD64>(hAces, "48 89 05 ? ? ? ? 45 33 C0 48 8D 54 24"), OFFSET_OP::MOV);
	O::ViewAngles = (CViewAngles *)U::GetOffsetX64<DWORD64 *>(U::FindPattern<DWORD64>(hAces, "48 8D 3D ? ? ? ? F3 0F 10 15 ? ? ? ? 84 D2 0F 28 C3"), OFFSET_OP::LEA);

	std::cout << "Base: " << std::hex << hAces << std::endl;
	G::pGameLocalPlayer.Set(U::GetOffsetX64<DWORD64>(U::FindPattern<DWORD64>(hAces, "4C 39 15 ?? ?? ?? ?? 45 8B DA"), OFFSET_OP::CMP));
}