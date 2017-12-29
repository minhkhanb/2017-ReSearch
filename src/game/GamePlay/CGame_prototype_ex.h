/**
*	will be included in CGame.h
*	specific vars and methods for gameplay
*/

public:
// game state
	void UpdateLogo();
	void RenderLogo();
private:
	
public:



	

//Save System.
void GameDataLoad();
void GameDataSave();
void GameDataDefault();
void GameDataToBuffer();
void GameLoadDataFromBuffer();

BYTE red, green, blue, alpha;
#define GAME_FILE	"pinerecord.dat"

#define GAME_VERSION				100

#define GAME_DATA_VERSION			(0)

#define GAME_DATA_LENGTH				(GAME_DATA_VERSION + 4)



char _buffer_save[GAME_DATA_LENGTH];
int _game_version;
int _rate;

//PineHighScore _pineHighScore;

 long _sys_begin_second;
 long _sys_millisecond;
 long _game_millisecond;
 long _sys_second;

void UnloadGameBackground();
bool LoadGameForceground();

bool _needresume_when_playing;
void PurchaseIAP(const char* strID, bool sucessed);
int  _frame_auto_stop;
int _stateInviteFriend;
PineGame *_main_game;
PineGame *_feature_game;
PineGame *_current_game;

////////
void SetClipMatrix(SMatrix4x4 tranf, float clipX, float clipY, float clipW, float clipH);
void GetRectTranform(SMatrix4x4 tranf, GameRect inRect, GameRect &outRect);

bool CheckRectOverlapCircle(int x, int y, int w, int h, int cX, int cY, int R);
bool CheckCircleOverlapCircle(int cX1, int cY1, int R1, int cX2, int cY2, int R2);
bool checkRectOverlapRect(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2);
