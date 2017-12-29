#if !defined(GAMEPLAY_H)
#define GAMEPLAY_H

#define MAX_DUCK_IN_GAME		10
#if defined MAC_OS
#define USE_CONTROL_STICK		0
#else
#define USE_CONTROL_STICK		1
#endif

#define BULLSEYE_FIND				0
#define BULLSEYE_AUTO_TARGET		1
#define BULLSEYE_FOLLOW_TARGET		2
#define BULLSEYE_FILL_GUN			3
#define BULLSEYE_AUTO_TARGET_FEVER	4

#define TIME_FILL_GUN				3


struct LaneInitDuck
{
	bool _isUsed;
	int _min, _max;
};
struct CharFever
{
	bool _isCollected;
	Position _pos;
	int _state,_stateRota,_stateScale,_stateOpacity, _opacity,_timedeplay;
	float _rota,_speedRota,_speedScale, _scale;

};
#define NUM_CHAR_EFFECT_FEVER			6
#define NUM_CHAR_FEVER					5
class CGamePlay
{
public:
	enum enumStateBullseye
	{
		BULLSEYE_STATE_FIND,
		BULLSEYE_STATE_CHECK_SNAP,
		BULLSEYE_STATE_FOLLOW_TARGET,
		BULLSEYE_STATE_GUN,
        BULLSEYE_STATE_CHECK_SHAK,
		BULLSEYE_STATE_AUTO_FIND,
		BULLSEYE_STATE_MISS_DUCK,
	};

	enum enumStateDog
	{
		DOG_STATE_BEGIN,
		DOG_STATE_MOVE,
		DOG_STATE_GUN,
		DOG_STATE_CHECK_TUTORIAL,
	};
	CDuck *_duckBeActack;
	bool _isFerverMode;
	int _typeRevive;
	enum  enumTypeRevive
	{
		TYPE_REVIVE_NONE,
		TYPE_REVIVE_ADS,
		TYPE_REVIVE_LOGIN,
		TYPE_REVIVE_INVITE
	};
	//bg
	CTree _tree;
	CBackGround _bgIngame;
    CBackGround _bgLayerMounth1,_bgLayerMounth2,_bgLayerMounth3;
    CBackGround _bgLayer1, _bgLayer2, _bgLayer3, _bgLayer4;
	CCloud _cloud;

	//bullseye
	//float _speedMoveBullseye;
	CAnimObject _animBullseye;
	CAnimObject _animDog;
	CAnimObject _animMiss[MAX_DUCK_IN_GAME];
	CAnimObject _animShot[MAX_DUCK_IN_GAME];

	CAnimObject _animGameOver;
	Position _bullseye;
	BYTE _stateBullseye;
    float _offTree;
	int _timeCheckSnap;
	//dog
	Position _posDog;
	int _stateDog;
    float _speedDog;
	//design
	INT_TIME _timeAddDuck;
	int _dangerousSpace;
	//fever
	int _percentAddFever;
	int _maxDuck;	
	int _numCombo;
	int _timeCombo;
	int _live;
	int _maxRanDuck;
	int _typeDuckRan[10];
	int _level;
	INT32 _score;
	bool _startGame,_isPlaying,_playSoundHighScore,_isNewScore;
	float _acclBullseye;
    float _base_bull_speedX;
    float _base_bull_speedY;
    int _offTargetFeverX, _offTargetFeverY;
	//control
    bool _isShaking;
#if USE_CONTROL_STICK
	int _touchID;
	float _touch_drag_x, _touch_drag_y;
	float _stickX, _stickY;
	float _stickCenterX, _stickCenterY;
	float _stickRenderX, _stickRenderY;
	float _stickCenterRenderX, _stickCenterRenderY;
	bool _isRenderStick;
	float _angleMoveBullseys, _speedMove;
	
#else

#endif // USE_CONTROL_STICK
	CDuck _duck[MAX_DUCK_IN_GAME];
	
	void NewGame();
	void Init();
	void Render();
	void Update();
	void UpdateTime();
	void UpdateDuck();
	void UpdateDuckFever();
	void UpdateBullseys();
	void UpdateDog();
	void UpdateTouch();
	void UpdateTouchControl();
	bool isMoveToTarget(float &x1, float &y1, float x2, float y2, float speed, float distanceMin,float offspeedx = 0 ,float offspeedy = 0);
	void AddDuck();
	void AddDuck(int type,float x, float y);
	void GetDuckFearAfterGun();
	int GetIndexDuckNearBullseye(Position bullseye, int stateDuck, int limit = 0);
	int GetIndexDuckFearTarget(Position bullseye);
	void ClearDuckFear(int state);
	void SetAllDuckState(int state);
	void CheckNewLVL(INT32 score);
	void SetNewValue(int lvl);
	void GetTypeDuckAdd(int lvl);
	int CountDuck();
    void DoShaking();
	void StopMoveBG();
	void RenderTimeBar(float x, float y);
	void AddAnimMiss(float x, float y);
	void AddAnimShoot(float x, float y);
	LaneInitDuck _lane[4];
	void InitLaneDuck(int lvl);
	void RemoveLaneUse(int index);
	int _minLaneX, _maxLaneX;
	int _oldLane[4],_numLaneUse;

    int _frame_miss;
    float _frame_accellSpeed;
	float _distanceNear,_oldDistanceNear;
	float _oldPosBullX, _offPosBullX;
	int _indexDuckFear;
	int _directionMove;
	//render eclip
	float _opacity;
	CAnimObject _animSun;
	Position _posSun,_elipCenter;
	float _rotaSun, _speedSun, _elipAngle, _offsetSunY,_maxDistance,_scaleSun;
	int _stateEffecetSun, _stateOpa,_timeSun;
	int _elipDX, _elipDY;
	void AddSun(float x, float y, int dx, int dy, float speed);
	void UpdateSun();
	void RenderSun();

	void RenderLive(float x, float y);

	//fever
	int _scoreWW;
	CharFever _charFever[NUM_CHAR_FEVER];
	float _opacityBGFever;
	bool CheckFullCollectFever();
	int GetIndexRandomFerver();

	void SetCloseFever();
	void AddCharFever(float x, float y, int index);
	void InitCharFever();
	void UpdateCharFever();
	void RenderCharFever(float x, float y);

	//effect fever
	CharFever _charEffectFever[NUM_CHAR_EFFECT_FEVER];
	void InitPosCharFever();
	void InitEffectFever();
	void UpdateEffecetFerver();
	void RenderEffecetFerver(float x, float y);
	bool CheckDuckHadChar(int index);

	//effect score
	INT32 _scoreScroll;
	float _scaleScore;
	int _stateScore;
	void AddScore(int value);
	void UpdateScore();

	bool _isEndGame,_isGameOver , _hasRivive;
	CAnimObject _animTimeRevive;
	void AddRevive(int type);
	void UpdateRevive();

	bool _addSoundScore;
	bool _endFerver;
	float _rotaFever;
	int _scoreFever;
	float _scaleScoreFever, _offScoreFeverX, _offScoreFeverY;
	int _scoreScrollFever;
	int _stateScoreFever;
	void AddScoreFever(int value);
	void UpdateScoreFerver();
	int _numFever;
};
#endif

