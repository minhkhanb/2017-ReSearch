#include "pch.h"
#include "../../GameCore/CGame.h"
#include "../../GameCore/Os_Functions.h"

#undef _DEC_BUTTON_NUMS
#undef _DEC_BUTTON_ARRAY
#undef _DEC_ANIM_BUTTON_BEIGN
#undef _DEC_SPRITE_BUTTON

#define _DEC_BUTTON_NUMS				(MAINMENU_BUTTON_NUMS)
#define _DEC_BUTTON_ARRAY				_button
#define _DEC_ANIM_BUTTON_BEGIN			4
#define _DEC_SPRITE_BUTTON				SPRITEID_HUD_BAR

void CGameStateStore::Init()
{
	/*for (int i = 0; i < _DEC_BUTTON_NUMS; i++)
	{
	_DEC_BUTTON_ARRAY[i].Init(BUTTON_STATE_NORMAL, 0, 0, A_HVCENTER, -1);
	}*/
}
void CGameStateStore::Update()
{
	if (MAIN_GAME()->_sub_state == k_LOOP)
	{
		if (!UpdateTouchButton())
		{
			UpdateTouch();
		}
	}

}
void CGameStateStore::Render()
{
	if (MAIN_GAME()->_sub_state == k_INIT)
	{
		Init();
		MAIN_GAME()->_sub_state = k_LOOP;
	}
	if (MAIN_GAME()->_sub_state == k_LOOP)
	{
		G()->SetColor(0xaa000000);
		G()->FillFullScreen(true);
		GET_SPRITE(SPRITEID_FONT_SCORE)->DrawAnsiTextFormat(G(),k_SCREEN_WIDTH/2,k_SCREEN_HEIGHT/2,_HCENTER,"11");
		/*for (int i = 0; i < _DEC_BUTTON_NUMS; i++)
		{
		_DEC_BUTTON_ARRAY[i].Render(G());
		_DEC_BUTTON_ARRAY[i].Update();
		}*/
	}
}
bool CGameStateStore::UpdateTouchButton()
{
	return false;
	bool ret = false;
	int index = -1;
	for (int i = 0; i < _DEC_BUTTON_NUMS; i++)
	{
		if (_DEC_BUTTON_ARRAY[i]._state == BUTTON_STATE_ACTIVE)
		{
			ret = true;
			index = i;
			break;
		}
	}
	if (ret)
	{
		if (_DEC_BUTTON_ARRAY[index].NeedAction())
		{
			switch (index)
			{
			case MAINMENU_BUTTON_PLAY:
				break;
			case MAINMENU_BUTTON_STORE:
				break;
			}

			for (int i = 0; i < _DEC_BUTTON_NUMS; i++)
			{
				if (_DEC_BUTTON_ARRAY[i]._state != BUTTON_STATE_LOCK && _DEC_BUTTON_ARRAY[i]._state != BUTTON_STATE_HIDE)
				{
					_DEC_BUTTON_ARRAY[i].SetState(BUTTON_STATE_NORMAL);
				}
			}
			CGame::_this->ClearAllTouch();
		}
		return true;
	}
	bool pressed = false;
	for (int i = CGame::_this->_num_touches - 1; i >= 0; i--)
	{
		if (TOUCHS(i)->_flag)
		{
			for (int touch = 0; touch < _DEC_BUTTON_NUMS; touch++)
			{
				if (_DEC_BUTTON_ARRAY[touch]._state == BUTTON_STATE_LOCK || _DEC_BUTTON_ARRAY[touch]._state == BUTTON_STATE_HIDE)
				{
					continue;
				}

				int x = _DEC_BUTTON_ARRAY[touch].TZ_GetX() - 30;
				int y = _DEC_BUTTON_ARRAY[touch].TZ_GetY() - 30;
				int w = _DEC_BUTTON_ARRAY[touch].TZ_GetWidth() + 60;
				int h = _DEC_BUTTON_ARRAY[touch].TZ_GetHeight() + 60;
				Button *button = &_DEC_BUTTON_ARRAY[touch];
				if (CHECK_POINT_IN_RECT(TOUCHS(i)->x, TOUCHS(i)->y, x, y, w, h))
				{
					pressed = true;
					if (TOUCHS(i)->_state == TOUCH_DOWN)
					{
						if (button->_state != BUTTON_STATE_SELECT && Button::GetButtonActive() == NULL)
						{


							button->SetState(BUTTON_STATE_SELECT);
						}
					}
					else if (TOUCHS(i)->_state == TOUCH_DRAG)
					{
						if (button->_state != BUTTON_STATE_SELECT && Button::GetButtonActive() == NULL)
						{
							button->SetState(BUTTON_STATE_SELECT);
						}
					}
					else if (TOUCHS(i)->_state == TOUCH_UP)
					{
						button->SetState(BUTTON_STATE_ACTIVE);
						TOUCHS(i)->_flag = false;
					}
				}
				else
				{
					if (button->_state == BUTTON_STATE_SELECT)
					{
						for (int j = 0; j < _DEC_BUTTON_NUMS; j++)
						{
							if (_DEC_BUTTON_ARRAY[j]._state != BUTTON_STATE_LOCK && _DEC_BUTTON_ARRAY[j]._state != BUTTON_STATE_HIDE)
							{
								_DEC_BUTTON_ARRAY[j].SetState(BUTTON_STATE_NORMAL);
							}
						}
						CGame::_this->ClearAllTouch();
					}

				}
			}
		}
	}
	if (pressed == false)
	{
		return false;
	}
	else
	{
		return true;
	}
}
void CGameStateStore::UpdateTouch()
{
	for (int i = CGame::_this->_num_touches - 1; i >= 0; i--)
	{
		if (TOUCHS(i)->_flag)
		{
			if (TOUCHS(i)->_state == TOUCH_DOWN)
			{

			}
			if (TOUCHS(i)->_state == TOUCH_DRAG)
			{

			}
			if (TOUCHS(i)->_state == TOUCH_UP)
			{				

				GAME()->ClearAllTouch();
			}
		}
	}
}