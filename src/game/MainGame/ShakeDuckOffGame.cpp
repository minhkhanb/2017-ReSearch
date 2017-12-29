//
//  BugerGame.cpp
//  PineEntertainment
//
//  Created by Do Thanh Nghia on 12/26/14.
//
//
#include "pch.h"
#include "ShakeDuckOffGame.h"

void ShakeDuckOffGame::Init()
{
    _syn_save_state = SYN_SAVE_NONE;
    _timesCharbost = TIMES_CHARBOST;
    _check_syn_save_complete = false;
	
	_isProcessingRevive = enumTypeProcessing::REVIVE_NONE;
    CGame::_this->_stateInviteFriend = 0;
    _fbSosial = NULL;
    _stateLoginFB = enumStateLoginFB::LOGIN_FB_NONE;
    _user._numPlayADay = 0;
    _user._canInvitFB = true;
    PineGame::Init();
}
void ShakeDuckOffGame::UpdateAndRender()
{
	G()->ClearClip();
	G()->SetOpacity(100);
	if (!GAME()->LoadGameForceground())
	{
		return;
	}
	
    CheckAndSynSave();
    
    switch (_current_state)
    {
        case k_GS_LOGO:
            _gameStateLogo.Render();
            //_gameStateLogo.Update();
            break;
            
        case k_GS_GAME_PLAY:
            _gameStateGamePlay.Render();
           // _gameStateGamePlay.Update();
            break;
    }
    
    if (_current_state == k_GS_GAME_PLAY)
    {
        if (_stateLoginFB == enumStateLoginFB::LOGIN_FB_PROCESSING)
        {
            if (_fbSosial->GetState() == PINESOCIAL_STATE_COMPLETED)
            {
                if (_fbSosial->GetFlag() == SOCIAL_FLAG_FULL_ACCESS)
                {
                    _stateLoginFB = enumStateLoginFB::LOGIN_FB_COMPLETE;
                }
                else if (_fbSosial->GetFlag() == SOCIAL_FLAG_ACCESS_DENINE || _fbSosial->GetFlag() == SOCIAL_FLAG_NO_ACCOUNT_LOGIN)
                {
                    _stateLoginFB = enumStateLoginFB::LOGIN_FB_NONE;
                }
            }
        }
        
        if (UpdateSynSave())
        {
            G()->SetOpacity(100);
            G()->SetColor(0xaa0000000);
            G()->FillFullScreen(true);
			_animLoading.DrawAnimObject(G());
			_animLoading.UpdateAnimObject();
            return;
        }
        if (_isProcessingRevive != enumTypeProcessing::REVIVE_NONE)
        {
			switch (_isProcessingRevive)
			{
			case enumTypeProcessing::REVIVE_PROCESSING_ADS:
				if (PineAdBox::showAdState > 0)
				{
					if (SHOW_AD_CANCEL == PineAdBox::showAdState)
					{
						PineAdBox::showAdState = SHOW_AD_NONE;
						_countTimeDeplay = 1 * 1000;
					}
					if (PineAdBox::showAdState == SHOW_AD_COMPLETED)
					{
						PineAdBox::showAdState = 100;
						_countTimeDeplay = 1 * 1000;
					}
					if (PineAdBox::showAdState >= 100)
					{
						PineAdBox::showAdState++;
						if (PineAdBox::showAdState == 130)
						{
							PineAdBox::showAdState = SHOW_AD_NONE;							
						}
					}
				}
				if (_countTimeDeplay <= 0 && PineAdBox::showAdState == SHOW_AD_NONE)
				{
					_isProcessingRevive = enumTypeProcessing::REVIVE_NONE;
					MAIN_GAME()->_gamePlay.ClearDuckFear(CDuck::enumDuckState::DUCK_STATE_FLY);
					MAIN_GAME()->_gamePlay._hasRivive = true;
					MAIN_GAME()->_gamePlay._isEndGame = false;
					MAIN_GAME()->_gameStateGamePlay._button[GAMEPLAY_BUTTON_PAUSE].SetState(BUTTON_STATE_NORMAL);
					MAIN_GAME()->_gameStateGamePlay._button[GAMEPLAY_BUTTON_REVIVE].SetState(BUTTON_STATE_HIDE);
                    
                    MAIN_GAME()->_gameStateGamePlay._isPauseGame = true;
                    MAIN_GAME()->_gameStateGamePlay._button[GAMEPLAY_BUTTON_RESUM].SetState(BUTTON_STATE_NORMAL);
                    MAIN_GAME()->_gameStateGamePlay._button[GAMEPLAY_BUTTON_END].SetState(BUTTON_STATE_NORMAL);
				}
				break;

			case enumTypeProcessing::REVIVE_PROCESSING_LOGIN_FB:
				if (_countTimeDeplay <= 0 && _stateLoginFB == enumStateLoginFB::LOGIN_FB_COMPLETE)
				{
					_isProcessingRevive = enumTypeProcessing::REVIVE_NONE;
					MAIN_GAME()->_gamePlay.ClearDuckFear(CDuck::enumDuckState::DUCK_STATE_FLY);
					MAIN_GAME()->_gamePlay._hasRivive = true;
					MAIN_GAME()->_gamePlay._isEndGame = false;
					MAIN_GAME()->_gameStateGamePlay._button[GAMEPLAY_BUTTON_PAUSE].SetState(BUTTON_STATE_NORMAL);
					MAIN_GAME()->_gameStateGamePlay._button[GAMEPLAY_BUTTON_REVIVE].SetState(BUTTON_STATE_HIDE);
                    
                    MAIN_GAME()->_gameStateGamePlay._isPauseGame = true;
                    MAIN_GAME()->_gameStateGamePlay._button[GAMEPLAY_BUTTON_RESUM].SetState(BUTTON_STATE_NORMAL);
                    MAIN_GAME()->_gameStateGamePlay._button[GAMEPLAY_BUTTON_END].SetState(BUTTON_STATE_NORMAL);
                    
				}
				break;

			case  enumTypeProcessing::REVIVE_PROCESSING_INVITE_FB:
				if (_countTimeDeplay <= 0 && CGame::_this->_stateInviteFriend == 2)
				{
					_isProcessingRevive = enumTypeProcessing::REVIVE_NONE;
					MAIN_GAME()->_gamePlay.ClearDuckFear(CDuck::enumDuckState::DUCK_STATE_FLY);
					MAIN_GAME()->_gamePlay._hasRivive = true;
					MAIN_GAME()->_gamePlay._isEndGame = false;
					MAIN_GAME()->_gameStateGamePlay._button[GAMEPLAY_BUTTON_PAUSE].SetState(BUTTON_STATE_NORMAL);
					MAIN_GAME()->_gameStateGamePlay._button[GAMEPLAY_BUTTON_REVIVE].SetState(BUTTON_STATE_HIDE);
					USER()._canInvitFB = false;
                    
                    MAIN_GAME()->_gameStateGamePlay._isPauseGame = true;
                    MAIN_GAME()->_gameStateGamePlay._button[GAMEPLAY_BUTTON_RESUM].SetState(BUTTON_STATE_NORMAL);
                    MAIN_GAME()->_gameStateGamePlay._button[GAMEPLAY_BUTTON_END].SetState(BUTTON_STATE_NORMAL);
				}
				break;
			}
           
           
            _countTimeDeplay-= DT();
            G()->SetColor(0xaa000000);
            G()->FillFullScreen(true);
            _animLoading.DrawAnimObject(G());
            _animLoading.UpdateAnimObject();
            return;
        }
    }
    
	switch (_current_state)
	{
	case k_GS_LOGO:
		//_gameStateLogo.Render();
		_gameStateLogo.Update();
		break;
	
	case k_GS_GAME_PLAY:
		//_gameStateGamePlay.Render();
		_gameStateGamePlay.Update();
		break;
	}
    
    PineGame::UpdateAndRender();
    
}
void ShakeDuckOffGame::CheckAndSynSave()
{
#ifdef MAC_OS
    //printf("\n falg = %d, satte = %d",GAME()->_pine_social->GetFlag(),GAME()->_pine_social->GetState());
    if (_current_state != k_GS_LOGO && !_check_syn_save_complete && ((GAME()->_pine_social->GetFlag() == SOCIAL_FLAG_FULL_ACCESS ||
                                                                      GAME()->_pine_social->GetFlag() == SOCIAL_FLAG_ID_IS_STRING_FORMAT )&&
                                                                     GAME()->_pine_social->GetState() == PINESOCIAL_STATE_COMPLETED))
    {
        ((PineGameCenterController*)GAME()->_pine_social)->LoadMyScore();
        
        _check_syn_save_complete = true;
        _syn_save_step = 0;
        _syn_save_state = SYN_SAVE_PROCESSING;
        
    }
#endif
}
bool ShakeDuckOffGame::UpdateSynSave()
{ 
    if (_syn_save_state)
    {
        _syn_time_out -= DT();
        //printf("\n _syn_save_step = %d",_syn_save_step);
        switch (_syn_save_step) {
            case 0:
                //_syn_save_step++;
                _user.DataLoad();
                _syn_time_out = 5000;
                break;
            case 1:
                //_syn_save_step++;
                break;
            case 2:
#ifdef MAC_OS
                if (_syn_time_out < 0)
                {
                    _syn_save_state = SYN_SAVE_NONE;
                }
                else if (CGame::_this->_pine_social->GetState() != PINESOCIAL_STATE_COMPLETED)
                {
                    _syn_save_step--;
                }
#endif
                break;
            case 14:
#ifdef MAC_OS
                _online_score = PineGameCenterController::GetMyScore(GC_SUBMIT_SCORE_ID);
                printf("\n_online_score = %d, high_score = %d", _online_score, _user._highScore);
#endif
                break;
            case 15:
#ifdef MAC_OS
                printf("\n user id = %s",_user._userID);
                _can_submit = true;
                if ((strcmp(_user._userID, "0") == 0 && _online_score <= 0) ||
                    ((strcmp(_user._userID, "0") != 0 && strcmp(_user._userID ,GAME()->_pine_social->GetIdString()) == 0)))
                {
                   sprintf(_user._userID,"%s",GAME()->_pine_social->GetIdString());
                    
                }
                else if (strcmp(_user._userID , GAME()->_pine_social->GetIdString()) != 0)
                {
                    sprintf(_user._userID,"%s",GAME()->_pine_social->GetIdString());
                    if (_online_score > _user._highScore) //get info form server
                    {
                        _user._highScore = _online_score;
                        _user.DataSave();
                    }
                    else
                    {
                        PineGameCenterController::SubmitMyScore(GC_SUBMIT_SCORE_ID, _user._highScore);
                    }
                }
                else
                {
                    _can_submit = false;
                }
#endif
                break;
                
            case 30:
                _syn_save_state = SYN_SAVE_NONE;
                break;
                
                
                
            default:
                break;
        }
        _syn_save_step++;
    }
    return _syn_save_state != SYN_SAVE_NONE;
}

bool ShakeDuckOffGame::IsTutorialComplete()
{ 
	return _stepTutorial >= MAX_STEP_TUTORIAL;
}
void ShakeDuckOffGame::InitTutorial()
{
	_animTutorial.SetAnimSprite(GET_SPRITE(SPRITEID_TUTORIAL_OBJECTS), SPRITEID_TUTORIAL_OBJECTS);
	_animTutorial.SetAnim(0, false);
	_animTutorial.SetPos(k_SCREEN_WIDTH/2, k_SCREEN_HEIGHT/2);
	_stepTutorial = 0;
}
void ShakeDuckOffGame::RenderTutorial()
{
	if (_stepTutorial == 0)
	{
		if (_animTutorial.IsCurrentAnimStop())
		{
			_animTutorial.SetAnim(1, false);
			_stepTutorial = 1;
			_countTimeDeplay = 180;
		}
		GET_SPRITE(SPRITEID_TUTORIAL_OBJECTS)->DrawFrame(G(), 10, k_SCREEN_WIDTH / 2, 130);
	}
	else if(_stepTutorial == 1)
	{
		_countTimeDeplay--;
		if (_countTimeDeplay <= 0)
		{
			_countTimeDeplay = 0;
		}
		GET_SPRITE(SPRITEID_TUTORIAL_OBJECTS)->DrawFrame(G(), 11, k_SCREEN_WIDTH / 2, 130);
	}
	_animTutorial.DrawAnimObject(G());
	_animTutorial.UpdateAnimObject();
}