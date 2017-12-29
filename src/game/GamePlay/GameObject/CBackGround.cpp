#include "pch.h"
#include "../../GameCore/CGame.h"
#include "../../GameCore/Os_Functions.h"

void CBackGround::Init(float x, float y, int numRow, int numCol, int spriteID, int frameID, int direction, float speed, bool loop, int limitW, int limitH)
{
	_loop = loop;
	_limitW = limitW;
	_limitH = limitH;
	_offY = 0;
	_offX = 0;
	for (int i = 0; i < BACKGROUND_NUM_ROW*BACKGROUND_NUM_COL; i++)
	{
		_state[i] = BACKGROUND_STATE_NULL;
	}

	_numRow = numRow;
	_numCol = numCol;
	
	_speed = speed;
	_directionMove = direction;
	
	int WW = Button::GetFrameWidth(GET_SPRITE(spriteID), frameID);
	int HH = Button::GetFrameHeight(GET_SPRITE(spriteID), frameID);
	for (int i = 0; i < _numRow; i++) //dong
	{
		for (int j = 0; j < _numCol; j++)//cot
		{			
			_w[i * BACKGROUND_NUM_COL + j] = WW;
			_h[i * BACKGROUND_NUM_COL + j] = HH;
	
			_posX[i * BACKGROUND_NUM_COL + j] = x + j*WW;
			_posY[i * BACKGROUND_NUM_COL + j] = y +i*HH;

			_state[i * BACKGROUND_NUM_COL + j] = BACKGROUND_STATE_NORMAL;
			_spriteID[i * BACKGROUND_NUM_COL + j] = spriteID;
			_frameID[i * BACKGROUND_NUM_COL + j] = frameID;
		}
	}

}
void CBackGround::SetSpriteID(int id)
{
	for (int i = 0; i < _numRow; i++) //dong
	{
		for (int j = 0; j < _numCol; j++)//cot
		{			
			_spriteID[i * BACKGROUND_NUM_COL + j] = id;			
		}
	}
}
void CBackGround::Update()
{	
	if (_loop)
	{
		for (int i = 0; i < BACKGROUND_NUM_ROW*BACKGROUND_NUM_COL; i++)
		{
			if (_directionMove == BACKGROUND_MOVE_TOP)
			{
				_posY[i] -= _speed;
				if (_posY[i] + _h[i] <= 0)
				{
					_posY[i] += _h[i] * _numRow;
				}
			}
			else if (_directionMove == BACKGROUND_MOVE_LEFT)
			{
				_posX[i] -= _speed;
				if (_posX[i] + _w[i] <= 0)
				{
					_posX[i] += _w[i] * _numCol;
				}
			}
			else if (_directionMove == BACKGROUND_MOVE_RIGHT)
			{
				_posX[i] += _speed;
				if (_posX[i] >= k_SCREEN_WIDTH)
				{
					_posX[i] -= _w[i] * _numCol;
				}
			}
			else if (_directionMove == BACKGROUND_MOVE_DOWN)
			{
				_posY[i] += _speed;
				if (_posY[i] > k_SCREEN_HEIGHT)
				{
					_posY[i] -= _h[i] * _numRow;
				}
			}
			else if (_directionMove == BACKGROUND_MOVE_TOP_LEFT)
			{
				_posY[i] -= _speed;
				if (_posY[i] + _h[i] <= 0)
				{
					_posY[i] += _h[i] * _numRow;
				}

				_posX[i] -= _speed;
				if (_posX[i] + _w[i] <= 0)
				{
					_posX[i] += _w[i] * _numCol;
				}

			}
			else if (_directionMove == BACKGROUND_MOVE_TOP_RIGHT)
			{
				_posY[i] -= _speed;
				if (_posY[i] + _h[i] <= 0)
				{
					_posY[i] += _h[i] * _numRow;
				}

				_posX[i] += _speed;
				if (_posX[i] >= k_SCREEN_WIDTH)
				{
					_posX[i] -= _w[i] * _numCol;
				}
			}
			else if (_directionMove == BACKGROUND_MOVE_DOWN_LEFT)
			{
				_posY[i] += _speed;
				if (_posY[i] > k_SCREEN_HEIGHT)
				{
					_posY[i] -= _h[i] * _numRow;
				}

				_posX[i] -= _speed;
				if (_posX[i] + _w[i] <= 0)
				{
					_posX[i] += _w[i] * _numCol;
				}
			}
			else if (_directionMove == BACKGROUND_MOVE_DOWN_RIGHT)
			{
				_posY[i] += _speed;
				if (_posY[i] > k_SCREEN_HEIGHT)
				{
					_posY[i] -= _h[i] * _numRow;
				}

				_posX[i] += _speed;
				if (_posX[i] >= k_SCREEN_WIDTH)
				{
					_posX[i] -= _w[i] * _numCol;
				}
			}
		}
	}
	else
	{
		bool move = true;
		if (_directionMove == BACKGROUND_MOVE_TOP)
		{
			
		}
		else if (_directionMove == BACKGROUND_MOVE_LEFT)
		{		
			if (_limitW != 0)
			{
				_offX -= _speed;
				if (_offX < -_limitW)
				{
					_offX = -_limitW;
					move = false;
				}
				else if (_offX > _limitW)
				{
					_offX = _limitW;
					move = false;
				}
			}
		}
		else if (_directionMove == BACKGROUND_MOVE_RIGHT)
		{
			
		}
		else if (_directionMove == BACKGROUND_MOVE_DOWN)
		{
			
		}
		else if (_directionMove == BACKGROUND_MOVE_TOP_LEFT)
		{
		
		}
		else if (_directionMove == BACKGROUND_MOVE_TOP_RIGHT)
		{

		}
		else if (_directionMove == BACKGROUND_MOVE_DOWN_LEFT)
		{
		
		}
		else if (_directionMove == BACKGROUND_MOVE_DOWN_RIGHT)
		{
			
		}
	

		for (int i = 0; i < BACKGROUND_NUM_ROW*BACKGROUND_NUM_COL; i++)
		{
			if (_directionMove == BACKGROUND_MOVE_TOP)
			{				
				
				_posY[i] -= _speed;	
				
			}
			else if (_directionMove == BACKGROUND_MOVE_LEFT)
			{					
				if (move)
				{
					_posX[i] -= _speed;
				}				
			}
			else if (_directionMove == BACKGROUND_MOVE_RIGHT)
			{
				_posX[i] += _speed;				
			}
			else if (_directionMove == BACKGROUND_MOVE_DOWN)
			{
				_posY[i] += _speed;				
			}
			else if (_directionMove == BACKGROUND_MOVE_TOP_LEFT)
			{
				_posY[i] -= _speed;	
				_posX[i] -= _speed;
			}
			else if (_directionMove == BACKGROUND_MOVE_TOP_RIGHT)
			{
				_posY[i] -= _speed;				
				_posX[i] += _speed;				
			}
			else if (_directionMove == BACKGROUND_MOVE_DOWN_LEFT)
			{
				_posY[i] += _speed;				
				_posX[i] -= _speed;				
			}
			else if (_directionMove == BACKGROUND_MOVE_DOWN_RIGHT)
			{
				_posY[i] += _speed;				
				_posX[i] += _speed;				
			}
		}
	}
}

void CBackGround::Render(float x, float y)
{
	for (int i = 0; i < BACKGROUND_NUM_ROW*BACKGROUND_NUM_COL; i++)
	{
		if (_state[i] != BACKGROUND_STATE_NULL)
		{			
			GET_SPRITE(_spriteID[i])->DrawFrame(G(), _frameID[i], x + _posX[i], y + _posY[i]);
		}
	}
}