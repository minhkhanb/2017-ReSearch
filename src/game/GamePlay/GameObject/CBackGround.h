#ifndef _CBACKGROUND_H
#define _CBACKGROUND_H

#define BACKGROUND_NUM_ROW			10
#define BACKGROUND_NUM_COL			10

#define BACKGROUND_STATE_NULL		0
#define BACKGROUND_STATE_NORMAL		1

#define BACKGROUND_MOVE_TOP				0
#define BACKGROUND_MOVE_LEFT			1
#define BACKGROUND_MOVE_RIGHT			2
#define BACKGROUND_MOVE_DOWN			3
#define BACKGROUND_MOVE_TOP_LEFT		4
#define BACKGROUND_MOVE_TOP_RIGHT		5
#define BACKGROUND_MOVE_DOWN_LEFT		6
#define BACKGROUND_MOVE_DOWN_RIGHT		7

class  CBackGround
{

	float _posX[BACKGROUND_NUM_ROW*BACKGROUND_NUM_COL];
	float _posY[BACKGROUND_NUM_ROW*BACKGROUND_NUM_COL];

	float _w[BACKGROUND_NUM_ROW*BACKGROUND_NUM_COL];
	float _h[BACKGROUND_NUM_ROW*BACKGROUND_NUM_COL];

	int _state[BACKGROUND_NUM_ROW*BACKGROUND_NUM_COL];
	int _spriteID[BACKGROUND_NUM_ROW*BACKGROUND_NUM_COL];
	int _frameID[BACKGROUND_NUM_ROW*BACKGROUND_NUM_COL];

	int _limitW;
	int _limitH;

	float _speed,_offX,_offY;
	int _directionMove;	
	int _numRow, _numCol;
	
	bool _loop;
public:

	CBackGround(){};
	void Init(float x, float y, int numRow, int numCol, int spriteID, int frameID,  int direction, float speed, bool loop, int limitW, int linitH);
	void SetSpriteID(int id);
	void SetSpeed(float speed){ _speed = speed; }
	void UpdateSpeed(int direction, float speed){ _directionMove = direction; _speed = speed; }
	void Render(float x, float y);
	void Update();
};

#endif