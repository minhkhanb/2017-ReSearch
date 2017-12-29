#include "pch.h"
#include "../../GameCore/CGame.h"

extern "C" float GetAccelX();
extern "C" float GetAccelY();
extern "C" float GetAccelZ();

void CGamePlay::Init()
{
	EFFECT().Init();
	_bgIngame.Init(0, 0, 1, 1, SPRITEID_BG_INGAME, 0, BACKGROUND_MOVE_LEFT, 0, false,0,0);
	//bg
	_bgLayerMounth1.Init(0, 0, 1, 1, SPRITEID_OBJECTS_MOUNTH, 0, BACKGROUND_MOVE_LEFT, 0, false, 100, 0);
	_bgLayerMounth2.Init(0, 0, 1, 1, SPRITEID_OBJECTS_MOUNTH, 1, BACKGROUND_MOVE_LEFT, 0, false, 100, 0);
	_bgLayerMounth3.Init(0, 0, 1, 1, SPRITEID_OBJECTS_MOUNTH, 2, BACKGROUND_MOVE_LEFT, 0, false, 100, 0);
    
	_bgLayer1.Init(-k_SCREEN_WIDTH / 2, 0, 1, 2, SPRITEID_OBJECTS_PROPS, 0, BACKGROUND_MOVE_LEFT, 0, false, 500, 0);
	_bgLayer2.Init(0, 0, 1, 1, SPRITEID_OBJECTS_PROPS, 1, BACKGROUND_MOVE_LEFT, 0, false, 400, 0);
	_bgLayer3.Init(0, 0, 1, 1, SPRITEID_OBJECTS_PROPS, 2, BACKGROUND_MOVE_LEFT, 0, false, 300, 0);
	_bgLayer4.Init(0, 0, 1, 1, SPRITEID_OBJECTS_PROPS, 3, BACKGROUND_MOVE_LEFT, 0, false, 200, 0);
	_cloud.Init();
	for (int i = 0; i < 10; i++)
	{
		_cloud.AddCloud(100 + (k_SCREEN_WIDTH/5+CMath::RANDOM(-50,50))*i, CMath::RANDOM(100, 250));
	}
	AddSun(k_SCREEN_WIDTH, k_SCREEN_HEIGHT - 55, k_SCREEN_WIDTH - 200, k_SCREEN_HEIGHT / 2 + 200, 0.002);
	_tree.Init(200, 600 + OFFSET_IPAD(128) + OFFSET_I4(70));

	_isPlaying = false;
	_isFerverMode = false;
	_isGameOver = false;
	_hasRivive = false;
	_isEndGame = false;
	_playSoundHighScore = true;
	_isNewScore = false;
}
void CGamePlay::NewGame()
{
	GAME()->StopCurrentMusic();
	GAME()->LoadMusic(BGM_INGAME);
	GAME()->PlayCurrentMusic(-1);
	MAIN_GAME()->_gameStateGamePlay._button[GAMEPLAY_BUTTON_PAUSE].SetState(BUTTON_STATE_NORMAL);

	for (int i = 0; i < MAX_DUCK_IN_GAME; i++)
	{
		_duck[i]._state = CDuck::enumDuckState::DUCK_STATE_NULL;
		_animMiss[i].SetAnimSprite(GET_SPRITE(SPRITEID_HUD_BAR), SPRITEID_HUD_BAR);
		_animShot[i].SetAnimSprite(GET_SPRITE(SPRITEID_CHAR_DUCK_FEVER), SPRITEID_CHAR_DUCK_FEVER);
	}

	for (int i = 0; i < NUM_CHAR_EFFECT_FEVER; i++)
	{
		_charEffectFever[i]._state = 0;
	}
	_stateScoreFever = 0;
#if USE_CONTROL_STICK
	_touchID = -1;
#endif
	_bullseye._x = k_SCREEN_WIDTH / 2;
	_bullseye._y = k_SCREEN_HEIGHT / 2;

	_posDog._x = -200;
	_posDog._y = k_SCREEN_HEIGHT - 55 + OFFSET_IPAD(128) + OFFSET_I4(70);

	_animBullseye.SetAnimSprite(GET_SPRITE(SPRITEID_OBJECTS_BULLSEYE), SPRITEID_OBJECTS_BULLSEYE);
	_animBullseye.SetAnim(BULLSEYE_FIND, true);
	_animBullseye.SetPos(_bullseye._x, _bullseye._y);

	_animGameOver.SetAnimSprite(GET_SPRITE(SPRITEID_HUD_RESULF), SPRITEID_HUD_RESULF);

	_stateBullseye = enumStateBullseye::BULLSEYE_STATE_FIND;

	_isFerverMode = false;
	_duckBeActack = NULL;

	//dog
	_animDog.SetAnimSprite(GET_SPRITE(SPRITEID_CHAR_DOG_01), SPRITEID_CHAR_DOG_01);
	_animDog.SetAnim(0, true);
	_animDog.SetPos(_posDog._x, _posDog._y);
	_speedDog = 5;
	//
	
	_score = 0;
	_scoreScroll = _score;
	_scaleScore = 1;
	_stateScore = 0;


	_level = MAIN_GAME()->_design.GetLevelByScore(_score);
	SetNewValue(_level);
	
	_timeAddDuck = MAIN_GAME()->_design.getLevelDes(_level, LEVEL_VALUE_DUCK_DELAY);
	//AddDuck(CMath::RANDOM(100, k_SCREEN_WIDTH - 100), k_SCREEN_HEIGHT + 100);
	_isShaking = false;
	_startGame = false;
	_stateDog = enumStateDog::DOG_STATE_BEGIN;
	_frame_accellSpeed = 0;
	_live = 3;

	//
	_numCombo = 0;
	_timeCombo = 0;

	
	
	_isGameOver = false;
	_hasRivive = false;
	_isEndGame = false;
	_playSoundHighScore = true;
	_isNewScore = false;

	_percentAddFever = 0;
	InitPosCharFever();

	_offTargetFeverX = 0;
	_offTargetFeverY = 0;

	_indexDuckFear = -1;
	_numFever = 0;

	
	if (CheckFullCollectFever())
	{
		for (int  i = 0; i < NUM_CHAR_FEVER; i++)
		{
			_charFever[i]._state = 6;
		}
		_charFever[0]._state = 7;
		_charFever[0]._pos._x = _charEffectFever[NUM_CHAR_EFFECT_FEVER - 1]._pos._x;
		_charFever[0]._pos._y = _charEffectFever[NUM_CHAR_EFFECT_FEVER - 1]._pos._y;
		InitEffectFever();
	}


	_endFerver = true;

	_offScoreFeverX = k_SCREEN_WIDTH / 2;
	_offScoreFeverY = 150;

	_addSoundScore = false;

	USER()._numPlayADay++;
    USER().DataSave();
}
void CGamePlay::AddRevive(int type)
{
	MAIN_GAME()->_gameStateGamePlay._button[GAMEPLAY_BUTTON_REVIVE].Init(BUTTON_STATE_NORMAL, 0, 0, A_HVCENTER, SFX_BUTTON);
	MAIN_GAME()->_gameStateGamePlay._button[GAMEPLAY_BUTTON_PAUSE].SetState(BUTTON_STATE_HIDE);
	if (type == enumTypeRevive::TYPE_REVIVE_ADS)
	{		
		MAIN_GAME()->_gameStateGamePlay._button[GAMEPLAY_BUTTON_REVIVE].SetAnim(SPRITEID_HUD_RESULF, 6);
	}
	else if (type == enumTypeRevive::TYPE_REVIVE_INVITE)
	{
		MAIN_GAME()->_gameStateGamePlay._button[GAMEPLAY_BUTTON_REVIVE].SetAnim(SPRITEID_HUD_RESULF, 12);
	}
	else if (type == enumTypeRevive::TYPE_REVIVE_LOGIN)
	{
		MAIN_GAME()->_gameStateGamePlay._button[GAMEPLAY_BUTTON_REVIVE].SetAnim(SPRITEID_HUD_RESULF, 16);
	}
	_animTimeRevive.SetAnimSprite(GET_SPRITE(SPRITEID_HUD_RESULF), SPRITEID_HUD_RESULF);
	_animTimeRevive.SetAnim(10, false);
	_animTimeRevive.SetPos(0,0);
	StopMoveBG();
}
void CGamePlay::UpdateRevive()
{
	if (_animTimeRevive.IsCurrentAnimStop())
	{
		_isEndGame = false;
		_animGameOver.SetAnim(0, false);
		_animGameOver.SetPos(0, 0 + OFFSET_IPAD(128) + OFFSET_I4(70));
		_isGameOver = true;
		StopMoveBG();
		if (_isNewScore)
		{
			PLAY_SFX(SFX_ENDGAME_WITH_HIGHSCORE);
			USER()._highScore = _score;
			USER().DataSave();
		}
		else
		{
			PLAY_SFX(SFX_ENDGAME_WITHOUT_HIGHSCORE);
		}
		MAIN_GAME()->_gameStateGamePlay._button[GAMEPLAY_BUTTON_REVIVE].SetState(BUTTON_STATE_HIDE);
	}
}
void CGamePlay::AddDuck()
{
	int index = CMath::RANDOM(0, _numLaneUse - 1);
	int x = CMath::RANDOM(_lane[index]._min, _lane[index]._max);
	int y = k_SCREEN_HEIGHT - 150 + OFFSET_IPAD(128) + OFFSET_I4(70);
	RemoveLaneUse(index);	

	int charFerver = -1;
	int ranPercent = CMath::RANDOM(1, 100);
	//printf("\nranPercent = %d, _percentAddFever = %d", ranPercent,_percentAddFever);
	if (ranPercent <= _percentAddFever)
	{
		charFerver = GetIndexRandomFerver();
		_percentAddFever = 0;
	}

	for (int i = 0; i < MAX_DUCK_IN_GAME; i++)
	{
		if (_duck[i]._state == CDuck::enumDuckState::DUCK_STATE_NULL && CountDuck() < _maxDuck)
		{
			int type = _typeDuckRan[CMath::RANDOM(0, _maxRanDuck - 1)];
			_duck[i].Init(type, charFerver, i, x, y);
			break;
		}
	}
}
void CGamePlay::AddDuck(int type, float x, float y)
{
	for (int i = 0; i < MAX_DUCK_IN_GAME; i++)
	{
		if (_duck[i]._state == CDuck::enumDuckState::DUCK_STATE_NULL)
		{			
			_duck[i].Init(type,-1, i, x, y);
			break;
		}
	}
}
void CGamePlay::Render()
{
	G()->SetScale(1, 1 + OFFSET_I4(0.2) + OFFSET_IPAD(0.34f), k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2);
	_bgIngame.Render(0, 0);
	G()->ClearScale();
	RenderSun();
	_bgLayerMounth1.Render(0, 0 + OFFSET_IPAD(100) + OFFSET_I4(55));
	_bgLayerMounth2.Render(0, 0 + OFFSET_IPAD(100) + OFFSET_I4(55));
	_bgLayerMounth3.Render(0, 0 + OFFSET_IPAD(100) + OFFSET_I4(55));
	_tree.Render();
	_cloud.Render();

    if (_isPlaying)
    {
        if (!_isFerverMode)
        {
			RenderLive(0, 0 + OFFSET_IPAD(-120) + OFFSET_I4(-60));
        }
		RenderCharFever(0, 0 + OFFSET_IPAD(-120) + OFFSET_I4(-60));

		if (_isPlaying && _endFerver)
		{
			char stringScore[128];
			sprintf(stringScore, "%d", _score);
			_scoreWW = GET_SPRITE(SPRITEID_FONT_SCORE)->GetAnsiTextWidth(stringScore, 0, strlen(stringScore));
			G()->SetScale(_scaleScore, _scaleScore, 30 + _scoreWW / 2, 70 + OFFSET_IPAD(-120) + OFFSET_I4(-60));
			GET_SPRITE(SPRITEID_FONT_SCORE)->DrawAnsiTextFormat(G(), 30, 70 + OFFSET_IPAD(-120) + OFFSET_I4(-60), _LEFT | _VCENTER, "%d", _scoreScroll);
			G()->ClearScale();
		}
    }
    
    
	if (_isPlaying)
	{
		for (int i = 0; i < MAX_DUCK_IN_GAME; i++)
		{
			_duck[i].Render();
		}

		if (_isGameOver || _isEndGame)
		{
			G()->SetColor(0xaa000000);
			G()->FillFullScreen(true);
		}

		if (!MAIN_GAME()->IsTutorialComplete() && _stateDog == enumStateDog::DOG_STATE_CHECK_TUTORIAL)
		{
			G()->SetColor(0xaa000000);
			G()->FillFullScreen(true);
			MAIN_GAME()->RenderTutorial();
		}

		if (_isFerverMode)
		{
			G()->SetOpacity(_opacityBGFever);
			G()->SetColor(0xffff0000);
			G()->FillFullScreen(true);
			G()->SetOpacity(100);
			if (_isPlaying)
			{
				for (int i = 0; i < MAX_DUCK_IN_GAME; i++)
				{
					_duck[i].Render(DUCK_TYPE_FEVER);
				}
			}

			for (int i = 0; i < MAX_DUCK_IN_GAME; i++)
			{
				_animShot[i].DrawAnimObject(G());
				_animShot[i].UpdateAnimObject();
			}
		}
		RenderEffecetFerver(0, 0 + OFFSET_IPAD(-75) + OFFSET_I4(-50));
	}
	

	if (_isPlaying && !_isGameOver)
	{
		if (!_isFerverMode)
		{
			for (int i = 0; i < NUM_CHAR_FEVER; i++)
			{
				if (_charFever[i]._state != 0 && !_charFever[i]._isCollected)
				{
					G()->SetScale(_charFever[i]._scale, _charFever[i]._scale, _charFever[i]._pos._x, _charFever[i]._pos._y);
					GET_SPRITE(SPRITEID_HUD_FEVER)->DrawFrame(G(), 20 + i, _charFever[i]._pos._x, _charFever[i]._pos._y);
					G()->ClearScale();
				}
			}
		}
		else
		{
			EFFECT().Render(EFFECT_PAPER_PARTICAL);
		}
	}
	

    _bgLayer1.Render(0, 0 + OFFSET_IPAD(128) + OFFSET_I4(70));
	_bgLayer2.Render(0, 0 + OFFSET_IPAD(128) + OFFSET_I4(70));
	_bgLayer3.Render(0, 0 + OFFSET_IPAD(128) + OFFSET_I4(70));
	_bgLayer4.Render(0, 0 + OFFSET_IPAD(128) + OFFSET_I4(70));
 
	if (_isGameOver)
	{	
		_animGameOver.DrawAnimObject(G());
		_animGameOver.UpdateAnimObject();
	}

	if (_isPlaying && !_isGameOver)
	{
		if (_startGame)
		{
			if (_isFerverMode)
			{
				if (_duckBeActack != NULL && _duckBeActack->_canActack)
				{
					_animBullseye.DrawAnimObject(G());
					_animBullseye.UpdateAnimObject();
				}
			}
			else
			{
				_animBullseye.DrawAnimObject(G());
				_animBullseye.UpdateAnimObject();
			}
		}
		if (CMath::ABSOLUTE_VALUE(_posDog._x - _bullseye._x) > 30 && _animDog.GetCurrentAnim() < 4)
		{
			if (_posDog._x < _bullseye._x)
			{
				if (_stateDog == enumStateDog::DOG_STATE_BEGIN)
				{
					G()->SetScale(1, 1, _posDog._x, _posDog._y);
				}
				else
				{
					G()->SetScale(-1, 1, _posDog._x, _posDog._y);
				}
			}
			else
			{
				if (_stateDog == enumStateDog::DOG_STATE_BEGIN)
				{
					G()->SetScale(-1, 1, _posDog._x, _posDog._y);
				}
				else
				{
					G()->SetScale(1, 1, _posDog._x, _posDog._y);
				}
			}
		}

		_animDog.DrawAnimObject(G());
		G()->ClearScale();
		_animDog.UpdateAnimObject();

		if (!_endFerver)
		{
			char stringScore[128];
			sprintf(stringScore, "%d", _score);
			int ww = GET_SPRITE(SPRITEID_FONT_SCORE)->GetAnsiTextWidth(stringScore, 0, strlen(stringScore));
			G()->SetScale(_scaleScore, _scaleScore, 30 + ww / 2, 70 + OFFSET_IPAD(-120) + OFFSET_I4(-60));
			GET_SPRITE(SPRITEID_FONT_SCORE)->DrawAnsiTextFormat(G(), 30, 70 + OFFSET_IPAD(-120) + OFFSET_I4(-60), _LEFT | _VCENTER, "%d", _scoreScroll);
			G()->ClearScale();
		}

	}
	if (_isPlaying)
	{	

		if ( _stateScoreFever != 0)
		{
			char stringScore[128];
			sprintf(stringScore, "%d", _scoreScrollFever);

			float OffY = _offScoreFeverY + OFFSET_IPAD(-100) + OFFSET_I4(-50);
			if (_stateScoreFever == 13 || _stateScoreFever == 14)
			{
				OffY = _offScoreFeverY + OFFSET_IPAD(-120) + OFFSET_I4(-60);
			}
			G()->SetScale(_scaleScoreFever, _scaleScoreFever, _offScoreFeverX, OffY);
			GET_SPRITE(SPRITEID_FONT_SCORE)->DrawAnsiTextFormat(G(), _offScoreFeverX, OffY, _HCENTER | _VCENTER, stringScore);
			G()->ClearScale();
		}
	}

	G()->SetOpacity(_opacity);
	G()->SetColor(0xff000000);
	G()->FillFullScreen(true);
	G()->SetOpacity(100);

	

	EFFECT().Render();
}
void CGamePlay::RenderTimeBar(float x, float y)
{
	/*GET_SPRITE(SPRITEID_HUD_BAR)->DrawFrame(G(), 0, 0, 0);
	
	float percent = _timePlay / (float)_maxTimePlay;
	int clipX = GET_SPRITE(SPRITEID_HUD_BAR)->GetFModuleX(1, 0);
	int clipY = GET_SPRITE(SPRITEID_HUD_BAR)->GetFModuleY(1, 0);
	int clipW = GET_SPRITE(SPRITEID_HUD_BAR)->GetFModuleWidth(1, 0)*percent;
	int clipH = GET_SPRITE(SPRITEID_HUD_BAR)->GetFModuleHeight(1, 0);

	G()->SetClip(clipX, clipY, clipW, clipH);
	GET_SPRITE(SPRITEID_HUD_BAR)->DrawFrame(G(), 1, 0, 0);
	G()->ClearClip();*/
}
void CGamePlay::DoShaking()
{
    _frame_accellSpeed = 0;
    _isShaking = true;
}
void CGamePlay::UpdateTime()
{
	if (_isFerverMode)
	{		
	}
	else
	{
		_timeCombo -= DT();	
		if (_timeCombo <= 0)
		{
			_numCombo = 0;
			_percentAddFever = 0;
		}
		if (_stateBullseye == enumStateBullseye::BULLSEYE_STATE_AUTO_FIND)
		{
			_stateBullseye = enumStateBullseye::BULLSEYE_STATE_FIND;
		}
	}
	
	if (_score > USER()._highScore && _playSoundHighScore)
	{
		_isNewScore = true;
		PLAY_SFX(SFX_INGAME_HIGHSCRORE);
		_playSoundHighScore = false;
	}
	if (_isGameOver)
	{
		StopMoveBG();	
		if (_animGameOver.IsCurrentAnimStop() )
		{			
			_isPlaying = false;
			MAIN_GAME()->_gameStateGamePlay._stateMenu = new CGameStateMainMenu;
			MAIN_GAME()->_gameStateGamePlay._stateMenu->Init();
			((CGameStateMainMenu*)MAIN_GAME()->_gameStateGamePlay._stateMenu)->InitTransaction(CGameStateMainMenu::enumTypeTransaction::TRANSACTION_COME_BACK_MENU);			
		}

	}
}
void CGamePlay::StopMoveBG()
{
	_tree.SetSpeed(0);
	_bgLayerMounth1.SetSpeed(0);
	_bgLayerMounth2.SetSpeed(0);
	_bgLayerMounth3.SetSpeed(0);
	_bgLayer2.SetSpeed(0);
	_bgLayer3.SetSpeed(0);
	_bgLayer4.SetSpeed(0);
	_bgLayer1.SetSpeed(0);
}
void CGamePlay::UpdateScore()
{	
	if (_scoreScroll >= _score)
	{
		_scoreScroll = _score;
	}
	else
	{
		if (GAME()->_frame_counter % 2 == 0)
		{
			_scoreScroll++;
			if (_scoreScroll % 2 == 0 && _addSoundScore)
			{
				PLAY_SFX(SFX_BONUS_SCORE_AFTER_FEVER);
			}
			if (_scoreScroll >= _score)
			{
				_scoreScroll = _score;
				_addSoundScore = false;
			}
			_stateScore = 1;
			if (_isFerverMode)
			{
				if (_scoreScroll %10 == 0)
				{
					for (int i = 0; i < 15; i++)
					{
						EFFECT().AddEffectPaperPartical(30 + CMath::RANDOM(-100, 100), 120 + CMath::RANDOM(-50, 50), CMath::RANDOM(180, 360), 5);
					}
				}				
			}
		}
	}
	switch (_stateScore)
	{
	case 1:
		_scaleScore += 0.07;
		if (_scaleScore >= 1.2)
		{
			_scaleScore = 1.2;
			_stateScore = 2;
		}
		break;

	case 2:
		_scaleScore -= 0.05;
		if (_scaleScore <= 1)
		{
			_scaleScore = 1;
			_stateScore = 0;
		}
		break;
	}
}
void CGamePlay::UpdateScoreFerver()
{
	if (_stateScoreFever < 10)
	{
		if (_scoreScrollFever >= _scoreFever)
		{
			_scoreScrollFever = _scoreFever;
		}
		else
		{
			if (GAME()->_frame_counter % 4 == 0)
			{
				_scoreScrollFever++;
				if (_scoreScrollFever >= _scoreFever)
				{
					_scoreScrollFever = _scoreFever;
				}
				_stateScoreFever = 1;
			}
		}
	}
	switch (_stateScoreFever)
	{
	case 1:
		_scaleScoreFever += 0.07;
		if (_scaleScoreFever >= 1.2)
		{
			_scaleScoreFever = 1.2;
			_stateScoreFever  = 2;
		}
		break;

	case 2:
		_scaleScoreFever -= 0.05;
		if (_scaleScoreFever <= 1)
		{
			_scaleScoreFever = 1;
			_stateScoreFever = 110;
		}
		break;

	case 10:
		_scaleScoreFever += 0.12;
		if (_scaleScoreFever >= 2.5)
		{
			_stateScoreFever = 11;
		}
		break;

	case 11:
		_scaleScoreFever -= 0.1;
		if (_scaleScoreFever < 1)
		{
			_stateScoreFever = 12;
		}
		break;

	case 12:
		if (CEffectManager::isMoveToTarget(_offScoreFeverX, _offScoreFeverY, 30 + _scoreWW / 2, 70 , 25))
		{
			_offScoreFeverX = 30 + _scoreWW / 2;
			_offScoreFeverY = 70 ;
			_stateScoreFever = 13;
		}
		break;

	case 13:
		_scaleScoreFever += 0.1;
		if (_scaleScoreFever >= 1.5)
		{
			_stateScoreFever = 14;
		}
		break;

	case 14:
		_scaleScoreFever -= 0.07;
		if (_scaleScoreFever < 1)
		{
			_stateScoreFever = 0;
			AddScore(_scoreFever);
			_addSoundScore = true;
		}
		break;
	}
}
void CGamePlay::AddScore(int value)
{
	_score += value;
	_stateScore = 1;
}
void CGamePlay::AddScoreFever(int value)
{
	_scoreFever += value;
	_stateScoreFever = 1;
}

void CGamePlay::Update()
{
	if (_isPlaying)
	{
        Device::DeviceUpdateMotion();
		if (_isFerverMode)
		{
			UpdateDuckFever();
			UpdateEffecetFerver();
			
		}
		else
		{
			UpdateDuck();
			
		}
		UpdateCharFever();
		if (_startGame)
		{		
			if (!_isFerverMode)
			{
				_timeAddDuck -= DT();
				if (_timeAddDuck <= 0)
				{
					_timeAddDuck = MAIN_GAME()->_design.getLevelDes(_level, LEVEL_VALUE_DUCK_DELAY)*(_isFerverMode ? 0.5 : 1);
					AddDuck();
				}
			}
			UpdateTime();
		}
	
		if (_isEndGame)
		{
			UpdateRevive();
		}
	}
	UpdateScore();
	UpdateScoreFerver();
	UpdateSun();
	_bgIngame.Update();
	_bgLayerMounth1.Update();
    _bgLayerMounth2.Update();
    _bgLayerMounth3.Update();
	_bgLayer1.Update();
	_bgLayer2.Update();
	_bgLayer3.Update();
	_bgLayer4.Update();
	_cloud.Update();
	_tree.Update();
	EFFECT().Update();
	//update time Play
     _base_bull_speedX = 0;
     _base_bull_speedY = 0;

	if (_isPlaying && !_isGameOver && !_isEndGame)
	{
#if USE_CONTROL_STICK

#else
		_acclBullseye = 25;

		_base_bull_speedX = _acclBullseye*(Device::CaculateMotionSpeedX())*SENSITIVITY_SPEED + _base_bull_speedX*(1 - SENSITIVITY_SPEED);
		_base_bull_speedY = _acclBullseye*(Device::CaculateMotionSpeedY())*SENSITIVITY_SPEED + _base_bull_speedY*(1 - SENSITIVITY_SPEED);
		if (_base_bull_speedX > 0.2 || _base_bull_speedY > 0.2)
		{
			_base_bull_speedX += 2;
			_base_bull_speedY += 2;
		}

		//printf("\n_frame_accellSpeed = %f",_frame_accellSpeed);
		if (_stateBullseye == enumStateBullseye::BULLSEYE_STATE_FIND && _startGame)
		{
			//check AI duck fear
			int indexDuck = GetIndexDuckFearTarget(_bullseye);
			if (indexDuck != -1)
			{
				_distanceNear = CMath::DISTANCE(_duck[indexDuck]._posDuck._x, _duck[indexDuck]._posDuck._y, _bullseye._x, _bullseye._y);
			}
			else
			{
				_distanceNear = 0;
			}
			_oldDistanceNear = 0;
			_oldPosBullX = _bullseye._x;
			_offPosBullX = 0;

			//update move
			if (isMoveToTarget(_bullseye._x, _bullseye._y, Device::GetCursorViewX(), Device::GetCursorViewY(), 0, 1, _base_bull_speedX, _base_bull_speedY))
			{
				_frame_accellSpeed = 0;
			}
			else
			{
				_frame_accellSpeed += Device::CaculateMotionSpeed();
			}

			//check
			_offPosBullX = _bullseye._x - _oldPosBullX;
			if (_offPosBullX > 0)
			{
				_directionMove = _RIGHT;
			}
			else
			{
				_directionMove = _LEFT;
			}
			if (indexDuck != -1)
			{
				if (_directionMove == _duck[indexDuck]._directionMove)
				{
					_indexDuckFear = indexDuck;
				}
				else
				{
					_oldDistanceNear = CMath::DISTANCE(_duck[indexDuck]._posDuck._x, _duck[indexDuck]._posDuck._y, _bullseye._x, _bullseye._y) - _distanceNear;
					if (_oldDistanceNear < 0)
					{
						_indexDuckFear = indexDuck;
					}
					else
					{
						_indexDuckFear = -1;
					}
				}
			}
			else
			{
				_indexDuckFear = -1;
			}
		}
		else
		{
			_frame_accellSpeed = 0;
		}
#endif
		_animBullseye.SetPos(_bullseye._x, _bullseye._y);
	}
	
	/*bool clear = true;
	for (int i = 0; i < MAX_DUCK_IN_GAME; i++)	
	{
		if (_duck[i].CheckDuckDie(_bullseye))
		{
			clear = false;
		}
	}
	if (clear && _duckBeActack!= NULL)
	{
		_duckBeActack = NULL;
		_stateBullseye = enumStateBullseye::BULLSEYE_STATE_FIND;
	}*/
	if (_startGame)
	{

		if (_bullseye._x < 0)
		{
			_bullseye._x = 0;
		}
		if (_bullseye._x >= k_SCREEN_WIDTH)
		{
			_bullseye._x = k_SCREEN_WIDTH;
		}
		if (_bullseye._y < 0)
		{
			_bullseye._y = 0;
		}
		if (_bullseye._y >= LIMIT_DUCK_H)
		{
			_bullseye._y = LIMIT_DUCK_H;
		}
	}
    if (_stateBullseye !=enumStateBullseye::BULLSEYE_STATE_FOLLOW_TARGET && _stateBullseye != enumStateBullseye::BULLSEYE_STATE_CHECK_SHAK )
    {
		if (MAIN_GAME()->IsTutorialComplete())
		{
			_isShaking = false;
		}
    }
	if (_isPlaying && !_isGameOver && !_isEndGame)
	{
		UpdateBullseys();
		UpdateDog();
	}
}
void CGamePlay::UpdateDuck()
{    
	for (int i = 0; i < MAX_DUCK_IN_GAME; i++)
	{
		if (_duck[i]._state != CDuck::enumDuckState::DUCK_STATE_NULL)
		{
			_duck[i].Update();
		}
	}
	if (_indexDuckFear != -1)
	{
		if (_duck[_indexDuckFear]._state != CDuck::enumDuckState::DUCK_STATE_NULL)
		{		
			if (_stateBullseye == enumStateBullseye::BULLSEYE_STATE_FIND && _startGame)
			{
				if (_duck[_indexDuckFear].CheckDuckDie(_bullseye, 20))
				{
					_duckBeActack = &_duck[_indexDuckFear];
					_timeCheckSnap = 0;
					//_stateBullseye = enumStateBullseye::BULLSEYE_STATE_CHECK_SNAP;
					_animBullseye.SetAnim(BULLSEYE_AUTO_TARGET, false);
					_stateBullseye = enumStateBullseye::BULLSEYE_STATE_FOLLOW_TARGET;
					_stateDog = enumStateDog::DOG_STATE_GUN;
					_animDog.SetAnim(1, true);
					PLAY_SFX(SFX_SNAP);					
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < MAX_DUCK_IN_GAME; i++)
		{
			if (_duck[i]._state != CDuck::enumDuckState::DUCK_STATE_NULL)
			{				
				if (!_duckBeActack && _stateBullseye == enumStateBullseye::BULLSEYE_STATE_FIND && _startGame)
				{
					if (_duck[i].CheckDuckDie(_bullseye, 20))
					{
						_duckBeActack = &_duck[i];
						_timeCheckSnap = 0;
						//_stateBullseye = enumStateBullseye::BULLSEYE_STATE_CHECK_SNAP;
						_animBullseye.SetAnim(BULLSEYE_AUTO_TARGET, false);
						_stateBullseye = enumStateBullseye::BULLSEYE_STATE_FOLLOW_TARGET;
						_stateDog = enumStateDog::DOG_STATE_GUN;
						_animDog.SetAnim(1, true);
						PLAY_SFX(SFX_SNAP);
						break;
					}
				}
			}
		}
	}
}
void CGamePlay::UpdateDuckFever()
{
	for (int i = 0; i < MAX_DUCK_IN_GAME; i++)
	{
		_duck[i].Update();
		if (_duck[i]._state == CDuck::enumDuckState::DUCK_STATE_FLY_FEVER && _startGame)
		{
			if (_duck[i]._canActack)
			{
				if ((_stateBullseye == enumStateBullseye::BULLSEYE_STATE_AUTO_FIND || _stateBullseye == enumStateBullseye::BULLSEYE_STATE_FIND))
				{
					_duckBeActack = &_duck[i];
					_stateBullseye = enumStateBullseye::BULLSEYE_STATE_AUTO_FIND;
					_offTargetFeverX = CMath::RANDOM(-100, 100);
					_offTargetFeverY = CMath::RANDOM(-100, 100);
				}
			}
			else
			{
				_stateBullseye = enumStateBullseye::BULLSEYE_STATE_FIND;
			}
		}
	}
}
void CGamePlay::UpdateBullseys()
{
	if (_duckBeActack != NULL)
	{
		switch (_stateBullseye)
		{
		case enumStateBullseye::BULLSEYE_STATE_CHECK_SNAP:
			_timeCheckSnap+=DT();
			if (_timeCheckSnap > 0)
			{
				if (_duckBeActack->CheckDuckDie(_bullseye, 20))
				{
					_animBullseye.SetAnim(BULLSEYE_AUTO_TARGET, false);
					_stateBullseye = enumStateBullseye::BULLSEYE_STATE_FOLLOW_TARGET;
					_stateDog = enumStateDog::DOG_STATE_GUN;
					_animDog.SetAnim(1, true);
					PLAY_SFX(SFX_SNAP);
				}
				else
				{
					_stateBullseye = enumStateBullseye::BULLSEYE_STATE_FIND;
					_animBullseye.SetAnim(BULLSEYE_FIND, true);
				}
			}		
			
			break;
		case enumStateBullseye::BULLSEYE_STATE_AUTO_FIND:		
			_bullseye._x = _duckBeActack->_posDuck._x + _offTargetFeverX;
			_bullseye._y = _duckBeActack->_posDuck._y + _offTargetFeverY;

			_animBullseye.SetAnim(BULLSEYE_AUTO_TARGET_FEVER, false);
			_animBullseye.SetSubTime(-3);
			_stateBullseye = enumStateBullseye::BULLSEYE_STATE_FOLLOW_TARGET;
			_stateDog = enumStateDog::DOG_STATE_GUN;
			_animDog.SetAnim(1, true);
			PLAY_SFX(SFX_SNAP);			
			break;

		case enumStateBullseye::BULLSEYE_STATE_FOLLOW_TARGET:			
			_bullseye._x = _duckBeActack->_posDuck._x + _offTargetFeverX;
			_bullseye._y = _duckBeActack->_posDuck._y + _offTargetFeverY;
			if (_isFerverMode)
			{
				//if (_animBullseye.IsCurrentAnimStop())
				{
					//if (_animBullseye.GetCurrentAnim() == BULLSEYE_AUTO_TARGET_FEVER)
					{
						_stateBullseye = enumStateBullseye::BULLSEYE_STATE_CHECK_SHAK;
						_animBullseye.SetAnim(BULLSEYE_FOLLOW_TARGET, true);
					}
				}
			}
			else
			{
				if (_animBullseye.IsCurrentAnimStop())
				{
					if (_animBullseye.GetCurrentAnim() == BULLSEYE_AUTO_TARGET)
					{
						_animBullseye.SetAnim(BULLSEYE_FILL_GUN, false);
						_animBullseye.SetSubTime(TIME_FILL_GUN);
						_stateBullseye = enumStateBullseye::BULLSEYE_STATE_CHECK_SHAK;
					}
				}				
			}
			break;

		case enumStateBullseye::BULLSEYE_STATE_CHECK_SHAK:		
			_bullseye._x = _duckBeActack->_posDuck._x + _offTargetFeverX;
			_bullseye._y = _duckBeActack->_posDuck._y + _offTargetFeverY;
			if (_isFerverMode)
			{
				if (_isShaking)
				{
					_stateBullseye = enumStateBullseye::BULLSEYE_STATE_GUN;
					_isShaking = false;
				}
			}
			else
			{			
				if (_animBullseye.GetCurrentAnim() == BULLSEYE_FILL_GUN)
				{
					if (_animBullseye.IsCurrentAnimStop())
					{
						_stateDog = enumStateDog::DOG_STATE_MOVE;
						_stateBullseye = enumStateBullseye::BULLSEYE_STATE_MISS_DUCK;
						_animBullseye.SetAnim(BULLSEYE_FIND, true);
						_frame_miss = 30;						
					}
					else
					{
						if (_isShaking)
						{
							_stateBullseye = enumStateBullseye::BULLSEYE_STATE_GUN;
							_isShaking = false;
						}
					}
				}
			}
			break;

		case enumStateBullseye::BULLSEYE_STATE_MISS_DUCK:
            _frame_miss--;
			if (_frame_miss <= 0 || isMoveToTarget(_bullseye._x, _bullseye._y, Device::GetCursorViewX(), Device::GetCursorViewY(), 0, 1,_base_bull_speedX,_base_bull_speedY))
			{
				_duckBeActack = NULL;
				if (_isFerverMode)
				{
					_stateBullseye = enumStateBullseye::BULLSEYE_STATE_AUTO_FIND;
				}
				else
				{
					_stateBullseye = enumStateBullseye::BULLSEYE_STATE_FIND;
				}
			}
			break;


		case enumStateBullseye::BULLSEYE_STATE_GUN:
		{			
			_bullseye._x = _duckBeActack->_posDuck._x + _offTargetFeverX;
			_bullseye._y = _duckBeActack->_posDuck._y + _offTargetFeverY;
			_cloud.CheckCloudHide(_bullseye);

			
			if (_isFerverMode)
			{
				PLAY_SFX(SFX_FEVER_SHOOT);
				AddScoreFever(1);
				_offTargetFeverX = CMath::RANDOM(-100, 100);
				_offTargetFeverY = CMath::RANDOM(-100, 100);
				_stateBullseye = enumStateBullseye::BULLSEYE_STATE_AUTO_FIND;
				_stateDog = enumStateDog::DOG_STATE_MOVE;

				
				//EFFECT().AddEffectAddScore(_duckBeActack->_posDuck._x, _duckBeActack->_posDuck._y - 80, ,1,3, 3);
				/*if (_score % 10 == 0)
				{
					for (int j = 0; j < 15; j++)
					{
						EFFECT().AddEffectPaperPartical(30 + CMath::RANDOM(-100, 100), 120 + CMath::RANDOM(-50, 50), CMath::RANDOM(180, 360), 5);
					}
				}*/
			}
			else
			{
				PLAY_SFX(SFX_SHOOT);
				AddScore(1);
				_offTargetFeverX = 0;
				_offTargetFeverY = 0;

				_stateBullseye = enumStateBullseye::BULLSEYE_STATE_FIND;
				_stateDog = enumStateDog::DOG_STATE_MOVE;
				
				if (_timeCombo >= 0)
				{
					_numCombo++;
					if (_numCombo >= TOTAL_COMBO_DES_ROW)
					{
						_numCombo = TOTAL_COMBO_DES_ROW;
					}
					_percentAddFever =  MAIN_GAME()->_design.getComboDes(_numCombo - 1, COMBO_DES_WORD_RATIO);
					int bonus = MAIN_GAME()->_design.getComboDes(_numCombo - 1, COMBO_DES_SCORE_BONUS);					
					AddScore(bonus);					
					EFFECT().AddEffectCombo(_duckBeActack->_posDuck._x, _duckBeActack->_posDuck._y + 50, _numCombo, 30);
					_timeCombo = DESIGN_MIN_TIME_KILL_DUCK_TO_COMBO;
					
				}
				else
				{
					_percentAddFever = 0;
					_timeCombo = DESIGN_MIN_TIME_KILL_DUCK_TO_COMBO;
					_numCombo = 0;
				}
			}
			
			CheckNewLVL(_score);
			

			if (CMath::ABSOLUTE_VALUE(_posDog._x - _bullseye._x) > 30 && CMath::RANDOM(0, 100) <= 30 && !_isFerverMode)
			{
				if (_posDog._x < _bullseye._x)
				{
					_animDog.SetAnim(4, false);
				}
				else
				{
					_animDog.SetAnim(5, false);
				}
			}
			else
			{
				_animDog.SetAnim(3, false);
			}
			_animDog.SetSubTime(-2);
			_animBullseye.SetAnim(BULLSEYE_FIND, true);
			if (!_isFerverMode)
			{
				_duckBeActack->SetState(CDuck::enumDuckState::DUCK_STATE_DIE);
				int index = GetIndexDuckNearBullseye(_bullseye, CDuck::enumDuckState::DUCK_STATE_FLY, 200);
				if (index >= 0 && !_isFerverMode)
				{
					_duck[index].SetState(CDuck::enumDuckState::DUCK_STATE_FEAR);
				}
			}
			else
			{
				if (_duckBeActack->_animDuck.GetCurrentAnim() == 0)
				{
					_duckBeActack->_animDuck.SetAnim(1, false);
				}
			}
			
			EFFECT().AddEffectBullFall(_duckBeActack->_posDuck._x, _duckBeActack->_posDuck._y, 5);
			_duckBeActack = NULL;
		}
			break;
		}
	}

}
void CGamePlay::SetAllDuckState(int state)
{
	for (int i = 0; i < MAX_DUCK_IN_GAME; i++)
	{
		if (_duck[i]._state != CDuck::enumDuckState::DUCK_STATE_NULL)
		{
			_duck[i].SetState(state);
		}
	}
}
void CGamePlay::ClearDuckFear(int state)
{
	for (int i = 0; i < MAX_DUCK_IN_GAME; i++)
	{
		if (_duck[i]._state == CDuck::enumDuckState::DUCK_STATE_FEAR)
		{
			_duck[i].SetState(state);
		}
	}
}
int CGamePlay::CountDuck()
{
	int res = 0;
	for (int i = 0; i < MAX_DUCK_IN_GAME; i++)
	{
		if (_duck[i]._state == CDuck::enumDuckState::DUCK_STATE_FEAR || _duck[i]._state == CDuck::enumDuckState::DUCK_STATE_FLY)
		{
			res++;
		}
	}
	return res;
}
int CGamePlay::GetIndexDuckNearBullseye(Position bullseye, int stateDuck, int limit)
{
	float minDistance = k_SCREEN_WIDTH*k_SCREEN_HEIGHT;
	int indexActack = -1;
	for (int i = 0; i < MAX_DUCK_IN_GAME; i++)
	{		
		if (_duck[i]._state == stateDuck && _startGame)
		{			
			GameRect rect(limit, limit, k_SCREEN_WIDTH - limit * 2, k_SCREEN_HEIGHT - limit * 2);
			if (_duck[i].CheckDuckInRect(rect))
			{
				float dd = CMath::DISTANCE(bullseye._x, bullseye._y, _duck[i]._posDuck._x, _duck[i]._posDuck._y);
				if (dd < minDistance && dd <= _dangerousSpace)
				{
					indexActack = i;
					minDistance = dd;
				}
			}
		}
	}
	return indexActack;
}
int CGamePlay::GetIndexDuckFearTarget(Position bullseye)
{
	int limit = 0;
	float minDistance = k_SCREEN_WIDTH*k_SCREEN_HEIGHT;
	int indexActack = -1;
	for (int i = 0; i < MAX_DUCK_IN_GAME; i++)
	{
		if (_duck[i]._state == CDuck::enumDuckState::DUCK_STATE_FEAR && _startGame)
		{
			GameRect rect(limit, limit, k_SCREEN_WIDTH - limit * 2, k_SCREEN_HEIGHT - limit * 2);
			if (_duck[i].CheckDuckInRect(rect))
			{
				float dd = CMath::DISTANCE(_duck[i]._targetMove._x, _duck[i]._targetMove._y, _duck[i]._posDuck._x, _duck[i]._posDuck._y);
				if (dd < minDistance )
				{
					indexActack = i;
					minDistance = dd;
				}
			}
		}
	}
	return indexActack;
}
void CGamePlay::UpdateDog()
{
	_animDog.SetPos(_posDog._x, _posDog._y);
	switch (_stateDog)
	{
	case enumStateDog::DOG_STATE_BEGIN:
		_posDog._x += _speedDog;
            _speedDog*=1.025;
		if (_posDog._x >= k_SCREEN_WIDTH/2)
		{
            PLAY_SFX(SFX_READY);

			if (!MAIN_GAME()->IsTutorialComplete())
			{				
				MAIN_GAME()->InitTutorial();
				_stateDog = enumStateDog::DOG_STATE_CHECK_TUTORIAL;
				_animDog.SetAnim(2, true);
				_animDog.SetSubTime(10);
			}
			else
			{
				_startGame = true;
				_stateDog = enumStateDog::DOG_STATE_MOVE;
				_animDog.SetAnim(2, true);
				AddDuck();
				float w = k_SCREEN_WIDTH;
				float h = k_SCREEN_HEIGHT;
				Device::InitDeviceMotion(w / 2, h / 2, w, h);
				Device::SetCurrentBalance();
			}
			
		}
		break;

	case enumStateDog::DOG_STATE_CHECK_TUTORIAL:
		if (MAIN_GAME()->_stepTutorial == 1)
		{
			if (MAIN_GAME()->_countTimeDeplay <= 0 && MAIN_GAME()->_animTutorial.IsCurrentAnimStop())
			{
				MAIN_GAME()->_stepTutorial = MAX_STEP_TUTORIAL;
				_startGame = true;
				_stateDog = enumStateDog::DOG_STATE_MOVE;
				_animDog.SetAnim(2, true);
				AddDuck();
				float w = k_SCREEN_WIDTH;
				float h = k_SCREEN_HEIGHT;
				Device::InitDeviceMotion(w / 2, h / 2, w, h);
				Device::SetCurrentBalance();
				USER().DataSave();
			}
			else
			{
				_isShaking = false;
			}
		}
		break;
	case enumStateDog::DOG_STATE_MOVE:
#if USE_CONTROL_STICK
		if (_animDog.GetCurrentAnim() >= 3)
		{
			if (_animDog.IsCurrentAnimStop())
			{				
				_animDog.SetAnim(1, true);
                PLAY_SFX(SFX_RELOAD);
			}
		}
		else
		{
			float oldX = _posDog._x;
			float offX = 0;
			if (isMoveToTarget(_posDog._x, _posDog._y, _bullseye._x, _posDog._y, 2, 10))
			{
				if (_animDog.GetCurrentAnim() == 2)
				{
					_animDog.SetAnim(1, true);
				}
			}
			else
			{
				if (_animDog.GetCurrentAnim() == 1)
				{
					_animDog.SetAnim(2, true);
				}
			}
			offX = _posDog._x - oldX;
			if (CMath::ABSOLUTE_VALUE(offX) > 1)
			{
				_tree.SetSpeed(-offX*0.3);
				_bgLayerMounth1.UpdateSpeed(BACKGROUND_MOVE_LEFT, offX*0.05);
				_bgLayerMounth2.UpdateSpeed(BACKGROUND_MOVE_LEFT, offX*0.1);
				_bgLayerMounth3.UpdateSpeed(BACKGROUND_MOVE_LEFT, offX*0.2);
				_bgLayer2.UpdateSpeed(BACKGROUND_MOVE_LEFT,offX*0.2);
				_bgLayer3.UpdateSpeed(BACKGROUND_MOVE_LEFT,offX*0.3);
				_bgLayer4.UpdateSpeed(BACKGROUND_MOVE_LEFT,offX*0.4);
				_bgLayer1.UpdateSpeed(BACKGROUND_MOVE_LEFT,offX*0.35);
			}
			else
			{
				StopMoveBG();
			}
		}
		
#else 
            //printf("\n _acclBullseye = %f",_acclBullseye);
		if (_animDog.GetCurrentAnim() >= 3)
		{
			if (_animDog.IsCurrentAnimStop())
			{				
				_animDog.SetAnim(1, true);
                PLAY_SFX(SFX_RELOAD);
			}
		}
        else
        {
            float oldX = _posDog._x;
            float offX = 0;
            if (isMoveToTarget(_posDog._x, _posDog._y, _bullseye._x, _posDog._y, _acclBullseye*0.15, 2))
            {
                if (_animDog.GetCurrentAnim() == 2)
                {
                    _animDog.SetAnim(1, true);
                }
            }
            else
            {
                if (_animDog.GetCurrentAnim() == 1)
                {
                    _animDog.SetAnim(2, true);
                }
            }
            offX =_posDog._x - oldX;
            if (CMath::ABSOLUTE_VALUE(offX) > 1)
            {
                _tree.SetSpeed(-offX*0.3);
                _bgLayerMounth1.UpdateSpeed(BACKGROUND_MOVE_LEFT,offX*0.05);
                _bgLayerMounth2.UpdateSpeed(BACKGROUND_MOVE_LEFT,offX*0.1);
                _bgLayerMounth3.UpdateSpeed(BACKGROUND_MOVE_LEFT,offX*0.2);
                _bgLayer2.UpdateSpeed(BACKGROUND_MOVE_LEFT,offX*0.2);
                _bgLayer3.UpdateSpeed(BACKGROUND_MOVE_LEFT,offX*0.3);
                _bgLayer4.UpdateSpeed(BACKGROUND_MOVE_LEFT,offX*0.4);
                _bgLayer1.UpdateSpeed(BACKGROUND_MOVE_LEFT,offX*0.35);
            }
            else
            {
				StopMoveBG();
            }
               
        }
#endif
            break;
        case enumStateDog::DOG_STATE_GUN:
			StopMoveBG();
            break;
	}
    
	if (_startGame)
	{
		if (_posDog._x < LIMIT_DUCK_X)
		{
			_posDog._x = LIMIT_DUCK_X;
		}
		if (_posDog._x > LIMIT_DUCK_W)
		{
			_posDog._x = LIMIT_DUCK_W;
		}
	}
  
}
bool CGamePlay::isMoveToTarget(float &x, float &y, float targetX, float targetY, float speed, float distanceMin,float offspeedx ,float offspeedy)
{
	bool ret = false;
    float dx1 = x-targetX;
    float dy1 = y-targetY;
	
	
    float angle = CMath::ANGLE(targetX, targetY, x, y) + 180;
    float vy = (speed+offspeedy)*MSIN(angle);
    float vx = (speed+offspeedx)*MCOS(angle);
    x += vx;
    y += vy;
    float dx2 = x-targetX;
    float dy2 = y-targetY;
    if(dx1*dx2 <= 0)
    {
        x = targetX;
    }
    if(dy2*dy1 <= 0)
    {
        y = targetY;
    }
    if(x == targetX && y == targetY)
    {
        ret = true;
    }

	
	return ret;
}

///
void CGamePlay::UpdateTouch()
{
	for (int i = CGame::_this->_num_touches - 1; i >= 0; i--)
	{
		if (TOUCHS(i)->_flag)
		{
			if (TOUCHS(i)->_state == TOUCH_DOWN)
			{
				printf("\n touch down");
				for (int j = 0; j < MAX_DUCK_IN_GAME; j++)
				{
					//_duck[j].CheckDuckDie(TOUCHS(i)->x, TOUCHS(i)->y);
				}
				TOUCHS(i)->_state = TOUCH_UP;
			}
			if (TOUCHS(i)->_state == TOUCH_UP)
			{
				CGame::_this->ClearAllTouch();
			}
		}
	}

}

//design
void CGamePlay::CheckNewLVL(INT32 score)
{
	int lvl = MAIN_GAME()->_design.GetLevelByScore(score);
	if (lvl > _level)
	{
		_level = lvl;
		SetNewValue(_level);
	}
}
void CGamePlay::SetNewValue(int lvl)
{
	////design
	//_timePlay = MAIN_GAME()->_design.getLevelDes(lvl, DESIGN_VALUE_TIME_TO_DIE) * 1000;
	//_maxTimePlay = MAIN_GAME()->_design.getLevelDes(lvl, DESIGN_VALUE_MAX_TIME) * 1000;
	//_timeBonus = MAIN_GAME()->_design.getLevelDes(lvl, DESIGN_VALUE_SCORE_BONUS) * 1000;
	//_timeSub = MAIN_GAME()->_design.getLevelDes(lvl, DESIGN_VALUE_TIME_MINUS) * 1000;
	//
	_dangerousSpace = MAIN_GAME()->_design.getLevelDes(lvl, LEVEL_VALUE_DANGEROUS_SPACE);
	_maxDuck = MAIN_GAME()->_design.getLevelDes(lvl,LEVEL_VALUE_DUCK_DISPLAY);
	//_timeTargetFever = MAIN_GAME()->_design.getLevelDes(lvl, DESIGN_VALUE_FEVER_MODE) * 1000;
	//_timeFeverCountDown = MAIN_GAME()->_design.getLevelDes(lvl, DESIGN_VALUE_FEVER_TIME) * 1000;
	//_bonusScore = MAIN_GAME()->_design.getLevelDes(lvl, DESIGN_VALUE_FEVER__X__SCORE);
	GetTypeDuckAdd(lvl);
	InitLaneDuck(lvl);
}
void CGamePlay::GetTypeDuckAdd(int lvl)
{
	char str[128];
	sprintf(str, "%s", MAIN_GAME()->_design.getDuckType(lvl));
	int lengh = strlen(str);
	_maxRanDuck = 0;
	char string[5];
	int count = 0;
	for (int i = 0; i < lengh; i++)
	{
		if (str[i] == ' ')
		{
			continue;
		}
		else if (str[i] != ',')
		{
			string[count] = str[i];
			count++;
		}
		else
		{
			string[count] = '\0';
			_typeDuckRan[_maxRanDuck] = atoi(string);
			_maxRanDuck++;
			count = 0;
		}
		if (i == lengh - 1)
		{
			string[count] = '\0';
			_typeDuckRan[_maxRanDuck] = atoi(string);
			_maxRanDuck++;
			count = 0;
		}
	}
}

void CGamePlay::InitLaneDuck(int lvl)
{
	int ww = k_SCREEN_WIDTH / 4.0f;
	int Begin = 0;
	for (int i = 0; i < 4; i++)
	{
		_lane[i]._isUsed = false;
		_lane[i]._min = Begin;
		_lane[i]._max = Begin + ww;
		Begin += ww;
	}
	
	if (MAIN_GAME()->_design.getLevelDes(lvl, LEVEL_VALUE_LANE_1) == 1)
	{
		_lane[0]._isUsed = true;
	}

	if (MAIN_GAME()->_design.getLevelDes(lvl, LEVEL_VALUE_LANE_2) == 1)
	{
		_lane[1]._isUsed = true;
	}

	if (MAIN_GAME()->_design.getLevelDes(lvl, LEVEL_VALUE_LANE_3) == 1)
	{
		_lane[2]._isUsed = true;
	}

	if (MAIN_GAME()->_design.getLevelDes(lvl, LEVEL_VALUE_LANE_4) == 1)
	{
		_lane[3]._isUsed = true;
	}

	_numLaneUse = 0;
	for (int i = 0; i < 4; i++)
	{
		if (_lane[i]._isUsed)
		{
			LaneInitDuck temp = _lane[i];
			_lane[i] = _lane[_numLaneUse];
			_lane[_numLaneUse] = temp;		
			_numLaneUse++;
		}
	}	
}
void CGamePlay::RemoveLaneUse(int index)
{
	for (int i = 0; i<_numLaneUse; i++)
	{
		if (i == index)
		{
			for (int j = i; j < _numLaneUse - 1; j++)
			{
				_lane[j] = _lane[j + 1];
			}	
			_numLaneUse--;
			break;
		}
	}
	if (_numLaneUse <= 0)
	{
		InitLaneDuck(_level);
	}	
}

#if USE_CONTROL_STICK
void CGamePlay::UpdateTouchControl()
{
	if (_stateBullseye != enumStateBullseye::BULLSEYE_STATE_FIND && !_startGame)
	{
		return;
	}
	for (int i = 0; i < CGame::_this->_num_touches; i++)
	{
		if (TOUCHS(i)->_flag)
		{
			if (TOUCHS(i)->_state == TOUCH_DOWN)
			{
				if (_touchID == -1)
				{
					_touchID = TOUCHS(i)->_id;

					_touch_drag_x = TOUCHS(i)->x;
					_touch_drag_y = TOUCHS(i)->y;

					_stickX = _touch_drag_x;
					_stickY = _touch_drag_y;

					_stickCenterX = _touch_drag_x;
					_stickCenterY = _touch_drag_y;

					_stickRenderX = _touch_drag_x;
					_stickRenderY = _touch_drag_y;

					if (_stickRenderX < 100)
					{
						_stickRenderX = 100;
					}

					if (_stickRenderX > k_SCREEN_WIDTH - 100)
					{
						_stickRenderX = k_SCREEN_WIDTH - 100;
					}

					if (_stickRenderY < 100)
					{
						_stickRenderY = 100;
					}

					if (_stickRenderY > k_SCREEN_HEIGHT - 100)
					{
						_stickRenderY = k_SCREEN_HEIGHT - 100;
					}

					_stickCenterRenderX = _stickRenderX;
					_stickCenterRenderY = _stickRenderY;

					_isRenderStick = true;
				}
				else
				{
					if (_stateBullseye == enumStateBullseye::BULLSEYE_STATE_CHECK_SHAK)
					{
						DoShaking();
					}
				}
			}
			else if (TOUCHS(i)->_state == TOUCH_DRAG)
			{
				if (_isRenderStick && _touchID == TOUCHS(i)->_id)
				{
					float dd = CMath::DISTANCE(_touch_drag_x, _touch_drag_y, _stickX, _stickY);	
					
					
					_acclBullseye = TOUCHS(i)->x - _touch_drag_x;
					_touch_drag_x = TOUCHS(i)->x;
					_touch_drag_y = TOUCHS(i)->y;


					
					float angle = CMath::ANGLE(_stickX, _stickY, TOUCHS(i)->x, TOUCHS(i)->y) + 180;

					if (dd > 30)
					{
						_angleMoveBullseys = CMath::ANGLE(_stickX, _stickY, TOUCHS(i)->x, TOUCHS(i)->y) + 180;
					}

					_stickCenterX = _stickX + dd * CMath::COS(angle + 180);
					_stickCenterY = _stickY + dd * CMath::SIN(angle + 180);

					_stickCenterRenderX = _stickRenderX + dd * CMath::COS(angle + 180);
					_stickCenterRenderY = _stickRenderY + dd * CMath::SIN(angle + 180);

					if (dd > 50)
					{
						_stickCenterX = _stickX + 50 * CMath::COS(angle + 180);
						_stickCenterY = _stickY + 50 * CMath::SIN(angle + 180);

						_stickCenterRenderX = _stickRenderX + 50 * CMath::COS(angle + 180);
						_stickCenterRenderY = _stickRenderY + 50 * CMath::SIN(angle + 180);
					}

					if (dd > 65)
					{
						_stickX = _touch_drag_x + 65 * CMath::COS(angle);
						_stickY = _touch_drag_y + 65 * CMath::SIN(angle);
						dd = 65;
					}
					_speedMove = 5;
					float acell = ((dd) / (30))*1.0f;

					//check AI duck fear
					int indexDuck = GetIndexDuckFearTarget(_bullseye);				
					if (indexDuck != -1)
					{
						_distanceNear = CMath::DISTANCE(_duck[indexDuck]._posDuck._x, _duck[indexDuck]._posDuck._y, _bullseye._x, _bullseye._y);
					}
					else
					{
						_distanceNear = 0;
					}
					_oldDistanceNear = 0;

					
					_oldPosBullX = _bullseye._x;
					_offPosBullX = 0;

					_bullseye._x -= _speedMove*acell*CMath::COS(angle);
					_bullseye._y -= _speedMove*acell*CMath::SIN(angle);

					_offPosBullX = _bullseye._x - _oldPosBullX;
					if (_offPosBullX > 0)
					{
						_directionMove = _RIGHT;
					}
					else
					{
						_directionMove = _LEFT;
					}
					if (indexDuck!= -1)
					{
						if (_directionMove == _duck[indexDuck]._directionMove)
						{
							_indexDuckFear = indexDuck;
						}
						else
						{
							_oldDistanceNear = CMath::DISTANCE(_duck[indexDuck]._posDuck._x, _duck[indexDuck]._posDuck._y, _bullseye._x, _bullseye._y) - _distanceNear;
							if (_oldDistanceNear < 0)
							{
								_indexDuckFear = indexDuck;
							}
							else
							{								
								_indexDuckFear = -1;
							}
						}	
					}
					else
					{
						_indexDuckFear = -1;
					}
					
				}
			}
			else if (TOUCHS(i)->_state == TOUCH_UP)
			{
				_touchID = -1;
				GAME()->ClearAllTouch();
			}
		}
	}
}
#endif

void CGamePlay::AddSun(float x, float y, int dx, int dy, float speed)
{
	_animSun.SetAnimSprite(GET_SPRITE(SPRITEID_OBJECTS_SUN), SPRITEID_OBJECTS_SUN);
	_animSun.SetAnim(0, true);
	_animSun.SetPos(k_SCREEN_WIDTH - 200, 150);
	_rotaSun = 0;


	_elipCenter._x = x;
	_elipCenter._y = y;
	_speedSun = 0.0015;

	_elipDX = dx;
	_elipDY = dy;

	_elipAngle = -PI / 2 + _speedSun * 100;
			
	_posSun._x = _elipCenter._x + _elipDX * cos(_elipAngle);
	_posSun._y = _elipCenter._y + _elipDY * sin(_elipAngle);

	_stateEffecetSun = 0;
	_offsetSunY = 0;
	_timeSun = 0;
	_maxDistance = CMath::DISTANCE(_elipCenter._x, _elipCenter._y, _elipCenter._x, _posSun._y);
	_stateOpa = 0;
}
void CGamePlay::UpdateSun()
{
	_posSun._x = _elipCenter._x + _elipDX * cos(_elipAngle);
	_posSun._y = _elipCenter._y + _elipDY * sin(_elipAngle) + _offsetSunY;
	
	
	

	switch (_stateEffecetSun)
	{
	case 0:
		_offsetSunY -= 0.6;
		if (_offsetSunY <= -9)
		{
			_offsetSunY = -9;
			_stateEffecetSun = 1;
		}
		break;

	case 1:
		_offsetSunY += 0.6;
		if (_offsetSunY >=0)
		{
			_offsetSunY = 0;
			_stateEffecetSun = 0;
		}
		break;
	}
	

	if (_stateOpa == 0)
	{
		_elipAngle -= _speedSun;
		float dd = CMath::DISTANCE(_elipCenter._x, _elipCenter._y, _elipCenter._x, _posSun._y);
		if (dd > _maxDistance)
		{
			dd = _maxDistance;
		}
		_scaleSun = dd / _maxDistance;
		_opacity = 100 - 100 * _scaleSun;
		if (_opacity >= 40)
		{
			_opacity = 40;
			_stateOpa = 1;
			_timeSun = 0;
		}
	}
	else if(_stateOpa == 1)
	{
		_elipAngle -= _speedSun;
		_timeSun++;
		if (_timeSun >= 180)
		{
			_stateOpa = 2;
		}
	}
	else if(_stateOpa == 2)
	{
		_opacity -= _speedSun*100;
		if (_opacity <= 0) 
		{
			_stateOpa = 0;
			_elipAngle = -PI / 2 + _speedSun * 100;
		}
	}

	{
		_opacity = 0;
	}

}
void CGamePlay::RenderSun()
{
	_animSun.SetPos(_posSun._x, _posSun._y);

	_rotaSun += 0.5;
	if (_rotaSun > 360)
	{
		_rotaSun -= 360;
	}

	G()->SetScale(_scaleSun, _scaleSun, _posSun._x, _posSun._y);
	G()->SetRotate(_rotaSun, _animSun._x, _animSun._y);
	GET_SPRITE(SPRITEID_OBJECTS_SUN)->DrawFrame(G(), 7, _animSun._x, _animSun._y);
	G()->ClearRotate();

	_animSun.DrawAnimObject(G());
	_animSun.UpdateAnimObject();
	G()->ClearScale();
}

void CGamePlay::RenderLive(float x, float y)
{
	GET_SPRITE(SPRITEID_HUD_BAR)->DrawFrame(G(), 20,x,y);
	for (int i = 0; i < 3; i++)
	{
		int xx = GET_SPRITE(SPRITEID_HUD_BAR)->GetFModuleX(20, i) + x;
		int yy = GET_SPRITE(SPRITEID_HUD_BAR)->GetFModuleY(20, i)+y;
		if (i < _live)
		{
			GET_SPRITE(SPRITEID_HUD_BAR)->DrawFrame(G(), 21, xx, yy);
		}
		else
		{
			GET_SPRITE(SPRITEID_HUD_BAR)->DrawFrame(G(), 22, xx, yy);
		}
	}

	for (int i = 0; i < MAX_DUCK_IN_GAME; i++)
	{		
		_animMiss[i].DrawAnimObject(G());
		_animMiss[i].UpdateAnimObject();
	}

	if (_hasRivive)
	{
		if (_isGameOver)
		{

			GET_SPRITE(SPRITEID_HUD_BAR)->DrawFrame(G(), 40, x, y);
		}
		else
		{

			GET_SPRITE(SPRITEID_HUD_BAR)->DrawFrame(G(), 39, x, y);
		}
	}
}
void CGamePlay::AddAnimMiss(float x, float y)
{
	int limit = 30;
	if (x < limit)
	{
		x = limit;
	}
	if (x > k_SCREEN_WIDTH - limit)
	{
		x = k_SCREEN_WIDTH - limit;
	}
	if (y < limit)
	{
		y = limit;
	}
	if (y > k_SCREEN_HEIGHT - limit)
	{
		y = k_SCREEN_HEIGHT - limit;
	}


	for (int i = 0; i < MAX_DUCK_IN_GAME; i++)
	{
		if (_animMiss[i].IsCurrentAnimStop())
		{
			_animMiss[i].SetPos(x,y);
			_animMiss[i].SetAnim(25,false);
			break;
			
		}
	
	}
}
void CGamePlay::AddAnimShoot(float x, float y)
{	
	for (int i = 0; i < MAX_DUCK_IN_GAME; i++)
	{
		if (_animShot[i].IsCurrentAnimStop())
		{
			_animShot[i].SetPos(x, y);
			_animShot[i].SetAnim(CMath::RANDOM(2,3), false);
			break;
		}
	}
}
////////////
void CGamePlay::InitEffectFever()
{
	GAME()->StopCurrentMusic();
	GAME()->LoadMusic(BGM_FEVER_MODE);
	GAME()->PlayCurrentMusic(-1);
	
	_opacityBGFever = 0;
	_isFerverMode = true;
	_numCombo = 0;
	_percentAddFever = 0;
	_endFerver = true;
	_rotaFever = 0;
	_scoreFever = 0;
	_scoreScrollFever = _scoreFever;
	_scaleScoreFever = 1;
	_stateScoreFever = 0;
	_offScoreFeverX = k_SCREEN_WIDTH / 2;
	_offScoreFeverY = 150;
	
	SetAllDuckState(CDuck::enumDuckState::DUCK_STATE_FLY_OUT);
	AddDuck(DUCK_TYPE_FEVER, -200, k_SCREEN_HEIGHT / 2 + 50 );

	for (int i = 0; i < NUM_CHAR_EFFECT_FEVER; i++)
	{
		_charEffectFever[i]._state = 0;
		_charEffectFever[i]._stateRota = 0;
		_charEffectFever[i]._rota = 0;
		_charEffectFever[i]._speedRota = CMath::RANDOM(50,100)*0.01;

		_charEffectFever[i]._stateOpacity = 0;
		_charEffectFever[i]._opacity = 100;

		_charEffectFever[i]._stateScale = 0;
		_charEffectFever[i]._scale = 1;
		_charEffectFever[i]._speedScale = CMath::RANDOM(200, 500)*0.00001;
		
		_charEffectFever[i]._pos._x = GET_SPRITE(SPRITEID_HUD_FEVER)->GetFModuleCenterX(0, i);
		_charEffectFever[i]._pos._y = GET_SPRITE(SPRITEID_HUD_FEVER)->GetFModuleCenterY(0, i);

		_charEffectFever[i]._timedeplay = CMath::RANDOM(0, 20);
	}
}
void CGamePlay::UpdateEffecetFerver()
{
	for (int i = 0; i < NUM_CHAR_EFFECT_FEVER; i++)
	{
		/*if (_charEffectFever[i]._state == 0 )
		{
			_charEffectFever[i]._timedeplay--;
			if (_charEffectFever[i]._timedeplay <= 0)
			{
				_charEffectFever[i]._state = 1;
				_charEffectFever[i]._stateScale = 1;
				_charEffectFever[i]._stateRota = 1;
			}			
		}*/

		if (GAME()->_frame_counter % (int)(NUM_CHAR_EFFECT_FEVER * 3) == i * 3)
		{
			EFFECT().AddEffectPaperPartical(_charEffectFever[i]._pos._x + CMath::RANDOM(-50, 50), _charEffectFever[i]._pos._y + CMath::RANDOM(-50, 0), CMath::RANDOM(180, 360), 5);
		}

		if (_charEffectFever[i]._state == 1)
		{
			if (i == NUM_CHAR_EFFECT_FEVER - 1)
			{
				_opacityBGFever+= 0.5;
				if (_opacityBGFever >= 60)
				{
					_opacityBGFever = 60;
				}
			}
			switch (_charEffectFever[i]._stateRota)
			{
			case 1:
				_charEffectFever[i]._rota -= _charEffectFever[i]._speedRota;
				if (_charEffectFever[i]._rota <= -10)
				{
					_charEffectFever[i]._stateRota = 2;
				}
				break;

			case 2:
				_charEffectFever[i]._rota += _charEffectFever[i]._speedRota;
				if (_charEffectFever[i]._rota >= 10)
				{
					_charEffectFever[i]._stateRota = 1;
					_charEffectFever[i]._speedRota = CMath::RANDOM(50, 100)*0.01;
				}
				break;
			}


			switch (_charEffectFever[i]._stateScale)
			{
			case 1:
				_charEffectFever[i]._scale += _charEffectFever[i]._speedScale;
				_charEffectFever[i]._opacity -= _charEffectFever[i]._speedScale * 220;
				if (_charEffectFever[i]._opacity <= 0)
				{
					_charEffectFever[i]._stateScale = 1;
					_charEffectFever[i]._scale = 1;
					_charEffectFever[i]._speedScale = CMath::RANDOM(90, 120)*0.00015;
					_charEffectFever[i]._opacity = 130;
				}
				break;
			}

			if (i > 0)
			{
				if (_charEffectFever[i - 1]._state == 3)
				{
					_charEffectFever[i]._state = 2;
					_charEffectFever[i]._opacity = 100;
				}
			}
		}
		else if (_charEffectFever[i]._state == 2)
		{
			switch (_charEffectFever[i]._stateRota)
			{
			case 1:
				_charEffectFever[i]._rota -= _charEffectFever[i]._speedRota;
				if (_charEffectFever[i]._rota <= -10)
				{
					_charEffectFever[i]._stateRota = 2;
				}
				break;

			case 2:
				_charEffectFever[i]._rota += _charEffectFever[i]._speedRota;
				if (_charEffectFever[i]._rota >= 10)
				{
					_charEffectFever[i]._stateRota = 1;
					_charEffectFever[i]._speedRota = CMath::RANDOM(50, 100)*0.01;
				}
				break;
			}
			if (i == NUM_CHAR_EFFECT_FEVER - 1)
			{
				_opacityBGFever -= 5;
			}

			_charEffectFever[i]._opacity -= 10;
			if (_charEffectFever[i]._opacity <= 0)
			{
				if (i == NUM_CHAR_EFFECT_FEVER - 1)
				{
					_isFerverMode = false;
				}
				_charEffectFever[i]._state = 3;
			}
		}
	}
}
void CGamePlay::RenderEffecetFerver(float x, float y)
{
	bool res = false;
	if (_charEffectFever[0]._state == 1 )
	{
		res = true;
	}
	if (_charEffectFever[0]._state > 1 && _charEffectFever[5]._state != 3)
	{
		res = true;
	}

	if(res && _isPlaying && _isFerverMode)
	{
		_rotaFever += 2;
		if (_rotaFever >= 360)
		{
			_rotaFever -= 360;
		}
		G()->SetRotate(_rotaFever, k_SCREEN_WIDTH / 2 +x, 150+y);
		GET_SPRITE(SPRITEID_HUD_RESULF)->DrawFrame(G(), 19, k_SCREEN_WIDTH / 2+x, 150+y);
		G()->ClearRotate();
	}
	
	
	for (int i = 0; i < NUM_CHAR_EFFECT_FEVER; i++)
	{
		if (_charEffectFever[i]._state != 0 && _charEffectFever[i]._state != 3)
		{
			G()->SetRotate(_charEffectFever[i]._rota, _charEffectFever[i]._pos._x+x, _charEffectFever[i]._pos._y+y);
			G()->SetOpacity(_charEffectFever[i]._opacity);
			G()->SetScale(_charEffectFever[i]._scale, _charEffectFever[i]._scale, _charEffectFever[i]._pos._x+x, _charEffectFever[i]._pos._y+y);
			GET_SPRITE(SPRITEID_HUD_FEVER)->DrawFrame(G(), 7 + i, _charEffectFever[i]._pos._x+x, _charEffectFever[i]._pos._y+y);
			G()->ClearScale();
			if (_charEffectFever[i]._state == 1)
			{
				G()->SetOpacity(100);
			}
			GET_SPRITE(SPRITEID_HUD_FEVER)->DrawFrame(G(), 1 + i, _charEffectFever[i]._pos._x+x, _charEffectFever[i]._pos._y+y);
			G()->ClearRotate();
			G()->SetOpacity(100);
		}
	}

	
}
void CGamePlay::SetCloseFever()
{
	_charEffectFever[0]._state = 2;
	_charEffectFever[0]._opacity = 100;
	
	_stateScoreFever = 10;
	_endFerver = false;

	GAME()->StopCurrentMusic();
	GAME()->LoadMusic(BGM_MAINMENU);
	GAME()->PlayCurrentMusic(-1);
	_numFever++;
}
/////////
void CGamePlay::InitCharFever()
{
	for (int i = 0; i < NUM_CHAR_FEVER; i++)
	{
		_charFever[i]._state = 0;
		_charFever[i]._isCollected = false;
	}
	_percentAddFever = 0;

	//InitPosCharFever();
}
void CGamePlay::AddCharFever(float x, float y, int index)
{
	_charFever[index]._pos._x = x;
	_charFever[index]._pos._y = y;
	_charFever[index]._state = 1;
	_charFever[index]._scale = 1;
}
void  CGamePlay::InitPosCharFever()
{
	for (int i = 0; i < NUM_CHAR_FEVER; i++)
	{
		_charFever[i]._pos._x = GET_SPRITE(SPRITEID_HUD_FEVER)->GetFModuleCenterX(19, i);;
		_charFever[i]._pos._y = GET_SPRITE(SPRITEID_HUD_FEVER)->GetFModuleCenterY(19, i) + OFFSET_IPAD(-120) + OFFSET_I4(-60);

		_charFever[i]._scale = 1;
	}
}
void CGamePlay::UpdateCharFever()
{
	for (int i = 0; i < NUM_CHAR_FEVER; i++)
	{
		int xx = GET_SPRITE(SPRITEID_HUD_FEVER)->GetFModuleCenterX(19, i);
		int yy = GET_SPRITE(SPRITEID_HUD_FEVER)->GetFModuleCenterY(19, i) + OFFSET_IPAD(-120) + OFFSET_I4(-60);
		switch (_charFever[i]._state)
		{
		case 1:
			_charFever[i]._scale += 0.1;
			if (_charFever[i]._scale >= 2)
			{
				_charFever[i]._state = 2;
			}
			break;

		case 2:
			_charFever[i]._scale -= 0.085;
			if (_charFever[i]._scale <= 1)
			{
				_charFever[i]._state = 3;
			}
			break;

		case 3:
			if (CEffectManager::isMoveToTarget(_charFever[i]._pos._x, _charFever[i]._pos._y, xx, yy, 20))
			{
				PLAY_SFX(SFX_COLLECT_FEVER_CHARACTER);
				_charFever[i]._state = 4;
				_charFever[i]._pos._x = xx;
				_charFever[i]._pos._y = yy;
				for (int j = 0; j < 15; j++)
				{
					EFFECT().AddEffectPaperPartical(_charFever[i]._pos._x + CMath::RANDOM(-100, 100), _charFever[i]._pos._y + CMath::RANDOM(-50, 50), CMath::RANDOM(180, 360), 5);
				}
			}
			break; 

		case 4:
			_charFever[i]._scale += 0.07;
			if (_charFever[i]._scale >= 1.2)
			{
				_charFever[i]._state = 5;
			}
			break;

		case 5:
			_charFever[i]._scale -= 0.05;
			if (_charFever[i]._scale <= 1)
			{

				_charFever[i]._state = 6;
				_charFever[i]._scale = 1;
				_charFever[i]._isCollected = true;
				USER().DataSave();
				if (CheckFullCollectFever())
				{
					_charFever[0]._state = 7;

					_charFever[0]._pos._x = _charEffectFever[NUM_CHAR_EFFECT_FEVER - 1]._pos._x;
					_charFever[0]._pos._y = _charEffectFever[NUM_CHAR_EFFECT_FEVER - 1]._pos._y + OFFSET_IPAD(-75);
					
					InitEffectFever();
				}
			}
			break;

		case 6:
			if (i >0 && _charFever[i - 1]._state == 8)
			{
				_charFever[i]._state = 7;
				_charFever[i]._pos._x = _charEffectFever[NUM_CHAR_EFFECT_FEVER - 1 - i]._pos._x;
				_charFever[i]._pos._y = _charEffectFever[NUM_CHAR_EFFECT_FEVER - 1 - i]._pos._y + OFFSET_IPAD(-75);
			}
			break;

		case 7:
			_charFever[i]._scale += 0.08;
			if (_charFever[i]._scale >= 2)
			{
				_charFever[i]._state = 8;	
				_charEffectFever[NUM_CHAR_EFFECT_FEVER - 1 - i]._state = 1;
				_charEffectFever[NUM_CHAR_EFFECT_FEVER - 1 - i]._state = 1;
				_charEffectFever[NUM_CHAR_EFFECT_FEVER - 1 - i]._stateScale = 1;
				_charEffectFever[NUM_CHAR_EFFECT_FEVER - 1 - i]._stateRota = 1;
				if (_charEffectFever[1]._state == 1)
				{
					_charEffectFever[0]._state = 1;
					_charEffectFever[0]._stateScale = 1;
					_charEffectFever[0]._stateRota = 1;

					_stateScoreFever = 110;
				}
			}
			break;
		}
	}
}
void CGamePlay::RenderCharFever(float x, float y)
{
	if (!CheckFullCollectFever())
	{
		GET_SPRITE(SPRITEID_HUD_FEVER)->DrawFrame(G(), 19, x, y);
	}	
	EFFECT().Render(EFFECT_PAPER_PARTICAL);
	for (int i = 0; i < NUM_CHAR_FEVER; i++)
	{
		if (_charFever[i]._state != 0 && _charFever[i]._state != 8	&& _charFever[i]._isCollected)
		{
			G()->SetScale(_charFever[i]._scale, _charFever[i]._scale, _charFever[i]._pos._x , _charFever[i]._pos._y);
			GET_SPRITE(SPRITEID_HUD_FEVER)->DrawFrame(G(), 20 + i, _charFever[i]._pos._x, _charFever[i]._pos._y);
			G()->ClearScale();
		}
	}
}
bool CGamePlay::CheckFullCollectFever()
{
	for (int i = 0; i < NUM_CHAR_FEVER; i++)
	{
		if (!_charFever[i]._isCollected)
		{
			return false;
		}
	}
	return true;
}

bool CGamePlay::CheckDuckHadChar(int index)
{
	for (int i = 0; i < MAX_DUCK_IN_GAME; i++)
	{
		if (_duck[i]._state == CDuck::enumDuckState::DUCK_STATE_FLY || _duck[i]._state == CDuck::enumDuckState::DUCK_STATE_FEAR)
		{
			if (_duck[i]._indexCharFever == index)
			{
				return true;
			}
		}
	}
	return false;
}
int CGamePlay::GetIndexRandomFerver()
{
	int res = -1;
	int arrRan[NUM_CHAR_FEVER];
	int count = 0;
	for (int i = 0; i < NUM_CHAR_FEVER; i++)
	{
		if (_charFever[i]._state == 0 && !CheckDuckHadChar(i))
		{
			arrRan[count] = i;
			count++;
		}
	}
	if (count > 0)
	{
		res = arrRan[CMath::RANDOM(0, count - 1)];
	}
	return res;
}