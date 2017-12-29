#if !CUSER_H
#define CUSER_H 1

#define MAX_SAVE_BUFFER_CACHE					10000

#define MAX_STRING_ID							128

#define SAVE_GAME_VERSION_0						0 //version 1.0.0
#define SAVE_GAME_VERSION_1						1 //version 1.0.1
#define CURRENT_SAVE_VERSION					SAVE_GAME_VERSION_1 

#define SAVE_FILE_NAME "data.pine"

#define SAVE_FORMAT_CURRENT_VERSION				(0)
#define SAVE_FORMAT_USER_ID						(SAVE_FORMAT_CURRENT_VERSION+8) //INT_TIME
#define SAVE_FORMAT_USER_NAME					(SAVE_FORMAT_USER_ID + MAX_STRING_ID) //char 128
#define SAVE_FORMAT_PLAY_TIMES					(SAVE_FORMAT_USER_NAME + MAX_STRING_ID) //INT_TIME
#define SAVE_FORMAT_HIGHT_SCORE					(SAVE_FORMAT_PLAY_TIMES + 8) //INT32

#define SAVE_FORMAT_MUSIC						(SAVE_FORMAT_HIGHT_SCORE + 4)//BYTE
#define SAVE_FORMAT_SOUND						(SAVE_FORMAT_MUSIC + 1)//BYTE
#define SAVE_FORMAT_NOTIFY						(SAVE_FORMAT_SOUND + 1)//BYTE


//char fever
#define SAVE_FORMAT_CHAR_FEVER_STATE					(SAVE_FORMAT_NOTIFY + 1)
#define SAVE_FORMAT_CHAR_FEVER_COLLECT					(SAVE_FORMAT_CHAR_FEVER_STATE + 2*NUM_CHAR_FEVER)

#define SAVE_FORMAT_TUTORIAL_STEP						(SAVE_FORMAT_CHAR_FEVER_COLLECT + 1*NUM_CHAR_FEVER)
#define SAVE_FORMAT_REMOVE_ADS					(SAVE_FORMAT_TUTORIAL_STEP + 2)

//version 1
#define SAVE_FORMAT_CURRENT_DAY							(SAVE_FORMAT_REMOVE_ADS + 1)
#define SAVE_FORMAT_NUM_PLAY_ADAY						(SAVE_FORMAT_CURRENT_DAY + 2)
#define SAVE_FORMAT_CAN_INVITE_FB						(SAVE_FORMAT_NUM_PLAY_ADAY + 2)

//

#define SAVE_FORMAT_END							(SAVE_FORMAT_CAN_INVITE_FB + 1)

class CUser
{
public:
	CUser()
	{
		sprintf(_userID, "%s","0");
		sprintf(_userName, "%s", "Unkown");
	};

	INT32 _highScore;
	bool _isRemoveAds;    
	INT_TIME _playTimes;

	char _userID[MAX_STRING_ID];
	char _userName[MAX_STRING_ID];
	char _avatarURL[1024];

	//save game
	INT_TIME _versionFormat;
	char _buffer[MAX_SAVE_BUFFER_CACHE];

	void DataDefault();
	void DataSave();
	void DataLoad();
	void DataFromBuffer();
	void DataToBuffer();
	void SynForNewVersion(INT_TIME fromVersion);
	void NewDataDefault(INT_TIME version);	

	int _currentDay;
	int _numPlayADay;
	bool _canInvitFB;
	void ResetTimeADay();
};

#endif