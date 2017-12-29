#if !CGAME_STATE_SETTING
#define CGAME_STATE_SETTING 1

#define SETTING_BUTTON_NUMS					5

class CGameStateSetting :public CGameState
{
public:
	CGameStateSetting(){};
	Button _button[SETTING_BUTTON_NUMS];

	void Update();
	void Render();
	bool UpdateTouchButton();
	void UpdateTouch();
	void Init();
};

#endif