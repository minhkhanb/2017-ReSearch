#include "pch.h"
#include "../../GameCore/CGame.h"

void CEffectManager::Init()
{
	for (int i = 0; i < EFFECT_MANAGER_MAX_CACHE; i++)
	{
		_effect[i]._state = EFFECT_MANAGER_STATE_NONE;
	}
}

bool CEffectManager::CompleteEffect(BYTE type)
{
	for (int i = 0; i < EFFECT_MANAGER_MAX_CACHE; i++)
	{
		if (_effect[i]._type== type && _effect[i]._state != EFFECT_MANAGER_STATE_NONE)
		{
			return false;
		}
	}
	return true;
}

bool CEffectManager::CompleteEffect()
{
	for (int i = 0; i < EFFECT_MANAGER_MAX_CACHE; i++)
	{
		if (_effect[i]._state != EFFECT_MANAGER_STATE_NONE)
		{
			return false;
		}
	}
	return true;
}
void CEffectManager::SetStateEffect(BYTE type, BYTE state)
{
	for (int i = 0; i < EFFECT_MANAGER_MAX_CACHE; i++)
	{
		if (_effect[i]._type == type)
		{
			_effect[i]._state = state;
		}
	}	
}
void CEffectManager::StopEffect(BYTE type)
{
	for (int i = 0; i < EFFECT_MANAGER_MAX_CACHE; i++)
	{
		if (_effect[i]._type == type && _effect[i]._state != EFFECT_MANAGER_STATE_NONE)
		{
			_effect[i]._state = EFFECT_MANAGER_STATE_NONE;
		}
	}

}
bool CEffectManager::CompleteStatePartical(int i, int num, BYTE state)
{
	bool res = true;
	for (int j = 0; j < num; j++)
	{
		if (_effect[i]._par_state[j] != state)
		{
			res = false;
			break;
		}
	}
	return res;
}

bool CEffectManager::isMoveToTargetSpeed(float &x, float &y, float targetX, float targetY, float &speed)
{
	bool ret = false;
	float distance = CMath::DistancePOW2(targetX, targetY, x, y);
	if (distance < (speed * speed))
	{
		targetX = x;
		targetY = y;
		ret = true;
	}
	else
	{
		float angle_move = CMath::ANGLE(targetX, targetY, x, y) - 180;
		float vy = speed*MSIN(angle_move);
		float vx = speed*MCOS(angle_move);
		x += vx;
		y += vy;
		speed *= 1.05;
	}
	return ret;
}
bool CEffectManager::isMoveToTarget(float &x, float &y, float targetX, float targetY, float speed)
{
	bool ret = false;
	float distance = CMath::DistancePOW2(targetX, targetY, x, y);
	if (distance < (speed * speed))
	{
		targetX = x;
		targetY = y;
		ret = true;
	}
	else
	{
		float angle_move = CMath::ANGLE(targetX, targetY, x, y) - 180;
		float vy = speed*MSIN(angle_move);
		float vx = speed*MCOS(angle_move);
		x += vx;
		y += vy;

	}
	return ret;
}

bool CEffectManager::isMoveToTarget(float &x, float &y, float targetX, float targetY, float offX, float offY, bool left,float speed)
{
	bool ret = false;
	float distance = CMath::DistancePOW2(targetX, targetY, x, y);
	if (distance < (speed * speed))
	{
		targetX = x;
		targetY = y;
		ret = true;
	}
	else
	{
		float angle_move = CMath::ANGLE(targetX, targetY, x, y) - 180;
		float vy = speed*MSIN(angle_move);
		float vx = speed*MCOS(angle_move);


		float del_x = speed * offX;
		float del_y = speed * offY;

	
		{
			x += vx + del_x;
		}
		y += vy +del_y;



	}
	return ret;
}



void CEffectManager::Update()
{
	for (int i = 0; i < EFFECT_MANAGER_MAX_CACHE; i++)
	{
		if (_effect[i]._state != EFFECT_MANAGER_STATE_NONE)
		{
			switch (_effect[i]._type)
			{
			case EFFECT_BULL_FALL:
				UpdateEffectBullFall(i);
				break;

			case EFFECT_COMBO:
				UpdateEffectCombo(i);
				break;

			case EFFECT_PAPER_PARTICAL:
				UpdateEffectPaperPartical(i);
				break;

			case EFFECT_ADD_SCORE:
				UpdateEffectAddScore(i);
				break;
			}
		}
	}
}
void CEffectManager::Render()
{
	for (int i = 0; i < EFFECT_MANAGER_MAX_CACHE; i++)
	{
		if (_effect[i]._state != EFFECT_MANAGER_STATE_NONE)
		{
			switch (_effect[i]._type)
			{
			case EFFECT_BULL_FALL:
				RenderEffectBullFall(i);
				break;

			case EFFECT_COMBO:
				RenderEffectCombo(i);
				break;

			case EFFECT_ADD_SCORE:
				RenderEffectAddScore(i);
				break;

			}
		}
	}
}
void CEffectManager::Render(BYTE type)
{
	for (int i = 0; i < EFFECT_MANAGER_MAX_CACHE; i++)
	{
		if (_effect[i]._state != EFFECT_MANAGER_STATE_NONE && _effect[i]._type == type)
		{
			switch (_effect[i]._type)
			{
			case EFFECT_BULL_FALL:
				RenderEffectBullFall(i);
				break;

			case EFFECT_COMBO:
				RenderEffectCombo(i);
				break;

			case EFFECT_PAPER_PARTICAL:
				RenderEffectPaperPartical(i);
				break;
			}
		}
	}
}


///
void CEffectManager::AddEffectBullFall(int x, int y, float speed)
{
	for (int i = 0; i < EFFECT_MANAGER_MAX_CACHE; i++)
	{
		if (_effect[i]._state == EFFECT_MANAGER_STATE_NONE)
		{
			_effect[i]._type = EFFECT_BULL_FALL;
			_effect[i]._state = 1;
			_effect[i]._x = x;
			_effect[i]._y = y;
			_effect[i]._var1 = 1;
			
			_effect[i]._var2 = SPRITEID_CHAR_DOG_01;

			for (int j = 0; j < _effect[i]._var1; j++)
			{
				_effect[i]._par_x0[j] = _effect[i]._x + CMath::RANDOM(-20, 20);
				_effect[i]._par_y0[j] = _effect[i]._y + CMath::RANDOM(-20, 20);
				_effect[i]._par_varF4[j] = k_SCREEN_HEIGHT;

				_effect[i]._par_varF1[j] = 70 + CMath::RANDOM(-10, 20);
				_effect[i]._par_varF2[j] = 20 + CMath::RANDOM(-5, 0);
				_effect[i]._par_var5[j] = 0;

				_effect[i]._par_varF3[j] = CMath::RANDOM(70, 90);

				_effect[i]._par_state[j] = 1;
				_effect[i]._par_var4[j] = 17 ;

				_effect[i]._par_var1[j] = 200;
				_effect[i]._par_var2[j] = 0;

				_effect[i]._par_var3[j] = CMath::RANDOM(0, 1) ? _LEFT : _RIGHT;

			}
			break;
		}
	}
}
void CEffectManager::RenderEffectBullFall(int i)
{
	for(int j = 0; j< _effect[i]._var1; j++)
	{
		if (_effect[i]._par_state[j] != EFFECT_MANAGER_STATE_NONE)
		{
			G()->SetOpacity(_effect[i]._par_var1[j]);
			G()->SetRotate(_effect[i]._par_var1[j]/10, _effect[i]._par_x[j], _effect[i]._par_y[j]);
			GET_SPRITE(_effect[i]._var2)->DrawFrame(G(), _effect[i]._par_var4[j], _effect[i]._par_x[j], _effect[i]._par_y[j]);
			G()->ClearRotate();
			G()->SetOpacity(100);
		}
	}
}
void CEffectManager::UpdateEffectBullFall(int i)
{
	switch (_effect[i]._state)
	{
	case 1:
		for (int j = 0; j < _effect[i]._var1; j++)
		{
			switch (_effect[i]._par_state[j])
			{
			case 1:
			{
				_effect[i]._par_var5[j] += 12;// DT();

				float ft = _effect[i]._par_var5[j] / 40.0f;
				float x = _effect[i]._par_varF1[j] * CMath::COS(_effect[i]._par_varF3[j])*ft;
				float y = _effect[i]._par_varF1[j] * CMath::SIN(_effect[i]._par_varF3[j])*ft - _effect[i]._par_varF2[j] * ft*ft*0.5f;


				if (_effect[i]._par_var3[j] == _RIGHT)
				{
					_effect[i]._par_x[j] = _effect[i]._par_x0[j] - x;
				}
				else
				{
					_effect[i]._par_x[j] = _effect[i]._par_x0[j] + x;
				}
				_effect[i]._par_y[j] = _effect[i]._par_y0[j] - y;


				_effect[i]._par_var1[j] -= 1;
				if (_effect[i]._par_var1[j] <= 0)
				{

				}
				if (_effect[i]._par_y[j] > _effect[i]._par_varF4[j])
				{
					_effect[i]._par_var2[j]++;
					_effect[i]._par_varF2[j] = 25;
					_effect[i]._par_var5[j] = 0;
					_effect[i]._par_varF1[j] *= 0.8;
					_effect[i]._par_y0[j] = _effect[i]._par_y[j];
					_effect[i]._par_x0[j] = _effect[i]._par_x[j];

				}

				if (_effect[i]._par_var2[j] > 1)
				{
					_effect[i]._par_state[j] = EFFECT_MANAGER_STATE_NONE;
				}



				if (CompleteStatePartical(i, _effect[i]._var1, EFFECT_MANAGER_STATE_NONE))
				{
					_effect[i]._state = EFFECT_MANAGER_STATE_NONE;
				}

			}
				break;
			}
		}
		break;
	}
}

void CEffectManager::AddEffectCombo(int x, int y, int combo, int deplay)
{
	for (int i = 0; i < EFFECT_MANAGER_MAX_CACHE; i++)
	{
		if (_effect[i]._state == EFFECT_MANAGER_STATE_NONE)
		{
			_effect[i]._type = EFFECT_COMBO;
			_effect[i]._state = 1;
			_effect[i]._x = x;
			_effect[i]._y = y;
			_effect[i]._var1 = combo;
			_effect[i]._var3 = deplay;

			_effect[i]._var2 = 0;//opa
			_effect[i]._varF1 = 0;//scale
			if (combo < 2)
			{
				sprintf(_effect[i]._string,"%s" ,"COMBO");
			}
			else
			{
				sprintf(_effect[i]._string, "COMBOx%d", combo);
			}			
			_effect[i]._var4 = strlen(_effect[i]._string); //num char

			_effect[i]._var5 = 0;//ww string
			for (int j = 0; j < _effect[i]._var4; j++)
			{
				int index = _effect[i]._string[_effect[i]._var4 - 1 - j] - 33;
				int ww = GET_SPRITE(SPRITEID_FONT_COMBO)->GetFModuleWidth(index, 0);

				if (index == -1)
				{
					_effect[i]._var5 += 10;
				}
				else
				{
					_effect[i]._var5 +=  ww;
				}
			}
			int BeginX = x + _effect[i]._var5 / 2;
			for (int j = 0; j < _effect[i]._var4; j++)
			{
				int index = _effect[i]._string[_effect[i]._var4 - 1 - j] - 33;
				int ww = Button::GetFrameWidth(GET_SPRITE(SPRITEID_FONT_COMBO), index);// ->GetFModuleWidth(index, 0);
				
				printf("\n char = %c, ww = %d", _effect[i]._string[_effect[i]._var4 - 1 - j], ww);
				_effect[i]._par_x[j] = BeginX;
          		if (index == -1)
				{
					BeginX += 10;
					index = 0;
				}
				else
				{
					BeginX -= ww;
				}
				
				_effect[i]._par_var1[j] = index;
			}
		
			break;
		}
	}
}
void CEffectManager::RenderEffectCombo(int i)
{
	
	
	G()->SetOpacity(_effect[i]._var2);
	G()->SetScale(_effect[i]._varF1, _effect[i]._varF1, _effect[i]._x, _effect[i]._y);
	int combo = _effect[i]._var1;	
	if (combo >= TOTAL_COMBO_COLOR_ROW)
	{
		combo = TOTAL_COMBO_COLOR_ROW - 1;
	}
	COLOR color = MAIN_GAME()->_design.getComboColor(combo);

	int BeginX = _effect[i]._x + _effect[i]._var5 / 2;
	for (int j = 0; j < _effect[i]._var4; j++)
	{
		int ww = Button::GetFrameWidth(GET_SPRITE(SPRITEID_FONT_COMBO), _effect[i]._par_var1[j]);// ->GetFModuleWidth(index, 0);
		BeginX -= ww - ww*0.1;
		GET_SPRITE(SPRITEID_FONT_COMBO_SHADOW)->DrawFrame(G(), _effect[i]._par_var1[j], BeginX, _effect[i]._y);
		G()->SetColorMask(color);
		GET_SPRITE(SPRITEID_FONT_COMBO)->DrawFrame(G(), _effect[i]._par_var1[j], BeginX, _effect[i]._y);
		G()->ClearColorMask();
	}

	//GET_SPRITE(SPRITEID_FONT_COMBO)->DrawAnsiTextFormat(G(), _effect[i]._x, _effect[i]._y, _HCENTER | _VCENTER, _effect[i]._string);
	
	G()->ClearScale();
	G()->SetOpacity(100);
}
void CEffectManager::UpdateEffectCombo(int i)
{
	//_effect[i]._y--;
	switch (_effect[i]._state)
	{
	case 1:
		_effect[i]._var3--;
		if (_effect[i]._var3 <= 0)
		{
			_effect[i]._state = 2;

			int combo = _effect[i]._var1;
			if (combo >= TOTAL_COMBO_COLOR_ROW)
			{
				combo = TOTAL_COMBO_COLOR_ROW;
			}
			int bonus = MAIN_GAME()->_design.getComboDes(combo - 1, COMBO_DES_SCORE_BONUS);
			if (bonus > 0)
			{
				AddEffectAddScore(_effect[i]._x, _effect[i]._y - 70, true,bonus+1, 1.5, 3);
			}
		}
		break;

	case 2:
		_effect[i]._var2 += 5;
		_effect[i]._varF1 += 0.1;
		if (_effect[i]._varF1 >= 1.2)
		{
			_effect[i]._state = 3;
		}
		break;

	case 3:
		_effect[i]._var2 += 5;
		_effect[i]._varF1 -= 0.01;
		if (_effect[i]._varF1 <= 1)
		{
			_effect[i]._varF1 = 1;			
			_effect[i]._state = EFFECT_MANAGER_STATE_NONE;
		}
		break;

	
	}
}

void CEffectManager::AddEffectPaperPartical(int x, int y, float angle, float speed)
{
	for (int i = 0; i < EFFECT_MANAGER_MAX_CACHE; i++)
	{
		if (_effect[i]._state == EFFECT_MANAGER_STATE_NONE)
		{
			_effect[i]._type = EFFECT_PAPER_PARTICAL;
			_effect[i]._state = 1;
			_effect[i]._x = x;
			_effect[i]._y = y;
			_effect[i]._var1 = CMath::RANDOM(13, 18);//frame
			_effect[i]._var2 = 0;//count time
			_effect[i]._varF1 = angle;
			_effect[i]._varF2 = speed;
			_effect[i]._varF3 = CMath::RANDOM(0,1)?1:-1;//rota
			_effect[i]._varF4 = CMath::RANDOM(50, 100) *0.01;//scale


			_effect[i]._var3 = 20;
			for (int j = 0; j < _effect[i]._var3; j++)
			{
				_effect[i]._par_state[j] = EFFECT_MANAGER_STATE_NONE;
			}
			break;
		}
	}
}
void CEffectManager::RenderEffectPaperPartical(int i)
{
	for (int j = 0; j < _effect[i]._var3; j++)
	{
		if (_effect[i]._par_state[j] != 0)
		{
			G()->SetOpacity(_effect[i]._par_var1[j]);
			G()->SetRotate(_effect[i]._par_varF1[j], _effect[i]._par_x[j], _effect[i]._par_y[j]);
			G()->SetScale(_effect[i]._par_varF2[j], _effect[i]._par_varF2[j], _effect[i]._par_x[j], _effect[i]._par_y[j]);
			GET_SPRITE(SPRITEID_HUD_FEVER)->DrawFrame(G(), _effect[i]._var1, _effect[i]._par_x[j], _effect[i]._par_y[j]);
			G()->ClearScale();
			G()->ClearRotate();
			G()->SetOpacity(100);
		}
	}


	G()->SetRotate(_effect[i]._varF3, _effect[i]._x, _effect[i]._y);
	G()->SetScale(_effect[i]._varF4, _effect[i]._varF4, _effect[i]._x, _effect[i]._y);
	GET_SPRITE(SPRITEID_HUD_FEVER)->DrawFrame(G(), _effect[i]._var1, _effect[i]._x, _effect[i]._y);
	G()->ClearScale();
	G()->ClearRotate();
}
void CEffectManager::UpdateEffectPaperPartical(int i)
{
	_effect[i]._var2++;
	switch (_effect[i]._state)
	{
	case 1:
		_effect[i]._varF4 += _effect[i]._varF2*0.001;
		_effect[i]._x -= _effect[i]._varF2 * CMath::COS(_effect[i]._varF1);
		_effect[i]._y -= _effect[i]._varF2 * CMath::SIN(_effect[i]._varF1);
		if (!CHECK_POINT_IN_RECT(_effect[i]._x, _effect[i]._y,0,0,k_SCREEN_WIDTH,k_SCREEN_HEIGHT))
		{
			_effect[i]._state = 2;
		}
		//add partical
		if (_effect[i]._var2 % 3 == 0)
		{
			int count = 0;
			for (int j = 0; j < _effect[i]._var3; j++)
			{
				if (_effect[i]._par_state[j] == EFFECT_MANAGER_STATE_NONE)
				{
					_effect[i]._par_state[j] = 1;
					_effect[i]._par_var1[j] = 100;//opacity
					_effect[i]._par_varF1[j] = CMath::RANDOM(0, 1) ? 1 : -1;//rota
					_effect[i]._par_varF2[j] = _effect[i]._varF4*0.2;//scale
					_effect[i]._par_x[j] = _effect[i]._x + CMath::RANDOM(-8, 8);
					_effect[i]._par_y[j] = _effect[i]._y + CMath::RANDOM(-8, 8);
					count++;
					if (count >= 2)
					{
						break;
					}
					
				}
			}
		}
		//update partical
		for (int j = 0; j < _effect[i]._var3; j++)
		{
			switch (_effect[i]._par_state[j])
			{
			case 1:
				_effect[i]._par_var1[j] -= 5;
				if (_effect[i]._par_var1[j] <= 0)
				{
					_effect[i]._par_state[j] = EFFECT_MANAGER_STATE_NONE;
				}
				break;
			}


			if (_effect[i]._par_varF1[j] > 0)
			{
				_effect[i]._par_varF1[j] += 5;
				if (_effect[i]._par_varF1[j] >= 360)
				{
					_effect[i]._par_varF1[j] -= 360;
				}
			}
			else
			{
				_effect[i]._par_varF1[j] -= 5;
				if (_effect[i]._par_varF1[j] <= -360)
				{
					_effect[i]._par_varF1[j] += 360;
				}
			}
		}
		
		break;

	case 2:
		_effect[i]._state = EFFECT_MANAGER_STATE_NONE;
		break;
	}

	if (_effect[i]._varF3 > 0)
	{
		_effect[i]._varF3 += 5;
		if (_effect[i]._varF3 >= 360)
		{
			_effect[i]._varF3 -= 360;
		}
	}
	else
	{
		_effect[i]._varF3 -= 5;
		if (_effect[i]._varF3 <= -360)
		{
			_effect[i]._varF3 += 360;
		}
	}
}


void CEffectManager::AddEffectAddScore(int x, int y, bool mull , int score, float speed, float speedOpa)
{
	for (int i = 0; i < EFFECT_MANAGER_MAX_CACHE; i++)
	{
		if (_effect[i]._state == EFFECT_MANAGER_STATE_NONE)
		{
			_effect[i]._type = EFFECT_ADD_SCORE;
			_effect[i]._state = 1;
			_effect[i]._x = x;
			_effect[i]._y = y;
			_effect[i]._var1 = score;//score
			_effect[i]._var2 = 0;
			_effect[i]._varF1 = speedOpa;
			_effect[i]._varF3 = speed;
			_effect[i]._varF2 = 1;

			_effect[i]._var5 = mull;
			break;
		}
	}
}
void CEffectManager::RenderEffectAddScore(int i)
{	
	G()->SetOpacity(_effect[i]._var2);
	G()->SetScale(_effect[i]._varF2, _effect[i]._varF2, _effect[i]._x, _effect[i]._y);
	if (_effect[i]._var5)
	{
		GET_SPRITE(SPRITEID_FONT_FEVERSCORE)->DrawAnsiTextFormat(G(), _effect[i]._x, _effect[i]._y, _HCENTER | _VCENTER, "x%d", _effect[i]._var1);
	}
	else
	{
		GET_SPRITE(SPRITEID_FONT_FEVERSCORE)->DrawAnsiTextFormat(G(), _effect[i]._x, _effect[i]._y, _HCENTER | _VCENTER, "+%d", _effect[i]._var1);
	}	
	G()->ClearScale();
	G()->SetOpacity(100);
}
void CEffectManager::UpdateEffectAddScore(int i)
{
	_effect[i]._y -= _effect[i]._varF3;
	switch (_effect[i]._state)
	{
	case 1:
		_effect[i]._var2 += _effect[i]._varF1;
		if (_effect[i]._var2 >= 120)
		{
			_effect[i]._state = 2;
			PLAY_SFX(SFX_COMBO);
		}
		break;

	case 2:
		_effect[i]._varF2 -= 0.01;
		_effect[i]._var2 -= _effect[i]._varF1*2;
		if (_effect[i]._var2 <= 0)
		{
			_effect[i]._state = EFFECT_MANAGER_STATE_NONE;
		}
		break;
	}
}
