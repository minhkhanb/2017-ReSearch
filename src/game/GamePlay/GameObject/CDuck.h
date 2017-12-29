#if !defined(CDUCK_H)
#define CDUCK_H

#define LIMIT_DUCK_X			100
#define LIMIT_DUCK_Y			70 + OFFSET_IPAD(-128) + OFFSET_I4(-70)
#define LIMIT_DUCK_W			k_SCREEN_WIDTH - LIMIT_DUCK_X
#define LIMIT_DUCK_H			k_SCREEN_HEIGHT- LIMIT_DUCK_X*2 - 50+ OFFSET_IPAD(128)+ OFFSET_I4(70)

#define NUM_COLLISION			5

#define DUCK_TYPE_FEVER			100

#define ANIM_DUCK_FLY_RIGHT		0
#define ANIM_DUCK_FLY_LEFT		1
#define ANIM_DUCK_RUN_RIGHT		2
#define ANIM_DUCK_RUN_LEFT		3
#define ANIM_DUCK_DIE			4
#define ANIM_DUCK_FALL			5

struct  Position
{
	float _x, _y;
};
class CDuck
{
public:

	int _indexDuck, _directionMove;
	int _numCollision;
	GameRect _collide[NUM_COLLISION], _collideTranform[NUM_COLLISION];
	Position _posDuck, _targetMove;
	int _distanceFear;
	float _speed,_currentSpeed, _sizeDuck, _scaleDuckX, _scaleDuckY, _angleMove;
	int _minDisctane, _maxDistance, _angle;
	bool _fristInit,_canActack;
	int _type,_indexCharFever;
	int _state, _spriteID;
	int _framColl;
	CAnimObject _animDuck;
	CAnimObject _animCharFever;
	enum enumDuckState
	{
		DUCK_STATE_NULL,
		DUCK_STATE_FLY,
		DUCK_STATE_FEAR,
		DUCK_STATE_DIE,
		DUCK_STATE_FLY_OUT,
		DUCK_STATE_FLY_FEVER,
	};

	void SetState(int state);
	void Init(int type, int charFever, int index, float x, float y);
	void Render(int type = -1);
	void Update();

	void SetNewTargetXY();
	bool CheckDuckDie(Position pos, int Rad);	
	bool CheckDuckInRect(GameRect rect);
	bool isMoveToTarget(float &x1, float &y1, float x2, float y2, float speed);
	void SetSpeedIncrease(int percent);
};
#endif

