#include "pch.h"
#include "../../GameCore/CGame.h"
#include "../../GameCore/Os_Functions.h"
extern "C" void FacebookInviteSuggetsFriend(const char* feed, const char* title);
#undef _DEC_BUTTON_NUMS
#undef _DEC_BUTTON_ARRAY
#undef _DEC_ANIM_BUTTON_BEIGN
#undef _DEC_SPRITE_BUTTON

#define _DEC_BUTTON_NUMS				(GAMEPLAY_BUTTON_NUMS)
#define _DEC_BUTTON_ARRAY				_button
#define _DEC_ANIM_BUTTON_BEGIN			0
#define _DEC_SPRITE_BUTTON				SPRITEID_HUD_BAR


void CGameStateGamePlay::Init()
{
	GAME()->StopCurrentMusic();
	GAME()->LoadMusic(BGM_MAINMENU);
	GAME()->PlayCurrentMusic(-1);
	MAIN_GAME()->_gamePlay.Init();
	for (int i = 0; i < _DEC_BUTTON_NUMS; i++)
	{
		int offY = 0;
		if (i == GAMEPLAY_BUTTON_PAUSE)
		{
			offY = OFFSET_IPAD(128) + OFFSET_I4(70);
		}
		_DEC_BUTTON_ARRAY[i].Init(BUTTON_STATE_NORMAL, 0, offY, A_HVCENTER, SFX_BUTTON);
		_DEC_BUTTON_ARRAY[i].SetAnim(_DEC_SPRITE_BUTTON, _DEC_ANIM_BUTTON_BEGIN + 4 * i);

	}
	
	_DEC_BUTTON_ARRAY[GAMEPLAY_BUTTON_REVIVE].Init(BUTTON_STATE_HIDE, 0, 0, A_HVCENTER, SFX_BUTTON);
	_DEC_BUTTON_ARRAY[GAMEPLAY_BUTTON_REVIVE].SetAnim(SPRITEID_HUD_RESULF, 6);

	_DEC_BUTTON_ARRAY[GAMEPLAY_BUTTON_RESUM].Init(BUTTON_STATE_HIDE, 0, 0, A_HVCENTER, SFX_BUTTON);
	_DEC_BUTTON_ARRAY[GAMEPLAY_BUTTON_RESUM].SetAnim(SPRITEID_HUD_BAR, 42);

	_DEC_BUTTON_ARRAY[GAMEPLAY_BUTTON_END].Init(BUTTON_STATE_HIDE, 0, 0, A_HVCENTER, SFX_BUTTON);
	_DEC_BUTTON_ARRAY[GAMEPLAY_BUTTON_END].SetAnim(SPRITEID_HUD_BAR, 46);

	_stateMenu = new CGameStateMainMenu();
	((CGameStateMainMenu*)_stateMenu)->InitTransaction(CGameStateMainMenu::enumTypeTransaction::TRANSACTION_INIT_MAIN_MENU);
	_stateMenu->Init();
    
	_isPauseGame = false;
	//MAIN_GAME()->_gamePlay.InitEffectFever();
   
}
void CGameStateGamePlay::Update()
{
	if (MAIN_GAME()->_sub_state == k_LOOP)
	{
		if (MAIN_GAME()->_gamePlay._isPlaying)
		{
			if (!UpdateTouchButton())
			{
#if USE_CONTROL_STICK
				MAIN_GAME()->_gamePlay.UpdateTouchControl();
#endif
			}
		}
		else
		{
			_stateMenu->Update();
		}		
		if (!_isPauseGame)
		{
			MAIN_GAME()->_gamePlay.Update();
		}
		
	}
}
void CGameStateGamePlay::Render()
{
	if (MAIN_GAME()->_sub_state == k_INIT)
	{
		Init();
		MAIN_GAME()->_sub_state = k_LOOP;
	}
	if (MAIN_GAME()->_sub_state == k_LOOP)
	{
		G()->SetColor(0xff008B8B);
		G()->FillFullScreen(true);
		MAIN_GAME()->_gamePlay.Render();

		if (_isPauseGame)
		{
			G()->SetColor(0xaa000000);
			G()->FillFullScreen(true);
		}
		if (MAIN_GAME()->_gamePlay._isPlaying)
		{
			for (int i = 0; i < _DEC_BUTTON_NUMS; i++)
			{
				_DEC_BUTTON_ARRAY[i].Render(G());
				_DEC_BUTTON_ARRAY[i].Update();
			}

			if (MAIN_GAME()->_gamePlay._isEndGame)
			{
				MAIN_GAME()->_gamePlay._animTimeRevive.DrawAnimObject(G());
				MAIN_GAME()->_gamePlay._animTimeRevive.UpdateAnimObject();
			}
		}
		else
		{
			_stateMenu->Render();
		}		

		//MAIN_GAME()->_gamePlay.RenderEffecetFerver();
	}
}
bool CGameStateGamePlay::UpdateTouchButton()
{
    if (MAIN_GAME()->_stepTutorial < MAX_STEP_TUTORIAL)
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
			case GAMEPLAY_BUTTON_END:
				_stateMenu = new CGameStateMainMenu();
				_stateMenu->Init();
				((CGameStateMainMenu*)_stateMenu)->InitTransaction(CGameStateMainMenu::enumTypeTransaction::TRANSACTION_COME_BACK_MENU);
				MAIN_GAME()->_gamePlay._isPlaying = false;
				MAIN_GAME()->_gamePlay.StopMoveBG();
				_DEC_BUTTON_ARRAY[GAMEPLAY_BUTTON_RESUM].SetState(BUTTON_STATE_HIDE);
				_DEC_BUTTON_ARRAY[GAMEPLAY_BUTTON_END].SetState(BUTTON_STATE_HIDE);
				_isPauseGame = false;
				if (MAIN_GAME()->_gamePlay._isFerverMode)
				{
					MAIN_GAME()->_gamePlay._isFerverMode = false;
					MAIN_GAME()->_gamePlay.InitCharFever();
				}
                Device::SetCurrentBalance();
				USER().DataSave();
				break;

			case GAMEPLAY_BUTTON_PAUSE:	
				_isPauseGame = true;	
				_DEC_BUTTON_ARRAY[GAMEPLAY_BUTTON_RESUM].SetState(BUTTON_STATE_NORMAL);
				_DEC_BUTTON_ARRAY[GAMEPLAY_BUTTON_END].SetState(BUTTON_STATE_NORMAL);
				break;

			case GAMEPLAY_BUTTON_RESUM:
                Device::SetCurrentBalance();
				_isPauseGame = false;
				_DEC_BUTTON_ARRAY[GAMEPLAY_BUTTON_RESUM].SetState(BUTTON_STATE_HIDE);
				_DEC_BUTTON_ARRAY[GAMEPLAY_BUTTON_END].SetState(BUTTON_STATE_HIDE);
				break;

			case GAMEPLAY_BUTTON_REVIVE:
                {
				if (MAIN_GAME()->_gamePlay._typeRevive == CGamePlay::enumTypeRevive::TYPE_REVIVE_ADS)
				{
					int adboxtype = -1;
					if (Device::IsNetworkAvailable())
					{
						adboxtype = PineAdBox::HasVideoAds();
					}
					if (adboxtype >= 0)
					{
						PineAdBox::ShowVideoAds(adboxtype);
						MAIN_GAME()->_isProcessingRevive = ShakeDuckOffGame::enumTypeProcessing::REVIVE_PROCESSING_ADS;
					}
				}
				else if (MAIN_GAME()->_gamePlay._typeRevive == CGamePlay::enumTypeRevive::TYPE_REVIVE_LOGIN)
				{
					if (MAIN_GAME()->_stateLoginFB == ShakeDuckOffGame::enumStateLoginFB::LOGIN_FB_NONE)
					{
						MAIN_GAME()->_fbSosial = new PineFacebookController();
						MAIN_GAME()->_fbSosial->Init();
						MAIN_GAME()->_stateLoginFB = ShakeDuckOffGame::enumStateLoginFB::LOGIN_FB_PROCESSING;
						MAIN_GAME()->_isProcessingRevive = ShakeDuckOffGame::enumTypeProcessing::REVIVE_PROCESSING_LOGIN_FB;
					}
				}
				else if (MAIN_GAME()->_gamePlay._typeRevive == CGamePlay::enumTypeRevive::TYPE_REVIVE_INVITE)
				{
					FacebookInviteSuggetsFriend("Play with me", "Shake Duck Off");
					MAIN_GAME()->_isProcessingRevive = ShakeDuckOffGame::enumTypeProcessing::REVIVE_PROCESSING_INVITE_FB;
					GAME()->_stateInviteFriend = 1;
				}

                    //Device::SetCurrentBalance();
                }				
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
			for (int touch = 0; touch < _DEC_BUTTON_NUMS; touch++)
			{
				if (_DEC_BUTTON_ARRAY[touch]._state == BUTTON_STATE_LOCK || _DEC_BUTTON_ARRAY[touch]._state == BUTTON_STATE_HIDE)
				{
					continue;
				}
			
				int x = _DEC_BUTTON_ARRAY[touch].TZ_GetX() - 30;
				int y = _DEC_BUTTON_ARRAY[touch].TZ_GetY() - 30;
				int w = _DEC_BUTTON_ARRAY[touch].TZ_GetWidth() + 60;
				int h = _DEC_BUTTON_ARRAY[touch].TZ_GetHeight() + 60;
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

				}
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
void CGameStateGamePlay::UpdateTouch()
{

}