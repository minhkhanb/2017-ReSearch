#if !CGAME_STATE_STORE
#define CGAME_STATE_STORE 1

#define SETTING_BUTTON_NUMS					5

class CGameStateStore :public CGameState
{
public:
	CGameStateStore(){};
	Button _button[SETTING_BUTTON_NUMS];

	void Update();
	void Render();
	bool UpdateTouchButton();
	void UpdateTouch();
	void Init();
};

#endif