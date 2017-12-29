//FPS
#define FPS_MAX						60
#define FPS_DELAY_TIME				(1000/CGame::_this->FPS_LIMIT)

//Game State
#define k_GS_LOGO					1
#define k_GS_GAME_PLAY				3

//Game Sub State
#define k_INIT						1
#define k_FIRST_TIME				2
#define k_LOOP						3
#define k_DESTROY					4
#define k_EFFECT_UNLOAD				5

#define k_MACHINE_FAKE_OUT			5
#define k_MACHINE_WIN				6


#define OFFSET_IPAD(a)				(CGame::_this->_device._version == SupportVersion::RESVER_1024_768 ? a:0)
#define OFFSET_I4(a)				(CGame::_this->_device._version == SupportVersion::RESVER_960_640 ? a:0)



