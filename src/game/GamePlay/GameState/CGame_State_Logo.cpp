#include "pch.h"
#include "../../GameCore/CGame.h"
#include "../../GameCore/Os_Functions.h"
//extern "C" void InitADManager();
//extern "C" void InitADBanner();
//extern "C" void InitTakePicture();
//extern "C" void takeAPicture();
//extern "C" void LoadImageToPngFile(const char* url,const char *file_name);
extern "C" void AuthTwitter();
extern "C" void CaptureScreenshot();
extern "C" void PostScreenToTwitter();
extern "C" void charboostShowVideoReward(const char* title);
extern "C" void ShowAdmobFullScreen();
extern "C" void OS_InitCaptureViewVideo();
extern "C" void OS_ScreenCaptureVideoStart();
extern "C" void OS_ScreenCaptureVideoStop();
extern "C" int OS_GetCaptureVideoState();
extern "C" void OS_SetCaptureVideoState(int state);
extern "C" void OS_VideoPostSocial(const char* feed);
extern "C" void OS_ShowWebPage(const char* url, bool isOffline);

void CGameStateLogo::Init()
{
	_loadingStep = 0;
	_autoInscreaseStep = true;
	GAME()->LoadSpriteCurrentVer(SPRITEID_BG_LOADING);
    GAME()->LoadSpriteCurrentVer(SPRITEID_HUD_LOADING_ICON);
   
    MAIN_GAME()->_animLoading.SetAnimSprite(GET_SPRITE(SPRITEID_HUD_LOADING_ICON), SPRITEID_HUD_LOADING_ICON);
    MAIN_GAME()->_animLoading.SetAnim(0, true);
    MAIN_GAME()->_animLoading.SetPos(k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2);
	InitFoot();
	USER().DataLoad();
}
void CGameStateLogo::Update()
{
	if (MAIN_GAME()->_sub_state == k_LOOP)
	{
		switch (_loadingStep)
		{
		case 0:
            if (OS_IsLogginFacebook())
            {
                MAIN_GAME()->_fbSosial = new PineFacebookController();
                MAIN_GAME()->_fbSosial->Init();
                MAIN_GAME()->_stateLoginFB = ShakeDuckOffGame::enumStateLoginFB::LOGIN_FB_PROCESSING;
            }
			break;

		case LOGO_STEP_BEGIN_LOAD_RESOURCE:
			GAME()->LoadSpriteCurrentVer(SPRITEID_FONT_SCORE);			
			break;

		case 11:
			GAME()->LoadSpriteCurrentVer(SPRITEID_BG_INGAME);
			break;

		case 12:
			GAME()->LoadSpriteCurrentVer(SPRITEID_HUD_BAR);
			break;

		case 13:
			GAME()->LoadSpriteCurrentVer(SPRITEID_OBJECTS_CLOUD);
			break;

		case 14:
			GAME()->LoadSpriteCurrentVer(SPRITEID_OBJECTS_PROPS);
			break;

		case 15:
			GAME()->LoadSpriteCurrentVer(SPRITEID_OBJECTS_TREE);
			break;

		case 16:
			GAME()->LoadSpriteCurrentVer(SPRITEID_OBJECTS_BULLSEYE);
			break;

		case 17:
			GAME()->LoadSpriteCurrentVer(SPRITEID_CHAR_DOG_01);
			break;

		case 18:
			GAME()->LoadSpriteCurrentVer(SPRITEID_OBJECTS_SUN);
			break;

		case 19:
			GAME()->LoadSpriteCurrentVer(SPRITEID_OBJECTS_MOUNTH);
			break;

		case 20:
			GAME()->LoadSpriteCurrentVer(SPRITEID_HUD_TITTLE);
			break;

		case 21:
			GAME()->LoadSpriteCurrentVer(SPRITEID_CHAR_DUCK_01);
			break;


		case 22:
			GAME()->LoadSpriteCurrentVer(SPRITEID_HUD_RESULF);			
			break;

		case 23:
			GAME()->LoadSpriteCurrentVer(SPRITEID_FONT_BESTSCORE);
			break;

		case 24:
			GAME()->LoadSpriteCurrentVer(SPRITEID_FONT_HIGHSCORE);
			GET_SPRITE(SPRITEID_FONT_HIGHSCORE)->SetCharSpacing(9);
			break;

		case 25:
			GAME()->LoadSpriteCurrentVer(SPRITEID_HUD_FEVER);			
			break;

		case 26:
			GAME()->LoadSpriteCurrentVer(SPRITEID_OBJECT_DUCK_FEVER_F);
			break;

		case 27:
			GAME()->LoadSpriteCurrentVer(SPRITEID_FONT_COMBO);
			break;

		case 28:
			GAME()->LoadSpriteCurrentVer(SPRITEID_FONT_COMBO_SHADOW);
			break;

		case 29:
			GAME()->LoadSpriteCurrentVer(SPRITEID_FONT_FEVERSCORE);
			break;

		case 30:
			GAME()->LoadSpriteCurrentVer(SPRITEID_OBJECT_DUCK_FEVER_E);
			break;

		case 31:
			GAME()->LoadSpriteCurrentVer(SPRITEID_OBJECT_DUCK_FEVER_V);
			break;

		case 32:
			GAME()->LoadSpriteCurrentVer(SPRITEID_OBJECT_DUCK_FEVER_E2);
			break;

		case 33:
			GAME()->LoadSpriteCurrentVer(SPRITEID_OBJECT_DUCK_FEVER_R);
			break;

		case 34:
			if (!MAIN_GAME()->IsTutorialComplete())
			{
				GAME()->LoadSpriteCurrentVer(SPRITEID_TUTORIAL_OBJECTS);
			}
			break;

		case 35:
			GAME()->LoadSpriteCurrentVer(SPRITEID_CHAR_DUCK_FEVER);
			break;

		case 36:
			break;

		case 37:
			break;


		case 44:    
			_foot[15]._state = 1;
			GAME()->LoadSFX(SFX_BUTTON);
			GAME()->LoadSFX(SFX_ENDGAME_WITH_HIGHSCORE);
			GAME()->LoadSFX(SFX_ENDGAME_WITHOUT_HIGHSCORE);
			GAME()->LoadSFX(SFX_INGAME_HIGHSCRORE);
			GAME()->LoadSFX(SFX_READY);
			GAME()->LoadSFX(SFX_RELOAD);
			GAME()->LoadSFX(SFX_RESULT_WITH_HIGHSCORE);
			GAME()->LoadSFX(SFX_RESULT_WITHOUT_HIGHSCORE);
			GAME()->LoadSFX(SFX_SHOOT);
			GAME()->LoadSFX(SFX_SNAP);
			GAME()->LoadSFX(SFX_BONUS_SCORE_AFTER_FEVER);
			GAME()->LoadSFX(SFX_COLLECT_FEVER_CHARACTER);
			GAME()->LoadSFX(SFX_COMBO);
			GAME()->LoadSFX(SFX_FEVER_SHOOT);
			break;
		}
		UpdateFoot();
		if (_autoInscreaseStep)
		{
			_loadingStep++;
		}		
		if (_loadingStep >= LOGO_MAX_STEP_LOADING)
		{
			MAIN_GAME()->SetState(k_GS_GAME_PLAY);
		}
	}

}
void CGameStateLogo::Render()
{
	if (MAIN_GAME()->_sub_state == k_INIT)
	{
		Init();
		MAIN_GAME()->_sub_state = k_LOOP;
	}
	if (MAIN_GAME()->_sub_state == k_LOOP)
	{
		G()->SetScale(1, 1 + OFFSET_I4(0.2) + OFFSET_IPAD(0.34f), k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2);
		GET_SPRITE(SPRITEID_BG_LOADING)->DrawFrame(G(), 0, 0, 0);
		G()->ClearScale();
		GET_SPRITE(SPRITEID_BG_LOADING)->DrawFrame(G(), 3, 0, 0);
		RenderFoot();

	/*	float percent = _loadingStep / (float)(LOGO_MAX_STEP_LOADING);
		G()->SetColor(0xff0000ff);
		G()->FillRect(0, k_SCREEN_HEIGHT / 2 - 10, k_SCREEN_WIDTH*percent, 20, true);*/

	}
}
bool CGameStateLogo::UpdateTouchButton()
{
	return false;
}
void CGameStateLogo::UpdateTouch()
{

}

void CGameStateLogo::InitFoot()
{
	float ww = k_SCREEN_WIDTH / (float)(LOGO_MAX_STEP_LOADING / 3 );
	for (int i = 0; i < LOGO_MAX_STEP_LOADING / 3+1; i++)
	{
		_foot[i]._state = 0;
		_foot[i]._pos._x = 0 + ww*i + CMath::RANDOM(-5, 5);
		if (i%2==0)
		{			
			_foot[i]._pos._y = k_SCREEN_HEIGHT - 120 + OFFSET_IPAD(128) + OFFSET_I4(74) + CMath::RANDOM(-10, 10);
		}
		else
		{			
			_foot[i]._pos._y = k_SCREEN_HEIGHT - 100 + OFFSET_IPAD(128) + OFFSET_I4(74) + CMath::RANDOM(-10, 10);
		}

	}
}
void CGameStateLogo::UpdateFoot()
{
	for (int i = 0; i < LOGO_MAX_STEP_LOADING / 3 + 1; i++)
	{
		if (_loadingStep/3 >= i)
		{
			_foot[i]._state = 1;
		}
	}
}
void CGameStateLogo::RenderFoot()
{
	for (int i = 0; i < LOGO_MAX_STEP_LOADING / 3+1; i++)
	{
		if (_foot[i]._state ==1)
		{
			GET_SPRITE(SPRITEID_BG_LOADING)->DrawFrame(G(), 1 + i % 2, _foot[i]._pos._x, _foot[i]._pos._y);
		}		
	}
}





