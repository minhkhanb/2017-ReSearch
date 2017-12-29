#include "pch.h"
#include "../GameCore/CGame.h"
#include "../GameCore/Os_Functions.h"
#include <stdio.h>
extern "C" void ServerADVERTStart();
extern "C" void PurchaseProduct(const char *indentify);
extern "C" bool isChartboostAvailable();
void CGame::OnScreenChange(PineSize size)
{
	///////////////////////////////////////////
	// INIT CONTEXT
	///////////////////////////////////////////

	//define support version
#if defined(_WINDOWS_PHONE8)
	ResVersion support_version[] = {
		{ SupportVersion::RESVER_1280_768, Orientation::Landscape_90 },
		{ SupportVersion::RESVER_800_480, Orientation::Landscape_90 }
	};
	int support_version_num = 2;
#elif defined(_WIN8)
	ResVersion support_version[] = {
		{ SupportVersion::RESVER_1366_768, Orientation::Landscape_90 }
	};
	int support_version_num = 1;
#elif defined(MAC_OS)
	ResVersion support_version[] = {
			{ SupportVersion::RESVER_1334_750, Orientation::Landscape_90 },
	};
	int support_version_num = 1;
#elif defined(ANDROID_OS)
	ResVersion support_version[] = {
		{ SupportVersion::RESVER_960_640, Orientation::Landscape_90 },
		{ SupportVersion::RESVER_1024_768, Orientation::Landscape_90 },
		{ SupportVersion::RESVER_1136_640, Orientation::Landscape_90 },
		{ SupportVersion::RESVER_800_480, Orientation::Landscape_90 },
	};
	int support_version_num = 4;
#else //windows
	ResVersion support_version[] = {
		{ SupportVersion::RESVER_1334_750, Orientation::Landscape_90 },
	};
	int support_version_num = 1;
#endif
    //detect device version
    {
        ResVersion device_support_version[] = {
            { SupportVersion::RESVER_960_640, Orientation::Landscape_90 },
            { SupportVersion::RESVER_1024_768, Orientation::Landscape_90 },
            { SupportVersion::RESVER_1136_640, Orientation::Landscape_90 },
            { SupportVersion::RESVER_800_480, Orientation::Landscape_90 },
        };
        PineSize dvsize = { _device.ScreenSize.W, _device.ScreenSize.H };
        _device._version = (GetAppropriateVersion(dvsize, device_support_version, 4)).Version;
    }
	if (support_version_num > 0){
		PineSize dvsize = { _device.ScreenSize.W, _device.ScreenSize.H };
		ResVersion curr_ver = GetAppropriateVersion(dvsize, support_version, support_version_num);
		Alignment game_align = { HorizontalAlign::Center, VerticalAlign::Middle };
		AlignmentContext(game_align, dvsize, curr_ver, true, 0);

		switch (curr_ver.Version)
		{
		case SupportVersion::RESVER_800_480:
			//_current_version = VERSION_480x800;
			break;
		case SupportVersion::RESVER_960_640:
			//_current_version = VERSION_640x960;
			break;
		case SupportVersion::RESVER_1024_768:
			//_current_version = VERSION_768x1024;
			break;
		case SupportVersion::RESVER_1136_640:
			//_current_version = VERSION_750x1334;
			break;
		case SupportVersion::RESVER_1366_768:
			//_current_version = VERSION_1366x768;
			break;
		}
	}
}
void CGame::InitEx()
{
	InitBuff(UNZIP_BUFFER_LENGTH_HIGH);
	
	_GameState = k_GS_LOGO;
	_GameSubState = k_INIT;
#ifdef MAC_OS
#if ADD_ADVERT
	ServerADVERTStart();
#endif
#endif

    
#ifdef MAC_OS
    _pine_social = new PineGameCenterController();
    _pine_social->Init();
#endif
    
	_main_game = new ShakeDuckOffGame();
	_main_game->Init();
	_main_game->SetState(k_GS_LOGO);
	_main_game->_game_id = APPLE_ID;

	_current_game = _main_game;

	_stateInviteFriend = 0;
	GAME()->setUseOptimizeDrawModule(1);
}
void CGame::ReleaseEx()
{
	
}
void CGame::PauseEx()
{

}
void CGame::ResumeEx()
{

}
void CGame::OnBackKeyPress(){
	//Implement function when 
}
void CGame::UpdateAndRender()
{
	_main_game->UpdateAndRender();
}

void CGame::UpdateTouch(int xx, int yy, int touch_state, int touch_id, int tap_count)
{
	/*switch (_GameState)
	{
	
	}*/
}
void CGame::PurchaseIAP(const char* strID, bool sucessed)
{
    /*
    if(sucessed)
    {
        for (int i = 0; i < TOTAL_IAP_NAME_DES_ROW; i++)
        {
            if (strcmp(GDS_Exporter::iap_name_des[TOTAL_IAP_NAME_DES*i+IAP_NAME_DES_APPLE_ID], strID) == 0)
            {
                if (strcmp(strID, "WWW_RemoveAd") == 0)
                {
                    USER()._removeAdd = true;
                    USER().DataSave();
                }
                else
                {
                    USER().AddCoin(GDS_Exporter::iap_value_des[i]);
                    SubmitTotalCoin();
                }
                
                break;
            }
        }
    }
    _isProssesIap = false;
     */
}
void CGame::UnloadGameBackground()
{
	//printf("\nUnload Game Back ground");
	if (!_compete_backup_stock)
	{
		_step_force_loading = 0;
		BackupAnimStock();
		for (int i = 0; i < TOTAL_SPRITE_INGAME; i++)
		{
			if (_sprite_loaded[i])
			{
				//printf("\nUnload SpriteID = %d",i);
				UnloadSpriteVer(i);
				_sprite_loaded[i] = 1;
			}
		}
		_compete_backup_stock = true;
		printf("\nUnload background");
	}

}
bool CGame::LoadGameForceground()
{

	if (_step_force_loading >= TOTAL_SPRITE_INGAME + 10)
	{
		return true;
	}

	_compete_backup_stock = false;
	//printf("\nloading force ground = %d",_step_force_loading);
	//G()->SetColor(0);
	//G()->FillRect(0,0,k_SCREEN_WIDTH,k_SCREEN_HEIGHT);


	if (_step_force_loading >= 10)
	{
		for (int i = 0; i < 100; i++)
		{

			if (_sprite_loaded[_step_force_loading - 10])
			{
				//printf("\n_step_force_loading-10 = %d",(_step_force_loading-10));
				LoadSpriteCurrentVer(_step_force_loading - 10, _sprite_antialias_loaded[_step_force_loading - 10], _sprite_optimized_loaded[_step_force_loading - 10]);
			}
			_step_force_loading++;
			if (_step_force_loading >= TOTAL_SPRITE_INGAME + 10)
			{
				RestoreAnimStock();

				return true;
			}

		}
	}
	else
	{
		_step_force_loading++;
	}
	return false;
}


void CGame::GetRectTranform(SMatrix4x4 tranf, GameRect inRect, GameRect &outRect)
{
	SMatrix4x4 clipMatrix;
	clipMatrix._m0 = inRect._x;
	clipMatrix._m1 = inRect._y;
	clipMatrix._m2 = 0;
	clipMatrix._m3 = 1;

	clipMatrix._m4 = inRect._x + inRect._w;
	clipMatrix._m5 = inRect._y;
	clipMatrix._m6 = 0;
	clipMatrix._m7 = 1;

	clipMatrix._m8 = inRect._x + inRect._w;
	clipMatrix._m9 = inRect._y + inRect._h;
	clipMatrix._m10 = 0;
	clipMatrix._m11 = 1;

	clipMatrix._m12 = inRect._x;
	clipMatrix._m13 = inRect._y + inRect._h;
	clipMatrix._m14 = 0;
	clipMatrix._m15 = 1;

	clipMatrix = SMatrix4x4::Mul(clipMatrix, tranf);

	outRect._x = clipMatrix._m0;
	outRect._y = clipMatrix._m1;
	outRect._w = clipMatrix._m4 - clipMatrix._m0;
	outRect._h = clipMatrix._m9 - clipMatrix._m1;	
}
bool CGame::CheckRectOverlapCircle(int x, int y, int w, int h, int cX, int cY, int R)
{
	float px = cX;
	float py = cY;

	if (px < x)
		px = x;
	else if (px >  x + w)
		px = x + w;

	if (py < y)
		py = y;
	else if (py > y + h)
		py = y + h;

	float dx = cX - px;
	float dy = cY - py;

	return (dx*dx + dy*dy) <= R*R;
}
bool CGame::CheckCircleOverlapCircle(int cX1, int cY1, int R1, int cX2, int cY2, int R2)
{
	float dx = cX1 - cX2;
	float dy = cY1 - cY2;
	return CMath::SQRT(dx*dx + dy*dy) <= R1 + R2;
}
bool CGame::checkRectOverlapRect(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2)
{
	if (x1 < x2 + w2 && x1 + w1 > x2 &&
		y1 < y2 + h2 && y1 + h1 > y2)
	{
		return true;
	}
	return false;
};