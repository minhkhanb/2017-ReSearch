#if !defined(_H_CGAME_STATE)
#define _H_CGAME_STATE

class CGameState
{
public:
	CGameState(){};
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual bool UpdateTouchButton() = 0;
	virtual void UpdateTouch() = 0;
	virtual void Init() = 0;
	int _gameStateOld;
};

#endif