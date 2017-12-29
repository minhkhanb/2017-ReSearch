#ifndef _DESIGN_UTIL_H
#define _DESIGN_UTIL_H

class  Design_Util
{
public:
	Design_Util()	{};

	
	static float getLevelDes(int index, int param)
	{
		return GDS_Exporter::level_value[index*TOTAL_LEVEL_VALUE + param];
	};

	static int getDuckDes(int index, int param)
	{
		return GDS_Exporter::duck_des[index*TOTAL_DUCK_DES + param];
	};

	static int getComboDes(int index, int param)
	{
		return GDS_Exporter::combo_des[index*TOTAL_COMBO_DES + param];
	};

	static const char* getDuckType(int index)
	{
		return GDS_Exporter::level_type[index];
	};

	static float getSpriteIDDuck(int index)
	{
		return GDS_Exporter::sprite_id_duck[index];
	};

	static COLOR getComboColor(int index)
	{
		return GDS_Exporter::combo_color[index];
	};
	
	int GetLevelByScore(INT32 score);

	static const int _spriteIDFever[5];
	
};

#endif