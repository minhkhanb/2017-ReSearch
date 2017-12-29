#include "pch.h"
#include "../../GameCore/CGame.h"


int Design_Util::GetLevelByScore(INT32 score)
{
	int res = 0;
	for (int  i = 0; i < TOTAL_LEVEL_VALUE_ROW; i++)
	{
		int scoreDES = getLevelDes(i, LEVEL_VALUE_SCORE);
		if (score >= scoreDES)
		{
			res = i + 1;
		}
	}
	if (res > TOTAL_LEVEL_VALUE_ROW - 1)
	{
		res = TOTAL_LEVEL_VALUE_ROW - 1;
	}
	return res;
}

const int Design_Util::_spriteIDFever[5] =
{
	SPRITEID_OBJECT_DUCK_FEVER_F,
	SPRITEID_OBJECT_DUCK_FEVER_E,
	SPRITEID_OBJECT_DUCK_FEVER_V,
	SPRITEID_OBJECT_DUCK_FEVER_E2,
	SPRITEID_OBJECT_DUCK_FEVER_R,
};