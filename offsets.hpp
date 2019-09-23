#pragma once

namespace O
{
	extern ScreenInfo		*Screen;
	extern GameInfo			*Game;
	extern MPlayerList		*List;
	extern LobbyInfo		*Lobby;
	extern D3DX11Matricies	*Matrices;
	extern PlayerView		*ClientView;
	extern MouseInfo		*MouseState;
	extern CViewAngles		*ViewAngles;

	void Initialize();
}
