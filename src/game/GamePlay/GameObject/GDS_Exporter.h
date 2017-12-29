#if !_GDS_EXPORTER_H
#define _GDS_EXPORTER_H
class GDS_Exporter
{
public:
//The code generate will auto appends.
//@[ginha.do]
//Note:generate code auto

//define atriubte 
#define TOTAL_COMBO_COLOR	(1)
#define TOTAL_COMBO_COLOR_ROW	(10)

#define COMBO_COLOR_COLOR	(0)
//end define
static const COLOR combo_color[10];
//@[ginha.do]
//Note:generate code auto

//define atriubte 
#define TOTAL_COMBO_DES	(3)
#define TOTAL_COMBO_DES_ROW	(10)

#define COMBO_DES_MULTI	(0)
#define COMBO_DES_WORD_RATIO	(1)
#define COMBO_DES_SCORE_BONUS	(2)
//end define
static const int combo_des[30];
//@[ginha.do]
//Note:generate code auto

//define atriubte 
#define TOTAL_SPRITE_ID_DUCK	(1)
#define TOTAL_SPRITE_ID_DUCK_ROW	(8)

#define SPRITE_ID_DUCK_SPRITE_ID	(0)
//end define
static const int sprite_id_duck[8];
//@[ginha.do]
//Note:generate code auto

//define atriubte 
#define TOTAL_DUCK_DES	(5)
#define TOTAL_DUCK_DES_ROW	(8)

#define DUCK_DES_TYPE	(0)
#define DUCK_DES_SPEED	(1)
#define DUCK_DES_MIN_DISTANCE	(2)
#define DUCK_DES_MAX_DISTANCE	(3)
#define DUCK_DES_ANGLE	(4)
//end define
static const float duck_des[40];
//@[ginha.do]
//Note:generate code auto

//define atriubte 
#define TOTAL_LEVEL_TYPE	(1)
#define TOTAL_LEVEL_TYPE_ROW	(6)

#define LEVEL_TYPE_DUCK_TYPE	(0)
//end define
static const char* level_type[6];
//@[ginha.do]
//Note:generate code auto

//define atriubte 
#define TOTAL_LEVEL_VALUE	(8)
#define TOTAL_LEVEL_VALUE_ROW	(6)

#define LEVEL_VALUE_SCORE	(0)
#define LEVEL_VALUE_DUCK_DELAY	(1)
#define LEVEL_VALUE_DUCK_DISPLAY	(2)
#define LEVEL_VALUE_DANGEROUS_SPACE	(3)
#define LEVEL_VALUE_LANE_1	(4)
#define LEVEL_VALUE_LANE_2	(5)
#define LEVEL_VALUE_LANE_3	(6)
#define LEVEL_VALUE_LANE_4	(7)
//end define
static const float level_value[48];
};


#endif
