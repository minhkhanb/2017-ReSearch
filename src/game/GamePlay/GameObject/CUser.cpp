#include "pch.h"
#include "../../GameCore/CGame.h"
#include "../../GameCore/Os_Functions.h"
//
void CUser::DataDefault()
{    
	for (int i = 0; i < MAX_SAVE_BUFFER_CACHE; i++)
	{
		_buffer[i] = 0;
	}
	_versionFormat = SAVE_GAME_VERSION_0;	
	_highScore = 0;
	_playTimes = 0;
    
	CGame::_this->SetMusicEnable(true);
	CGame::_this->SetSoundEnable(true);
	CGame::_this->_notificationEnable = true;
	_isRemoveAds = false;
	MAIN_GAME()->_gamePlay.InitCharFever();
	MAIN_GAME()->_stepTutorial = 0;

	Pine_Date_Time localTime;
	localTime.setDateByLocalDate();
	_currentDay = localTime.getDay();
	_numPlayADay = 0;
	_canInvitFB = true;

	SynForNewVersion(_versionFormat);	
}

void CUser::SynForNewVersion(INT_TIME fromVersion)
{
	for (INT_TIME i = fromVersion; i <= CURRENT_SAVE_VERSION; i++)
	{
		NewDataDefault(i);
	}
	_versionFormat = CURRENT_SAVE_VERSION;
}

void CUser::NewDataDefault(INT_TIME version)
{	
	switch (version)
	{
	case SAVE_GAME_VERSION_0: //version 0  1.0.0
		break;
	case SAVE_GAME_VERSION_1:
	{
		Pine_Date_Time localTime;
		localTime.setDateByLocalDate();
		_currentDay = localTime.getDay();
		_numPlayADay = 0;
		_canInvitFB = true;
	}
		break;
	}
}

void CUser::DataSave()
{
	DataToBuffer();
	OS_SaveAppData(SAVE_FILE_NAME, _buffer, MAX_SAVE_BUFFER_CACHE, false);
}

void CUser::DataLoad()
{
	int saved = 0;
	Buffer* buff = OS_LoadAppData(SAVE_FILE_NAME);
	if (buff != NULL)
	{
		saved = buff->Length();
		memcpy(_buffer, buff->Data(), saved);
		delete buff;
	}

	if (saved == 0)
	{
		DataDefault();
		DataSave();
	}
	else
	{
		DataFromBuffer();
	}
}

void CUser::DataToBuffer()
{    
	printf("\n ========= BEGIN SAVE version %d ==========", _versionFormat);

	//for version  0
	GAME()->SetInt64At(_buffer, SAVE_FORMAT_CURRENT_VERSION, _versionFormat);
	for (int i = 0; i < MAX_STRING_ID; i++)
	{
		GAME()->SetCharAt(_buffer, SAVE_FORMAT_USER_NAME + i, _userName[i]);
	}
	for (int i = 0; i < MAX_STRING_ID; i++)
	{
		GAME()->SetCharAt(_buffer, SAVE_FORMAT_USER_ID + i, _userID[i]);
	}

	GAME()->SetInt64At(_buffer, SAVE_FORMAT_PLAY_TIMES, _playTimes);
	GAME()->SetInt32At(_buffer, SAVE_FORMAT_HIGHT_SCORE, _highScore);    
    
	GAME()->SetByteAt(_buffer, SAVE_FORMAT_MUSIC, CGame::_this->IsEnableMusic());
	GAME()->SetByteAt(_buffer, SAVE_FORMAT_SOUND, CGame::_this->IsEnableSound());
	GAME()->SetByteAt(_buffer, SAVE_FORMAT_NOTIFY, CGame::_this->_notificationEnable);
    
	for (int  i = 0; i < NUM_CHAR_FEVER; i++)
	{
		GAME()->SetInt16At(_buffer, SAVE_FORMAT_CHAR_FEVER_STATE + i * 2, MAIN_GAME()->_gamePlay._charFever[i]._state);
		GAME()->SetByteAt(_buffer, SAVE_FORMAT_CHAR_FEVER_COLLECT + i * 1, MAIN_GAME()->_gamePlay._charFever[i]._isCollected);
	}
	GAME()->SetInt16At(_buffer, SAVE_FORMAT_TUTORIAL_STEP, MAIN_GAME()->_stepTutorial);

	//ver1
	GAME()->SetInt16At(_buffer, SAVE_FORMAT_NUM_PLAY_ADAY, _numPlayADay);
	GAME()->SetInt16At(_buffer, SAVE_FORMAT_CURRENT_DAY, _currentDay);
	GAME()->SetByteAt(_buffer, SAVE_FORMAT_CAN_INVITE_FB,_canInvitFB);

	printf("\n ========= END SAVE ========== \n");
	
}

void CUser::DataFromBuffer()
{   
	_versionFormat = GAME()->GetInt64At(_buffer, SAVE_FORMAT_CURRENT_VERSION);
	printf("\n ========= LOAD SAVE version %d ==========", _versionFormat);

	//for version  0
	for (int i = 0; i < MAX_STRING_ID; i++)
	{
		_userID[i] = GAME()->GetCharAt(_buffer, SAVE_FORMAT_USER_ID + i);
	}
	for (int i = 0; i < MAX_STRING_ID; i++)
	{
		_userName[i] = GAME()->GetCharAt(_buffer, SAVE_FORMAT_USER_NAME + i);
	}

	_highScore = GAME()->GetInt32At(_buffer, SAVE_FORMAT_HIGHT_SCORE);
	_playTimes = GAME()->GetInt64At(_buffer, SAVE_FORMAT_PLAY_TIMES);
    
	CGame::_this->SetMusicEnable(GAME()->GetByteAt(_buffer, SAVE_FORMAT_MUSIC));
	CGame::_this->SetSoundEnable(GAME()->GetByteAt(_buffer, SAVE_FORMAT_SOUND));
	CGame::_this->_notificationEnable = GAME()->GetByteAt(_buffer, SAVE_FORMAT_NOTIFY);	
	
    _isRemoveAds= GAME()->GetByteAt(_buffer, SAVE_FORMAT_REMOVE_ADS);

	for (int i = 0; i < NUM_CHAR_FEVER; i++)
	{
		MAIN_GAME()->_gamePlay._charFever[i]._state = GAME()->GetInt16At(_buffer, SAVE_FORMAT_CHAR_FEVER_STATE + i * 2);
		MAIN_GAME()->_gamePlay._charFever[i]._isCollected = GAME()->GetByteAt(_buffer, SAVE_FORMAT_CHAR_FEVER_COLLECT + i * 1);
	}

	MAIN_GAME()->_stepTutorial = GAME()->GetInt16At(_buffer, SAVE_FORMAT_TUTORIAL_STEP);
	//ver1
	_numPlayADay = GAME()->GetInt16At(_buffer, SAVE_FORMAT_NUM_PLAY_ADAY);
	_currentDay = GAME()->GetInt16At(_buffer, SAVE_FORMAT_CURRENT_DAY);
	_canInvitFB = GAME()->GetByteAt(_buffer, SAVE_FORMAT_CAN_INVITE_FB);

	if (_versionFormat < CURRENT_SAVE_VERSION)
	{
		SynForNewVersion(_versionFormat + 1);
		DataSave();
	}

	printf("\n ========= END LOAD ========== \n");
	
}

void CUser::ResetTimeADay()
{
	Pine_Date_Time localTime;
	localTime.setDateByLocalDate();
	if (_currentDay <localTime.getDay())
	{
		_currentDay = localTime.getDay();
		_numPlayADay = 0;
		_canInvitFB = true;
	}
}	



