#include "pch.h"
#include "../../GameCore/CGame.h"

void CDuck::Init(int type, int charFever, int index, float x, float y)
{
    
    _fristInit = true;
    _type = type;
	_indexDuck = index;
	_posDuck._x = x;
	_posDuck._y = y;
	
	_spriteID = MAIN_GAME()->_design.getSpriteIDDuck(_type);
	_state = enumDuckState::DUCK_STATE_FLY;
	if (_type == DUCK_TYPE_FEVER)
	{
		_state = enumDuckState::DUCK_STATE_FLY_FEVER;
		_spriteID = SPRITEID_CHAR_DUCK_FEVER;
	
	}
	if (GET_SPRITE(_spriteID) == NULL)
	{
		GAME()->LoadSpriteCurrentVer(_spriteID);
	}	
	_indexCharFever = charFever;
	if (_indexCharFever != -1)
	{
		_animCharFever.SetAnimSprite(GET_SPRITE(MAIN_GAME()->_design._spriteIDFever[_indexCharFever]), MAIN_GAME()->_design._spriteIDFever[_indexCharFever]);
		_animCharFever.SetAnim(ANIM_DUCK_FLY_RIGHT, true);
	}
	
	_animDuck.SetAnimSprite(GET_SPRITE(_spriteID), _spriteID);
	_animDuck.SetAnim(ANIM_DUCK_FLY_RIGHT, true);
	
	

	_sizeDuck = 1;
	_scaleDuckX = 1;
	_scaleDuckY = 1;
	_angleMove = 0;

	SetNewTargetXY();

	//coll
	_numCollision = GET_SPRITE(_spriteID)->GetFModuleNum(0);
	for (int i = 0; i < _numCollision; i++)
	{
		_collide[i]._w = GET_SPRITE(_spriteID)->GetFModuleWidth(0, i);
		_collide[i]._h = GET_SPRITE(_spriteID)->GetFModuleHeight(0, i);
		_collide[i]._x = _posDuck._x + GET_SPRITE(_spriteID)->GetFModuleX(0, i);
		_collide[i]._y = _posDuck._y + GET_SPRITE(_spriteID)->GetFModuleY(0, i);
	}
	

	SMatrix4x4 scaleDuck;
	scaleDuck.IdentifiedPoint();
	scaleDuck = SMatrix4x4::Scale(_scaleDuckX*_sizeDuck, _scaleDuckY*_sizeDuck, _posDuck._x, _posDuck._y);
	for (int i = 0; i < _numCollision; i++)
	{
		GAME()->GetRectTranform(scaleDuck, _collide[i], _collideTranform[i]);
	}

	//design	
	if (_type != DUCK_TYPE_FEVER)
	{

		_speed = MAIN_GAME()->_design.getDuckDes(_type, DUCK_DES_SPEED);
		_currentSpeed = MAIN_GAME()->_design.getDuckDes(_type, DUCK_DES_SPEED);
		_minDisctane = MAIN_GAME()->_design.getDuckDes(_type, DUCK_DES_MIN_DISTANCE);
		_maxDistance = MAIN_GAME()->_design.getDuckDes(_type, DUCK_DES_MAX_DISTANCE);
		_angle = MAIN_GAME()->_design.getDuckDes(_type, DUCK_DES_ANGLE);

		SetSpeedIncrease(MAIN_GAME()->_gamePlay._numFever*DESIGN_ALL_DUCK_INCREASE_SPEED_AFTER_FEVER);
	}
	else
	{
		float dd = _targetMove._x - _posDuck._x;		
		_currentSpeed = dd / (float)((DESIGN_FEVER_TIME + DESIGN_BONUS_TIME_FEVER*MAIN_GAME()->_gamePlay._numFever) / 1000 * 60);
		_speed = _currentSpeed;
		_minDisctane = 0;
		_maxDistance = 0;
		_angle = 0;
	}
	
	_canActack = true;
	
}
void CDuck::SetSpeedIncrease(int percent)
{
	_speed = _currentSpeed + _currentSpeed*percent / 100.0f;
}
void CDuck::SetState(int state)
{	
	switch (state)
	{
	case enumDuckState::DUCK_STATE_FLY_OUT:
		_speed = 25;
		if (_posDuck._x < k_SCREEN_WIDTH / 2)
		{
			_targetMove._x = k_SCREEN_WIDTH;
			_targetMove._y = CMath::RANDOM(0, k_SCREEN_HEIGHT / 2);
		}
		else
		{
			_targetMove._x = 0;
			_targetMove._y = CMath::RANDOM(0, k_SCREEN_HEIGHT / 2);
		}
		if (_targetMove._x > _posDuck._x)
		{
			_directionMove = _RIGHT;
			_framColl = 0;//right
			if (_animDuck.GetCurrentAnim() != ANIM_DUCK_RUN_RIGHT)
			{
				_animDuck.SetAnim(ANIM_DUCK_RUN_RIGHT, true);
				if (_indexCharFever != -1)
				{
					_animCharFever.SetAnim(ANIM_DUCK_RUN_RIGHT, true);
				}
			}
		}
		else
		{
			_directionMove = _LEFT;
			_framColl = 1;//left
			if (_animDuck.GetCurrentAnim() != ANIM_DUCK_RUN_LEFT)
			{
				_animDuck.SetAnim(ANIM_DUCK_RUN_LEFT, true);
				if (_indexCharFever != -1)
				{
					_animCharFever.SetAnim(ANIM_DUCK_RUN_LEFT, true);
				}
			}
		}
		break;
	case enumDuckState::DUCK_STATE_FLY:
		SetSpeedIncrease(MAIN_GAME()->_gamePlay._numFever*DESIGN_ALL_DUCK_INCREASE_SPEED_AFTER_FEVER);
		SetNewTargetXY();
		break;

	case enumDuckState::DUCK_STATE_DIE:		
		_animDuck.SetAnim(ANIM_DUCK_DIE, false);
		if (_indexCharFever != -1)
		{
			MAIN_GAME()->_gamePlay.AddCharFever(_posDuck._x, _posDuck._y + 50, _indexCharFever);
		}
		break;

	case enumDuckState::DUCK_STATE_FEAR:	{
		SetSpeedIncrease(DESIGN_SCARE_DUCK_SPEED_INCREASE + MAIN_GAME()->_gamePlay._numFever*DESIGN_ALL_DUCK_INCREASE_SPEED_AFTER_FEVER);
		printf("\n sppeed = %f",_speed);
		int ran = CMath::RANDOM(1, 3);
		
		if (_posDuck._x < k_SCREEN_WIDTH/2)
		{
			_targetMove._x = k_SCREEN_WIDTH + 150;
			_targetMove._y = CMath::RANDOM(0, k_SCREEN_HEIGHT / 2);
		}
		else
		{
			_targetMove._x = -150;
			_targetMove._y = CMath::RANDOM(0, k_SCREEN_HEIGHT / 2);
		}

		if (_targetMove._x > _posDuck._x)
		{
			_directionMove = _RIGHT;
			_framColl = 0;//right
			if (_animDuck.GetCurrentAnim() != ANIM_DUCK_RUN_RIGHT)
			{
				_animDuck.SetAnim(ANIM_DUCK_RUN_RIGHT, true);
				if (_indexCharFever != -1)
				{
					_animCharFever.SetAnim(ANIM_DUCK_RUN_RIGHT, true);
				}
			}
		}
		else
		{
			_directionMove = _LEFT;
			_framColl = 1;//left
			if (_animDuck.GetCurrentAnim() != ANIM_DUCK_RUN_LEFT)
			{
				_animDuck.SetAnim(ANIM_DUCK_RUN_LEFT, true);
				if (_indexCharFever != -1)
				{
					_animCharFever.SetAnim(ANIM_DUCK_RUN_LEFT, true);
				}
			}
		}
	}
		break;
	}
	_state = state;
}
void CDuck::Render(int typeRender)
{		
	if (_state == enumDuckState::DUCK_STATE_NULL)
	{
		return;
	}

	if (typeRender == _type || (typeRender == -1 && _type != DUCK_TYPE_FEVER))
	{
		_scaleDuckY = 1;
		_scaleDuckX = 1;
		_animDuck.SetPos(_posDuck._x, _posDuck._y);
		if (_indexCharFever !=-1)
		{
			_animCharFever.SetPos(_posDuck._x, _posDuck._y);
		}
		SMatrix4x4 scaleDuck;
		scaleDuck.IdentifiedPoint();
		scaleDuck = SMatrix4x4::Scale(_scaleDuckX*_sizeDuck, _scaleDuckY*_sizeDuck, _posDuck._x, _posDuck._y);

		//G()->SetRotate(_angleMove, _posDuck._x, _posDuck._y);
		G()->SetTransform(scaleDuck.GetParam());
		_animDuck.DrawAnimObject(G());
		if (_indexCharFever != -1 && _state != enumDuckState::DUCK_STATE_DIE)
		{
			_animCharFever.DrawAnimObject(G());
			_animCharFever.UpdateAnimObject();
		}

		G()->ClearTransform();
		G()->ClearRotate();
        
		_animDuck.UpdateAnimObject();
		for (int i = 0; i < _numCollision; i++)
		{
			GAME()->GetRectTranform(scaleDuck, _collide[i], _collideTranform[i]);

			//G()->SetColor(0xaaff0000);
			//G()->FillRect(_collideTranform[i]._x, _collideTranform[i]._y, _collideTranform[i]._w, _collideTranform[i]._h, true);
		}

		//GET_SPRITE(SPRITEID_FONT_BESTSCORE)->DrawAnsiTextFormat(G(),_posDuck._x,_posDuck._y - 70,_HCENTER|_VCENTER,"%d",_indexDuck);
	}
}
void CDuck::Update()
{	
	if (_state == enumDuckState::DUCK_STATE_NULL)
	{
		return;
	}
	if (MAIN_GAME()->_gamePlay._isFerverMode)
	{
		
	}
	else
	{	
		if (_state != enumDuckState::DUCK_STATE_FLY_OUT)
		{
			SetSpeedIncrease(MAIN_GAME()->_gamePlay._numFever*DESIGN_ALL_DUCK_INCREASE_SPEED_AFTER_FEVER);
		}
		
	}
	switch (_state)
	{
	case enumDuckState::DUCK_STATE_FLY_FEVER:
		if (_posDuck._x >= 150 && _posDuck._x < k_SCREEN_WIDTH - 100)
		{
			_canActack = true;
			SetSpeedIncrease(0);
		}
		else
		{
			SetSpeedIncrease(200);
			_canActack = false;
		}
		if (_animDuck.GetCurrentAnim() == 1)
		{
			if (_animDuck.IsCurrentAnimStop())
			{
				_animDuck.SetAnim(ANIM_DUCK_FLY_RIGHT, true);
			}
		}
		if (isMoveToTarget(_posDuck._x, _posDuck._y, _targetMove._x, _targetMove._y, _speed))
		{
			SetState(enumDuckState::DUCK_STATE_NULL);
			MAIN_GAME()->_gamePlay.SetCloseFever();
			//MAIN_GAME()->_gamePlay._isFerverMode = false;
			MAIN_GAME()->_gamePlay._stateBullseye = CGamePlay::enumStateBullseye::BULLSEYE_STATE_FIND;
			MAIN_GAME()->_gamePlay._animBullseye.SetAnim(BULLSEYE_FIND, true);
			MAIN_GAME()->_gamePlay._duckBeActack = NULL;
			MAIN_GAME()->_gamePlay._offTargetFeverX = 0;
			MAIN_GAME()->_gamePlay._offTargetFeverY = 0;
			MAIN_GAME()->_gamePlay.InitCharFever();
            MAIN_GAME()->_gamePlay.InitPosCharFever();
		}
		break;
	case enumDuckState::DUCK_STATE_FLY_OUT:
		if (isMoveToTarget(_posDuck._x, _posDuck._y, _targetMove._x, _targetMove._y, _speed))
		{			
			SetState(enumDuckState::DUCK_STATE_NULL);			
		}
		break;
	case enumDuckState::DUCK_STATE_FLY:
		if (isMoveToTarget(_posDuck._x, _posDuck._y, _targetMove._x, _targetMove._y, _speed))
		{		
			SetNewTargetXY();			
		}
		break;

	case enumDuckState::DUCK_STATE_FEAR:
	{
        bool clean = false;
		if (isMoveToTarget(_posDuck._x, _posDuck._y, _targetMove._x, _targetMove._y, _speed))
		{
            clean = true;
		}
		int limit = -70;
		GameRect rect(limit, limit, k_SCREEN_WIDTH - (limit * 2), k_SCREEN_HEIGHT - (limit * 2));
		if (!CheckDuckInRect(rect))
		{
            clean = true;
		}
		if (clean && !MAIN_GAME()->_gamePlay._isGameOver)
        {
            SetState(enumDuckState::DUCK_STATE_NULL);
            MAIN_GAME()->_gamePlay.AddAnimMiss(_posDuck._x, _posDuck._y);

			if (MAIN_GAME()->_gamePlay._hasRivive)
			{
				MAIN_GAME()->_gamePlay._animGameOver.SetAnim(0, false);
				MAIN_GAME()->_gamePlay._animGameOver.SetPos(0, 0 + OFFSET_IPAD(128) + OFFSET_I4(70));
				MAIN_GAME()->_gamePlay._isGameOver = true;
				MAIN_GAME()->_gamePlay.StopMoveBG();
				MAIN_GAME()->_gameStateGamePlay._button[GAMEPLAY_BUTTON_PAUSE].SetState(BUTTON_STATE_HIDE);
				if (MAIN_GAME()->_gamePlay._isNewScore)
				{
					PLAY_SFX(SFX_ENDGAME_WITH_HIGHSCORE);
					USER()._highScore = MAIN_GAME()->_gamePlay._score;
					USER().DataSave();					
				}
				else
				{
					PLAY_SFX(SFX_ENDGAME_WITHOUT_HIGHSCORE);
				}
			}
			else
			{
				if (MAIN_GAME()->_gamePlay._live > 0)
				{
					MAIN_GAME()->_gamePlay._live--;
					if (MAIN_GAME()->_gamePlay._live <= 0)
					{  
						MAIN_GAME()->_gamePlay._typeRevive = CGamePlay::enumTypeRevive::TYPE_REVIVE_NONE;
						//if (USER()._numPlayADay > 3)
						{
							if (USER()._canInvitFB && USER()._numPlayADay >= 3)
							{
								if (MAIN_GAME()->_stateLoginFB == ShakeDuckOffGame::enumStateLoginFB::LOGIN_FB_NONE)
								{
									MAIN_GAME()->_gamePlay._typeRevive = CGamePlay::enumTypeRevive::TYPE_REVIVE_LOGIN;
								}
								else if(MAIN_GAME()->_stateLoginFB == ShakeDuckOffGame::enumStateLoginFB::LOGIN_FB_COMPLETE)
								{
									MAIN_GAME()->_gamePlay._typeRevive = CGamePlay::enumTypeRevive::TYPE_REVIVE_INVITE;
								}
							}
							else
							{
								int adboxtype = -1;
								if (Device::IsNetworkAvailable())
								{
									adboxtype = PineAdBox::HasVideoAds();
								}
								if (adboxtype >= 0)
								{
									MAIN_GAME()->_gamePlay._typeRevive = CGamePlay::enumTypeRevive::TYPE_REVIVE_ADS;
								}
							}
						}


						if (!MAIN_GAME()->_gamePlay._isNewScore &&  MAIN_GAME()->_gamePlay._typeRevive != CGamePlay::enumTypeRevive::TYPE_REVIVE_NONE)
						{							
							
							MAIN_GAME()->_gamePlay.AddRevive(MAIN_GAME()->_gamePlay._typeRevive);
							MAIN_GAME()->_gamePlay._isEndGame = true;                           
						}
						else
						{
							MAIN_GAME()->_gamePlay._animGameOver.SetAnim(0, false);
							MAIN_GAME()->_gamePlay._animGameOver.SetPos(0, 0 + OFFSET_IPAD(128) + OFFSET_I4(70));
							MAIN_GAME()->_gamePlay._isGameOver = true;
							MAIN_GAME()->_gameStateGamePlay._button[GAMEPLAY_BUTTON_PAUSE].SetState(BUTTON_STATE_HIDE);
							MAIN_GAME()->_gamePlay.StopMoveBG();
							if (MAIN_GAME()->_gamePlay._isNewScore)
							{
								PLAY_SFX(SFX_ENDGAME_WITH_HIGHSCORE);
								USER()._highScore = MAIN_GAME()->_gamePlay._score;
								USER().DataSave();
								
							}
							else
							{
								PLAY_SFX(SFX_ENDGAME_WITHOUT_HIGHSCORE);
							}
						}
					}
				}
			}
        }
	}
		
		break;

	case enumDuckState::DUCK_STATE_DIE:
		if (_animDuck.IsCurrentAnimStop())
		{
			SetState(enumDuckState::DUCK_STATE_NULL);   
		}		
		break;
	}
	for (int i = 0; i < _numCollision; i++)
	{
		_collide[i]._x = _posDuck._x + GET_SPRITE(_spriteID)->GetFModuleX(0, i);
		_collide[i]._y = _posDuck._y + GET_SPRITE(_spriteID)->GetFModuleY(0, i);
	}
	
}
void CDuck::SetNewTargetXY()
{
	if (_fristInit)
	{
		if (_type == DUCK_TYPE_FEVER)
		{
			_targetMove._x = k_SCREEN_WIDTH + 200;
			_targetMove._y = _posDuck._y;
		}
		else
		{

			_targetMove._x = CMath::RANDOM(LIMIT_DUCK_X, LIMIT_DUCK_W);
			_targetMove._y = CMath::RANDOM(LIMIT_DUCK_Y, k_SCREEN_HEIGHT / 2);
		}
		_fristInit = false;
	}
	else
	{
		int distance = CMath::RANDOM(_minDisctane, _maxDistance);
		float bullseyeAngle = CMath::ANGLE(_posDuck._x, _posDuck._y, MAIN_GAME()->_gamePlay._bullseye._x, MAIN_GAME()->_gamePlay._bullseye._y) + 180;

		int aa = CMath::RANDOM(-_angle, _angle);

		_targetMove._x = distance*CMath::COS(bullseyeAngle + aa);
		_targetMove._y = distance*CMath::SIN(bullseyeAngle + aa);
		if (_targetMove._x < LIMIT_DUCK_X || _targetMove._x > LIMIT_DUCK_W)
		{
			_targetMove._x = CMath::RANDOM(LIMIT_DUCK_X, LIMIT_DUCK_W);
		}

		if (_targetMove._y < LIMIT_DUCK_Y || _targetMove._y > LIMIT_DUCK_H)
		{
			_targetMove._y = CMath::RANDOM(LIMIT_DUCK_Y, LIMIT_DUCK_H);
		}
	}

	if (_targetMove._x > _posDuck._x)
	{
		_directionMove = _RIGHT;
		_framColl = 0;//right
		if (_animDuck.GetCurrentAnim() != ANIM_DUCK_FLY_RIGHT)
		{
			_animDuck.SetAnim(ANIM_DUCK_FLY_RIGHT, true);
			if (_indexCharFever != -1)
			{
				_animCharFever.SetAnim(ANIM_DUCK_FLY_RIGHT, true);
			}
		}
	}
	else
	{
		_directionMove = _LEFT;
		_framColl = 1;//left
		if (_animDuck.GetCurrentAnim() != ANIM_DUCK_FLY_LEFT)
		{
			_animDuck.SetAnim(ANIM_DUCK_FLY_LEFT, true);
			if (_indexCharFever != -1)
			{
				_animCharFever.SetAnim(ANIM_DUCK_FLY_LEFT, true);
			}
		}
	}
}
bool CDuck::CheckDuckDie(Position pos, int Rad)
{
	
	if (_state != enumDuckState::DUCK_STATE_FLY && _state != enumDuckState::DUCK_STATE_FEAR)
	{
		return false;
	}
	for (int i = 0; i < _numCollision; i++)
	{
		if (CHECK_RECT_IN_CIRCLE(_collideTranform[i]._x, _collideTranform[i]._y, _collideTranform[i]._w, _collideTranform[i]._h, pos._x, pos._y, Rad))
		{
			return true;
		}
	}	
	return false;
}
bool CDuck::CheckDuckInRect(GameRect rect)
{
	for (int i = 0; i < _numCollision; i++)
	{
		if (CHECK_RECT_IN_RECT(_collideTranform[i]._x, _collideTranform[i]._y, _collideTranform[i]._w, _collideTranform[i]._h, rect._x, rect._y, rect._w, rect._h))
		{
			return true;
		}
	}
	return false;
}
bool CDuck::isMoveToTarget(float &x, float &y, float targetX, float targetY, float speed)
{
	bool ret = false;
	float distance = CMath::DistancePOW2(targetX, targetY, x, y);
	if (distance < (speed * speed))
	{
		targetX = x;
		targetY = y;
		ret = true;
	}
	else
	{
		_angleMove = CMath::ANGLE(targetX, targetY, x, y) + 180;
		float vy = speed*MSIN(_angleMove);
		float vx = speed*MCOS(_angleMove);
		x += vx;
		y += vy;

	}
	return ret;
}

