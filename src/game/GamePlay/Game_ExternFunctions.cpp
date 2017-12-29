#include "pch.h"
#include "../GameCore/CGame.h"
#if defined(_WINDOWS)
extern "C" void OS_AdBoxDownloadFile() {}
extern "C" float GetAccelX(){ return 0; }
extern "C" float GetAccelY(){ return 0; }
extern "C" float GetAccelZ(){ return 0; }
extern "C" Orientation GetViewOrientation()
{
	return Orientation::Landscape_90;
}
extern "C" void OS_BasicPostSocial(const char* feed){}

extern "C" void OS_GameCenterAuthentication(PineSocial* socil){}
extern "C" void OS_GameCenterLoadScore(PineSocial* social){}
extern "C" void OS_GameCenterSubmitScore(INT_TIME value, const char* leaderboard){}
extern "C" void OS_ShowLeaderboard(const char*  leaderboard){}
extern "C" void OS_GameCenterLoadFriend(PineSocial* social){}
extern "C" void OS_GameCenterSubmitAchievement(int percent, const char* leaderboard, bool noti){}
extern "C" void OS_ResetAllAchievement(){}
extern "C" void OS_GameCenterLoadAchievement(){}
extern "C" void ShowIADFullScreen(){}
extern "C" void charboostShowAtMenu(const char* title){}
extern "C" void charboostMoreGame(){}
extern "C" void ShowAdmobFullScreen(){}
extern "C" bool chartboost_hasAdsInterstitial(const char* loc){ return false; }
extern "C" bool charboostHasShowVideoReward(const char* title){ return false; }
extern "C" void charboostShowVideoReward(const char* title){}
extern "C" bool HasAdmobToShow(){ return false; }
extern "C" void OS_SystemMessageAsk(const char* feed, int code){}
extern "C" void				GoToReviewInItune(){}
extern "C" void FacebookInviteSuggetsFriend(const char* feed, const char* title)
{}
extern "C" void OS_InitFacebook(PineSocial *social){}
extern "C" const char* OS_GetFacebookAvartar(const char* id) { return ""; }
extern "C" void OS_LoadFriendFacebook(PineSocial *social){}
#endif

#if defined(_WINDOWS_PHONE8)
extern "C" void OS_AdBoxDownloadFile() {}
#endif