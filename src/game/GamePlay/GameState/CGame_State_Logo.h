#if !CGAME_STATE_LOGO
#define CGAME_STATE_LOGO 1

#define LOGO_STEP_BEGIN_LOAD_RESOURCE	10
#define LOGO_MAX_STEP_LOADING			45

struct Foot
{
	Position _pos;
	int _state;
};
class CGameStateLogo :public CGameState
{
public:
	CGameStateLogo(){};

	void Update();
	void Render();
	bool UpdateTouchButton();
	void UpdateTouch();
	void Init();

	int _loadingStep;
	bool _autoInscreaseStep;
	BYTE _loadingType;
	int _loading_timeout;

	Foot _foot[LOGO_MAX_STEP_LOADING/3+1];

	void InitFoot();
	void UpdateFoot();
	void RenderFoot();
	
};

#endif