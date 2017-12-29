#if !CGAME_STATE_GAME_PLAY
#define CGAME_STATE_GAME_PLAY 1

#define GAMEPLAY_BUTTON_NUMS			4
#define GAMEPLAY_BUTTON_PAUSE			0
#define GAMEPLAY_BUTTON_REVIVE			1

#define GAMEPLAY_BUTTON_RESUM			2
#define GAMEPLAY_BUTTON_END				3

class CGameStateGamePlay :public CGameState
{
public:
	CGameStateGamePlay(){};
	Button _button[GAMEPLAY_BUTTON_NUMS];

	void Update();
	void Render();
	bool UpdateTouchButton();
	void UpdateTouch();
	void Init();
	bool _isPauseGame;
	CGameState *_stateMenu;
	
};

#endif

