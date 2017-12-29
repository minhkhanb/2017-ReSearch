#if !_EFFECT_MANAGER_dddd_H
#define _EFFECT_MANAGER_dddd_H 1

#define EFFECT_MANAGER_MAX_CACHE	(100)
#define EFFECT_PATICAL_MAX_CACHE	(50)

#define EFFECT_BULL_FALL			1
#define EFFECT_COMBO				2
#define EFFECT_PAPER_PARTICAL		3
#define EFFECT_ADD_SCORE			4

#define EFFECT_MANAGER_STATE_NONE       0

struct Effect
{
	char _string[1024];
	BYTE _state, _type;
	CAnimObject _anim;

	int _var1, _var2, _var3, _var4, _var5, _var6;
	float _varF1, _varF2, _varF3, _varF4, _varF5, _varF6;
	float _x, _y;

	//patical
	BYTE _par_state[EFFECT_PATICAL_MAX_CACHE];
	CAnimObject _par_anim[EFFECT_PATICAL_MAX_CACHE];

	char _par_char[EFFECT_PATICAL_MAX_CACHE];
	float _par_x[EFFECT_PATICAL_MAX_CACHE];
	float _par_y[EFFECT_PATICAL_MAX_CACHE];
	float _par_x0[EFFECT_PATICAL_MAX_CACHE];
	float _par_y0[EFFECT_PATICAL_MAX_CACHE];

	float _par_varF1[EFFECT_PATICAL_MAX_CACHE];
	float _par_varF2[EFFECT_PATICAL_MAX_CACHE];
	float _par_varF3[EFFECT_PATICAL_MAX_CACHE];
	float _par_varF4[EFFECT_PATICAL_MAX_CACHE];
	float _par_varF5[EFFECT_PATICAL_MAX_CACHE];
	float _par_varF6[EFFECT_PATICAL_MAX_CACHE];
	float _par_varF7[EFFECT_PATICAL_MAX_CACHE];

	int _par_var1[EFFECT_PATICAL_MAX_CACHE];
	int _par_var2[EFFECT_PATICAL_MAX_CACHE];
	int _par_var3[EFFECT_PATICAL_MAX_CACHE];
	int _par_var4[EFFECT_PATICAL_MAX_CACHE];
	int _par_var5[EFFECT_PATICAL_MAX_CACHE];
	int _par_var6[EFFECT_PATICAL_MAX_CACHE];
	int _par_var7[EFFECT_PATICAL_MAX_CACHE];
};
class CEffectManager
{

public:
	static bool isMoveToTargetSpeed(float &x1, float &y1, float x2, float y2, float &speed);
	static bool isMoveToTarget(float &x1, float &y1, float x2, float y2, float speed);
	static bool isMoveToTarget(float &x, float &y, float targetX, float targetY, float offX, float offY, bool left, float speed);
	CEffectManager(){};

	Effect _effect[EFFECT_MANAGER_MAX_CACHE];
	bool CompleteStatePartical(int i, int num, BYTE state);
	void Init();

	//main
	void Update();
	void Render();
	void Render(BYTE type);

	bool CompleteEffect(BYTE type);
	bool CompleteEffect();
	void StopEffect(BYTE type);
	void SetStateEffect(BYTE type,BYTE state);

	void AddEffectBullFall(int x, int y, float speed);
	void RenderEffectBullFall(int i);
	void UpdateEffectBullFall(int i);

	void AddEffectCombo(int x, int y, int combo, int deplay);
	void RenderEffectCombo(int i);
	void UpdateEffectCombo(int i);

	void AddEffectPaperPartical(int x, int y, float angle, float speed);
	void RenderEffectPaperPartical(int i);
	void UpdateEffectPaperPartical(int i);

	void AddEffectAddScore(int x, int y, bool mull ,int score, float speed, float speedOpa);
	void RenderEffectAddScore(int i);
	void UpdateEffectAddScore(int i);

};

#endif