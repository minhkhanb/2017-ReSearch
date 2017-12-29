#include "pch.h"
#include "../../GameCore/CGame.h"
#include "../../GameCore/Os_Functions.h"

extern "C" void OS_BasicPostSocial(const char* feed);
extern "C" void				GoToReviewInItune();
extern "C" void FacebookInviteSuggetsFriend(const char* feed, const char* title);

#undef _DEC_BUTTON_NUMS
#undef _DEC_BUTTON_ARRAY
#undef _DEC_ANIM_BUTTON_BEIGN
#undef _DEC_SPRITE_BUTTON

#define _DEC_BUTTON_NUMS				(MAINMENU_BUTTON_NUMS)
#define _DEC_BUTTON_ARRAY				_button
#define _DEC_ANIM_BUTTON_BEGIN			4
#define _DEC_SPRITE_BUTTON				SPRITEID_HUD_BAR

void CGameStateMainMenu::Init()
{
	
	for (int i = 0; i < _DEC_BUTTON_NUMS; i++)
	{
        if (i == MAINMENU_BUTTON_PLAY)
        {
            _DEC_BUTTON_ARRAY[i].Init(BUTTON_STATE_NORMAL, 0, 0, A_HVCENTER, SFX_BUTTON,TYPE_EFFECT_MOVE_UP_DOWN);
        }
        else
        {
			_DEC_BUTTON_ARRAY[i].Init(BUTTON_STATE_NORMAL, 0, 0, A_HVCENTER, SFX_BUTTON);
        }
		if (i >= MAINMENU_BUTTON_ABOUT)
		{
			_DEC_BUTTON_ARRAY[i].SetState(BUTTON_STATE_HIDE);
		}
		_DEC_BUTTON_ARRAY[i].SetAnim(_DEC_SPRITE_BUTTON, _DEC_ANIM_BUTTON_BEGIN + 4 * i);
		_animTitle.SetAnimSprite(GET_SPRITE(SPRITEID_HUD_TITTLE), SPRITEID_HUD_TITTLE);
		_animTitle.SetAnim(0, true);
	}
	_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SHARE].Init(BUTTON_STATE_HIDE, 0, 0, A_HVCENTER, -1);
	_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SHARE].SetAnim(SPRITEID_HUD_RESULF, 2);

	_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_ABOUT].SetAnim(_DEC_SPRITE_BUTTON, 26);
	_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_NOTI].SetAnim(_DEC_SPRITE_BUTTON, 30);
	_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_MUSIC].SetAnim(_DEC_SPRITE_BUTTON, 34);
	_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SOUND].SetAnim(_DEC_SPRITE_BUTTON, 38);
	
	_statePoup = NULL;
    
	_stateSetting = 0;
	_stateAbout = 0;
	_isRenderSetting = false;
	_isRenderAbout = false;
	_animText.SetAnimSprite(GET_SPRITE(SPRITEID_HUD_RESULF), SPRITEID_HUD_RESULF);
	_animText.SetAnim(11,true);
	_animText.SetPos(0, 0 + OFFSET_IPAD(128) + OFFSET_I4(70));

	_animDuck.SetAnimSprite(GET_SPRITE(SPRITEID_CHAR_DUCK_FEVER), SPRITEID_CHAR_DUCK_FEVER);
}
void CGameStateMainMenu::InitTransaction(int type)
{
	switch (type)
	{
	case enumTypeTransaction::TRANSACTION_INIT_MAIN_MENU:
		_stateTransaction = 0;
		_offX = 0;
		_offY = -200;
		_offTitleX = 0;
		_offTitleY = -505 + OFFSET_IPAD(-100) + OFFSET_I4(-75);
		_opacity = 0;
		_speed = 5;
		break;

	case enumTypeTransaction::TRANSACTION_GO_TO_GAME:
		_stateTransaction = 1;
		_offX = 0;
		_offY = 0;
		_opacity = 100;
		_speed = 5;
            MAIN_GAME()->_timesCharbost--;
            PineAdBox::SetSkipAdvert();
		break;

	case enumTypeTransaction::TRANSACTION_COME_BACK_MENU:
		GAME()->StopCurrentMusic();
		GAME()->LoadMusic(BGM_MAINMENU);
		GAME()->PlayCurrentMusic(-1);

        USER().DataSave();
		_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SHARE].SetState(BUTTON_STATE_NORMAL);
		_stateTransaction = 0;
		_offX = 0;
		_offY = -200;
		_opacity = 0;
		_speed = 5;
		_rota = 0;
		_opacityGlow = 0;
		_animNew.SetAnimSprite(GET_SPRITE(SPRITEID_HUD_RESULF), SPRITEID_HUD_RESULF);
		_animNew.SetAnim(1, true);
		_animNew.SetPos(0, 0 + OFFSET_IPAD(-75) + OFFSET_I4(-50));
		//	
		//MAIN_GAME()->_gamePlay._score = 59;
		_offScoreX = k_SCREEN_WIDTH / 2;
		_offScoreY = -100 + OFFSET_IPAD(-75) + OFFSET_I4(-50);
		_opacityResult = 0;
		_score = 0;		
		_offScore = MAIN_GAME()->_gamePlay._score/30;
		if (_offScore <= 0)
		{
			_offScore = 1;
		}		

		if (MAIN_GAME()->_gamePlay._isNewScore)
		{
			_scaleHighScore = 0;
			PLAY_SFX(SFX_RESULT_WITH_HIGHSCORE);
            USER()._highScore = MAIN_GAME()->_gamePlay._score;
            PineGameCenterController::SubmitMyScore(GC_SUBMIT_SCORE_ID, USER()._highScore );
		}
		else
		{
			_scaleHighScore = 1;
			PLAY_SFX(SFX_RESULT_WITHOUT_HIGHSCORE);
            
            if(MAIN_GAME()->_timesCharbost <= 0)
            {
                MAIN_GAME()->_timesCharbost = TIMES_CHARBOST;
                PineAdBox::SetReactiveAdvert();
                PineAdBox::ShowAdvertFullScreen();
            }
		}
		USER().DataSave();
		break;
	}
	_typeTransaction = type;
}
void CGameStateMainMenu::Update()
{
	if (MAIN_GAME()->_sub_state == k_LOOP)
	{	
		
		
		
		if (_statePoup != NULL)
		{
			_statePoup->Update();
		}		
		else
		{
			if (!UpdateTouchButton())
			{
			}
		}
		UpdateTransaction();

	}

}
void CGameStateMainMenu::UpdateInitMenu()
{
	switch (_stateTransaction)
	{
	case 0:
		_offY += _speed;
		_speed *= 1.1f;		
		if (_offY >=0 )
		{
			_offY = 0;
			_stateTransaction = 1;
			_speed = 30;
		}		
		break;

	case 1:
		_offY -= 2;
		if (_offY <= -10)
		{
			_stateTransaction = 2;
		}

		_offTitleY += _speed;
		_speed *= 1.02f;
		if (_offTitleY >= 0)
		{
			_offTitleY = 0;			
		}
		break;
	case 2:
		_offY += 1;
		if (_offY >= 0)
		{
			_offY = 0;
			_stateTransaction = 3;			
		}
		_offTitleY += _speed;
		_speed *= 1.02f;
		if (_offTitleY >= 0)
		{
			_offTitleY = 0;
		}
		break;


	case 3:
		_offTitleY += _speed;
		_speed *= 1.02f;
		if (_offTitleY >= 40)
		{
			_offTitleY = 0;
			_stateTransaction = 4;
		}
		break;

	case 4:
		_offTitleY -= 5;
		if (_offTitleY <= -30)
		{
			_stateTransaction = 5;
		}
		break;
	case 5:
		_offTitleY += 3;
		if (_offTitleY >= 0)
		{
			_offTitleY = 0;
			_stateTransaction = END_TRANSACTION;
		}
		break;
	}
	_opacity += 5;
	if (_opacity >= 100)
	{
		_opacity = 100;
	}
}
void CGameStateMainMenu::UpdateGoToGame()
{

	switch (_stateTransaction)
	{
	case 1:
		_opacity -= 5;
		_offY -= _speed;
		_offTitleY -= _speed;
		_speed *= 1.1;
		if (_offY < -k_SCREEN_HEIGHT || _opacity <= 5)
		{
			MAIN_GAME()->_gamePlay._isPlaying = true;
			MAIN_GAME()->_gamePlay.NewGame();
			_stateTransaction = END_TRANSACTION;
		}
		break;
	}
}
void CGameStateMainMenu::UpdateComeBackMenu()
{
	switch (_stateTransaction)
	{
	case 0:
		_offY += _speed;
		_speed *= 1.1f;
		if (_offY >= 0)
		{
			_offY = 0;
			_stateTransaction = 1;
			_speed = 30;
		}
		break;

	case 1:
		_offY -= 2;
		if (_offY <= -10)
		{
			_stateTransaction = 2;
		}
		if (MAIN_GAME()->_gamePlay._isNewScore)
		{
			_scaleHighScore += 0.075;			
		}
		
		break;
	case 2:
		_offY += 1;
		if (_offY >= 0)
		{
			_offY = 0;
			_stateTransaction = 3;
		}
		if (MAIN_GAME()->_gamePlay._isNewScore)
		{
			_scaleHighScore += 0.075;
		}
		break;

	case 3:
		if (MAIN_GAME()->_gamePlay._isNewScore)
		{
			_scaleHighScore += 0.075;
			if (_scaleHighScore >= 1.3)
			{
				_stateTransaction = 4;
			}
		}
		else
		{
			_stateTransaction = 10;
		}
		break;

	case 4:
		_scaleHighScore -= 0.05;
		if (_scaleHighScore <= 1 )
		{
			_scaleHighScore = 1;
			_stateTransaction = 10;
		}
		break;

	case 10:
		_offScoreY += 10;
		if (_offScoreY >= 115 + OFFSET_IPAD(-75) + OFFSET_I4(-50))
		{
			_offScoreY = 115 + OFFSET_IPAD(-75) + OFFSET_I4(-50);
			_stateTransaction = 11;
		}
		break;

	case 11:
		_score += _offScore;
		if (_score >= MAIN_GAME()->_gamePlay._score)
		{
			_score = MAIN_GAME()->_gamePlay._score;			
			_stateTransaction = END_TRANSACTION;
		}
		break;

	}
	_opacity += 5;
	if (_opacity >= 100)
	{
		_opacity = 100;
	}
	if (_stateTransaction > 10)
	{
		_opacityGlow += 2;
		if (_opacityGlow >= 85)
		{
			_opacityGlow = 85;
		}
	}
	if (_stateTransaction > 1)
	{
		_opacityResult += 5;
		if (_opacityResult >= 100)
		{
			_opacityResult = 100;
		}
	}
	_rota++;
}
void CGameStateMainMenu::UpdateTransaction()
{
	switch (_typeTransaction)
	{
	case enumTypeTransaction::TRANSACTION_INIT_MAIN_MENU:
		UpdateInitMenu();
		break;

	case enumTypeTransaction::TRANSACTION_GO_TO_GAME:
		UpdateGoToGame();
		break;

	case enumTypeTransaction::TRANSACTION_COME_BACK_MENU:
		UpdateComeBackMenu();
		break;
	}

	switch (_stateSetting)
	{
	case 1:
		_offsetSettingY -= 25;
		if (_offsetSettingY <= 0)
		{
			_offsetSettingY = 0;
			_stateSetting = 0;
		}
		break;

	case 2:
		_offsetSettingY += 25;
		if (_offsetSettingY >= 280)
		{
			_offsetSettingY = 280;
			_stateSetting = 0;
			CloseSetting();
		}
		break;
	}

	switch (_stateAbout)
	{
	case 1:
		_opacityAboutGB += 0.5;
		if (_opacityAboutGB >= 60)
		{
			_opacityAboutGB = 60;
		}
		_offsetAboutX -= 2;
		if (_offsetAboutX < -4360)
		{
			_offsetAboutX = k_SCREEN_WIDTH + 150;
		}
		break;

	case 2:
		_opacityAboutGB -= 5;
		if (_opacityAboutGB <= 0)
		{
			_opacityAboutGB = 0;
		}
		_opacityAbout -= 5;
		if (_opacityAbout <= 0)
		{
			_opacityAbout = 0;
			_stateAbout = 3;
			
		}
		break;

	case 3:
		_stateAbout = 0;
		_isRenderAbout = false;
		break;
	}
}
void CGameStateMainMenu::Render()
{
	if (MAIN_GAME()->_sub_state == k_INIT)
	{
		Init();
		MAIN_GAME()->_sub_state = k_LOOP;
	}
	if (MAIN_GAME()->_sub_state == k_LOOP)
	{	
		float OffsetButtom = 0 + OFFSET_IPAD(128) + OFFSET_I4(64);
		if (_typeTransaction == enumTypeTransaction::TRANSACTION_INIT_MAIN_MENU)
		{
			G()->SetOpacity(_opacity);
			_animTitle.SetPos(_offTitleX, _offTitleY + OFFSET_IPAD(-25));
			_animTitle.DrawAnimObject(G());
			_animTitle.UpdateAnimObject();
		}
		else if (_typeTransaction == enumTypeTransaction::TRANSACTION_COME_BACK_MENU)
		{

			G()->SetOpacity(_opacityResult);
			_animText.DrawAnimObject(G());
			_animText.UpdateAnimObject();
			RenderResult();
		}

		G()->SetOpacity(_opacity);

		if (_isRenderSetting)
		{
			int clipX = GET_SPRITE(SPRITEID_HUD_BAR)->GetFModuleX(41, 0) - 10;
			int clipY = GET_SPRITE(SPRITEID_HUD_BAR)->GetFModuleY(41, 0) - 10 + OffsetButtom;
			int clipW = GET_SPRITE(SPRITEID_HUD_BAR)->GetFModuleWidth(41, 0) + 20;
			int clipH = GET_SPRITE(SPRITEID_HUD_BAR)->GetFModuleHeight(41, 0) + 20;

			G()->SetClip(clipX, clipY, clipW, clipH);
			GET_SPRITE(SPRITEID_HUD_BAR)->DrawFrame(G(), 41, 0, _offY + _offsetSettingY + OffsetButtom);
			for (int i = 0; i < _DEC_BUTTON_NUMS; i++)
			{
				//shadow			
				if (i >= MAINMENU_BUTTON_ABOUT)
				{
					_DEC_BUTTON_ARRAY[i].SetPos(_offX, _offY + _offsetSettingY + OffsetButtom);
					_DEC_BUTTON_ARRAY[i].Render(G());
					_DEC_BUTTON_ARRAY[i].Update();
				}
			}
			if (!GAME()->_notificationEnable)
			{
				GET_SPRITE(SPRITEID_HUD_BAR)->DrawFrame(G(), 46, 0, _offY + _offsetSettingY + OffsetButtom);
			}
			if (!GAME()->IsEnableMusic())
			{
				GET_SPRITE(SPRITEID_HUD_BAR)->DrawFrame(G(), 49, 0, _offY + _offsetSettingY + OffsetButtom);
			}
			if (!GAME()->IsEnableSound())
			{
				GET_SPRITE(SPRITEID_HUD_BAR)->DrawFrame(G(), 52, 0, _offY + _offsetSettingY + OffsetButtom);
			}
			G()->ClearClip();
		}

		for (int i = 0; i < _DEC_BUTTON_NUMS; i++)
		{
			//shadow			
			if (i < MAINMENU_BUTTON_SHARE)
			{
				_DEC_BUTTON_ARRAY[i].SetPos(_offX, _offY + OffsetButtom);
				float xx = GET_SPRITE(SPRITEID_HUD_BAR)->GetFModuleCenterX(4 + i * 2, 0);
				if (i == MAINMENU_BUTTON_PLAY)
				{
					GET_SPRITE(SPRITEID_HUD_BAR)->DrawFrame(G(), 15, xx + _offX, k_SCREEN_HEIGHT - 60 + OffsetButtom);
				}
				else if (i < MAINMENU_BUTTON_SHARE)
				{
					GET_SPRITE(SPRITEID_HUD_BAR)->DrawFrame(G(), 14, xx + _offX, k_SCREEN_HEIGHT - 60 + OffsetButtom);
				}
				_DEC_BUTTON_ARRAY[i].Render(G());
				_DEC_BUTTON_ARRAY[i].Update();
			}	
		}
		G()->SetOpacity(100);

		if (_isRenderAbout)
		{
			RenderAbout();
		}


		
	}
}
void CGameStateMainMenu::InitSetting()
{
	_isRenderSetting = true;
	_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_ABOUT].SetState(BUTTON_STATE_NORMAL);
	_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_NOTI].SetState(BUTTON_STATE_NORMAL);
	_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_MUSIC].SetState(BUTTON_STATE_NORMAL);
	_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SOUND].SetState(BUTTON_STATE_NORMAL);
	_offsetSettingY = 280;
	_stateSetting = 1;
}
void CGameStateMainMenu::CloseSetting()
{
	_isRenderSetting = false;
	_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_ABOUT].SetState(BUTTON_STATE_HIDE);
	_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_NOTI].SetState(BUTTON_STATE_HIDE);
	_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_MUSIC].SetState(BUTTON_STATE_HIDE);
	_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SOUND].SetState(BUTTON_STATE_NORMAL);
}
void CGameStateMainMenu::RenderSetting()
{}

void CGameStateMainMenu::InitAbout()
{
	_isRenderAbout = true;
	_animDuck.SetAnim(4, true);
	_offsetAboutX = k_SCREEN_WIDTH + 150;
	_stateAbout = 1;
	_opacityAboutGB = 0;
	_opacityAbout = 100;
}
void CGameStateMainMenu::CloseAbout()
{
	_isRenderAbout = false;
	
}
void CGameStateMainMenu::RenderAbout()
{
	G()->SetOpacity(_opacityAboutGB);
	G()->SetColor(0xff000000 );
	G()->FillFullScreen(true);

	G()->SetOpacity(_opacityAbout);
	_animDuck.SetPos(_offsetAboutX, k_SCREEN_HEIGHT / 2 - 58);
	G()->SetScale(-1, 1, _offsetAboutX, k_SCREEN_HEIGHT / 2 - 58);
	_animDuck.DrawAnimObject(G());
	G()->ClearScale();
	_animDuck.UpdateAnimObject();

	GET_SPRITE(SPRITEID_HUD_BAR)->DrawFrame(G(), 53, _offsetAboutX + 135, 0);
}


void CGameStateMainMenu::RenderResult()
{
	GET_SPRITE(SPRITEID_HUD_RESULF)->DrawFrame(G(), 12, 0, 0 + OFFSET_IPAD(-75) + OFFSET_I4(-50));

	G()->SetOpacity(_opacityGlow);
	G()->SetRotate(_rota, _offScoreX, 135 + OFFSET_IPAD(-75) + OFFSET_I4(-50));
	GET_SPRITE(SPRITEID_HUD_RESULF)->DrawFrame(G(), 19, _offScoreX, 135 + OFFSET_IPAD(-75) + OFFSET_I4(-50));
	G()->ClearRotate();
	G()->SetOpacity(_opacityResult);
	GET_SPRITE(SPRITEID_FONT_HIGHSCORE)->DrawAnsiTextFormat(G(), _offScoreX, _offScoreY, _HCENTER | _VCENTER, "%d", _score);

	if (MAIN_GAME()->_gamePlay._isNewScore && _stateTransaction == END_TRANSACTION)
	{
		_animNew.DrawAnimObject(G());
		_animNew.UpdateAnimObject();
	}
	G()->SetScale(_scaleHighScore, _scaleHighScore, k_SCREEN_WIDTH / 2, 300);
	_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SHARE].Render(G());
	_DEC_BUTTON_ARRAY[MAINMENU_BUTTON_SHARE].Update();
	GET_SPRITE(SPRITEID_HUD_RESULF)->DrawFrame(G(),16,0,0);

	GET_SPRITE(SPRITEID_FONT_BESTSCORE)->DrawAnsiTextFormat(G(), k_SCREEN_WIDTH / 2 + 5, 333, _HCENTER | _VCENTER, "%d", USER()._highScore);
    G()->ClearScale();

}

bool CGameStateMainMenu::UpdateTouchButton()
{
	if (_stateTransaction < END_TRANSACTION)
	{
		return false;
	}
	bool ret = false;
	int index = -1;
	for (int i = 0; i < _DEC_BUTTON_NUMS; i++)
	{
		if (_DEC_BUTTON_ARRAY[i]._state == BUTTON_STATE_ACTIVE)
		{
			ret = true;
			index = i;
			break;
		}
	}
	if (ret)
	{
		if (_DEC_BUTTON_ARRAY[index].NeedAction())
		{
			switch (index)
			{
                    
			case MAINMENU_BUTTON_PLAY:
				InitTransaction(enumTypeTransaction::TRANSACTION_GO_TO_GAME);
				break;
			case MAINMENU_BUTTON_STORE:				
				GoToReviewInItune();
				break;

			case MAINMENU_BUTTON_SETTING:
				if (_isRenderSetting)
				{
					_stateSetting = 2;
				}
				else
				{
					InitSetting();
				}
				
				break;
			case MAINMENU_BUTTON_ABOUT:
				if (_isRenderAbout)
				{
					_stateAbout = 2;
				}
				else
				{
					InitAbout();
				}
				break;
                    
			case MAINMENU_BUTTON_SHARE:
			{
				char string[512];
				sprintf(string, "I has just scored %d in Shake Duck Off", _score);
				OS_BasicPostSocial(string);
			}                
                //_statePoup->Init();
                break;
                    
            case MAINMENU_BUTTON_LEADER_BROAD:
                PineGameCenterController::ShowLeaderBoard(GC_SUBMIT_SCORE_ID);
                //_statePoup->Init();
                break;
                    
            case MAINMENU_BUTTON_ACHIEVENTMENT:
                PineAdBox::ShowMoreGame();
                break;


			case MAINMENU_BUTTON_NOTI:				
				GAME()->_notificationEnable = !GAME()->_notificationEnable;
				USER().DataSave();
				break;

			case MAINMENU_BUTTON_MUSIC:
				if (GAME()->IsEnableMusic())
				{
					GAME()->SetMusicEnable(false);
					GAME()->StopCurrentMusic();
				}
				else
				{
					GAME()->SetMusicEnable(true);
					GAME()->StopCurrentMusic();
					GAME()->LoadMusic(BGM_MAINMENU);
					GAME()->PlayCurrentMusic(-1);
				}
				USER().DataSave();				
				break;

			case MAINMENU_BUTTON_SOUND:
				if (GAME()->IsEnableSound())
				{
					GAME()->SetSoundEnable(false);
				}
				else
				{
					GAME()->SetSoundEnable(true);
				}
				USER().DataSave();
				break;
			}

			for (int i = 0; i < _DEC_BUTTON_NUMS; i++)
			{
				if (_DEC_BUTTON_ARRAY[i]._state != BUTTON_STATE_LOCK && _DEC_BUTTON_ARRAY[i]._state != BUTTON_STATE_HIDE)
				{
					_DEC_BUTTON_ARRAY[i].SetState(BUTTON_STATE_NORMAL);
				}
			}
			CGame::_this->ClearAllTouch();
		}
		return true;
	}
	bool pressed = false;
	for (int i = CGame::_this->_num_touches - 1; i >= 0; i--)
	{
		if (TOUCHS(i)->_flag)
		{
			bool hasTouchButton = false;
			for (int touch = 0; touch < _DEC_BUTTON_NUMS; touch++)
			{
				if (_DEC_BUTTON_ARRAY[touch]._state == BUTTON_STATE_LOCK || _DEC_BUTTON_ARRAY[touch]._state == BUTTON_STATE_HIDE)
				{
					continue;
				}
				if (_isRenderAbout)
				{
					continue;
				}
				int x = _DEC_BUTTON_ARRAY[touch].TZ_GetX() - 30;
				int y = _DEC_BUTTON_ARRAY[touch].TZ_GetY() - 30;
				int w = _DEC_BUTTON_ARRAY[touch].TZ_GetWidth() + 60;
				int h = _DEC_BUTTON_ARRAY[touch].TZ_GetHeight() + 60;

				if (touch >= MAINMENU_BUTTON_ABOUT || touch == MAINMENU_BUTTON_SETTING)
				{
					x = _DEC_BUTTON_ARRAY[touch].TZ_GetX() - 5;
					y = _DEC_BUTTON_ARRAY[touch].TZ_GetY() - 5;
					w = _DEC_BUTTON_ARRAY[touch].TZ_GetWidth() + 10;
					h = _DEC_BUTTON_ARRAY[touch].TZ_GetHeight() + 10;
				}
				Button *button = &_DEC_BUTTON_ARRAY[touch];			
				if (CHECK_POINT_IN_RECT(TOUCHS(i)->x, TOUCHS(i)->y, x, y, w, h))
				{
					pressed = true;
					if (TOUCHS(i)->_state == TOUCH_DOWN)
					{
						if (button->_state != BUTTON_STATE_SELECT && Button::GetButtonActive() == NULL)
						{


							button->SetState(BUTTON_STATE_SELECT);
						}
					}
					else if (TOUCHS(i)->_state == TOUCH_DRAG)
					{
						if (button->_state != BUTTON_STATE_SELECT && Button::GetButtonActive() == NULL)
						{
							button->SetState(BUTTON_STATE_SELECT);
						}
					}
					else if (TOUCHS(i)->_state == TOUCH_UP)
					{
						button->SetState(BUTTON_STATE_ACTIVE);
						TOUCHS(i)->_flag = false;
					}
					hasTouchButton = true;
				}
				else
				{					
					if (button->_state == BUTTON_STATE_SELECT)
					{
						for (int j = 0; j < _DEC_BUTTON_NUMS; j++)
						{
							if (_DEC_BUTTON_ARRAY[j]._state != BUTTON_STATE_LOCK && _DEC_BUTTON_ARRAY[j]._state != BUTTON_STATE_HIDE)
							{
								_DEC_BUTTON_ARRAY[j].SetState(BUTTON_STATE_NORMAL);
							}
						}
						CGame::_this->ClearAllTouch();
					}

					if (TOUCHS(i)->_state == TOUCH_UP)
					{						
						CGame::_this->ClearAllTouch();
					}
				}
			}
			if (TOUCHS(i)->_state == TOUCH_UP && !hasTouchButton)
			{
				if (_isRenderAbout)
				{
					_stateAbout = 2;
				}
				else
				{
					if (_isRenderSetting)
					{
						_stateSetting = 2;
					}
				}
				CGame::_this->ClearAllTouch();
			}
		}
	}
	if (pressed == false)
	{
		return false;
	}
	else
	{
		return true;
	}
}
void CGameStateMainMenu::UpdateTouch()
{

}