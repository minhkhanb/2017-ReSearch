//
//  BugerGame.h
//  PineEntertainment
//
//  Created by Do Thanh Nghia on 12/26/14.
//
//

#include "../GameCore/PineGame.h"
#include <stdio.h>

#ifndef _SHAKE_DUCK_OFF_H
#define _SHAKE_DUCK_OFF_H

#define SYN_SAVE_NONE (0)
#define SYN_SAVE_PROCESSING (1)
#define TIMES_CHARBOST         3

#define MAX_STEP_TUTORIAL	100
class ShakeDuckOffGame:public PineGame
{
public:
    
    void UpdateAndRender();
    void Init();
    
	void SetClipMatrix(SMatrix4x4 tranf, float clipX, float clipY, float clipW, float clipH);
	void GetRectTranform(SMatrix4x4 tranf, GameRect inRect, GameRect &outRect);

	bool CheckRectOverlapCircle(int x, int y, int w, int h, int cX, int cY, int R);
	bool CheckCircleOverlapCircle(int cX1, int cY1, int R1, int cX2, int cY2, int R2);
	bool checkRectOverlapRect(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2);

    int _check_syn_save_complete;
    void CheckAndSynSave();
    int  _syn_save_state;
    int  _syn_save_step;
    int  _syn_time_out;
    bool UpdateSynSave();

    bool _can_submit;
    INT32 _online_score;
	CAnimObject _animLoading;
	BYTE _isProcessingRevive;
	enum enumTypeProcessing
	{
		REVIVE_NONE,
		REVIVE_PROCESSING_ADS,
		REVIVE_PROCESSING_LOGIN_FB,
		REVIVE_PROCESSING_INVITE_FB,
	};
    INT32 _countTimeDeplay;
	///
	Design_Util _design;
	CEffectManager _effect;
	CGamePlay _gamePlay;
	CGameStateLogo _gameStateLogo;
	CGameStateMainMenu _gameStateMainMenu;
	CGameStateGamePlay _gameStateGamePlay;
	CUser _user;
    int _timesCharbost;

	CAnimObject _animTutorial;
	
	int _stepTutorial;
	bool IsTutorialComplete();
	void InitTutorial();
	void RenderTutorial();

    PineSocial *_fbSosial;
    int _stateLoginFB;
    enum enumStateLoginFB
    {
        LOGIN_FB_NONE,
        LOGIN_FB_PROCESSING,
        LOGIN_FB_COMPLETE,
    };
};

#endif /* defined(__PineEntertainment__BugerGame__) */
