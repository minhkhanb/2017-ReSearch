#if !CGAME_STATE_MAIN_MENU
#define CGAME_STATE_MAIN_MENU 1

#define MAINMENU_BUTTON_NUMS					10
#define MAINMENU_BUTTON_LEADER_BROAD			0
#define MAINMENU_BUTTON_ACHIEVENTMENT			1
#define MAINMENU_BUTTON_PLAY					2
#define MAINMENU_BUTTON_STORE					3
#define MAINMENU_BUTTON_SETTING					4
#define MAINMENU_BUTTON_SHARE					5

#define MAINMENU_BUTTON_ABOUT					6
#define MAINMENU_BUTTON_NOTI					7
#define MAINMENU_BUTTON_MUSIC					8
#define MAINMENU_BUTTON_SOUND					9

#define END_TRANSACTION							100
class CGameStateMainMenu :public CGameState
{

public:
	CGameStateMainMenu(){};

	Button _button[MAINMENU_BUTTON_NUMS];

	void Update();
	void Render();
	bool UpdateTouchButton();
	void UpdateTouch();
	void Init();

	CAnimObject _animTitle;
	CAnimObject _animNew;
	float _rota;
	float _offTitleX,_offTitleY;
	//effect
	int _typeTransaction;
	enum enumTypeTransaction
	{
		TRANSACTION_INIT_MAIN_MENU,
		TRANSACTION_GO_TO_GAME,
		TRANSACTION_GO_TO_SETTING_STORE,
		TRANSACTION_COME_BACK_MENU,
	};
	float _offX,_offY,_speed;	
	int _stateTransaction, _opacity;

	void InitTransaction(int type);
	void UpdateTransaction();
	void UpdateInitMenu();
	void UpdateGoToGame();
	void UpdateComeBackMenu();

	//result
	float _offScoreX, _offScoreY;
	float _scaleHighScore,_opacityHighScore;
	int _score,_offScore,_opacityResult,_opacityGlow;
	void RenderResult();

	//seeting
	bool _isRenderSetting;
	void InitSetting();
	void CloseSetting();
	void RenderSetting();
	float _offsetSettingY;
	int _stateSetting;
	CGameState *_statePoup;
	CAnimObject _animText;

	//about
	float _offsetAboutX, _opacityAboutGB;
	bool _isRenderAbout;
	int _stateAbout, _opacityAbout;
	CAnimObject _animDuck;
	void InitAbout();
	void CloseAbout();
	void RenderAbout();
	
};

#endif