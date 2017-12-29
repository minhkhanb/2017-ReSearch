#include "pch.h"
#include "MenuSystem.h"
#include "../../GameCore/CGame.h"
#define DEBUG_BUTTON 0
Button* Button::_bt_actived = NULL;
int Button::_num_button = 0;
Button** Button::_button_namager = NULL;
void Button::InitButtonManager()
{
	_num_button = 0;
	_button_namager = new Button*[10000];
}
void Button::AddButton(Button* button)
{
	for (int i = 0; i < _num_button; i++)
	{
		if (button == _button_namager[i])
		{
			return;
		}
	}
	_button_namager[_num_button] = button;
	_num_button++;
}
void Button::DrawFrameWithScale(CSprite* sprite, int frame_id, float scalex, float scaley, int xx, int yy)
{
	float posX = Button::GetFrameX(sprite, frame_id) + xx;
	float posY = Button::GetFrameY(sprite, frame_id) + yy;
	float w = Button::GetFrameWidth(sprite, frame_id);
	float h = Button::GetFrameHeight(sprite, frame_id);
	sprite->DrawFrame(G(), frame_id, posX*scalex - posX + (w*scalex - w) / 2, posY*scaley - posY + (h*scalex - h) / 2);
}
void Button::SetPosScale(float scalex, float scaley)
{
	_pos_scalex = scalex;
	_pos_scaley = scaley;
}
Button::Button()
{
	_anim = new CAnimObject();
	_anim_overplay = new CAnimObject();
	_anim_lock = new CAnimObject();
	_posX = 0;
	_posY = 0;
	_anchor = A_NORMAL;
	_extWidth = 0;
	_extHeight = 0;
	_state = BUTTON_STATE_NORMAL;
}

void Button::Init(int state, float x, float y, int anchor, int soundID, BYTE typeEffect, int timeDlay)
{
	_posX = x;
	_posY = y;
	_anchor = anchor;
	_state = state;
	_pos_scalex = 1;
	_pos_scaley = 1;

	_soundID = soundID;

	_offX = 0;
	_offY = 0;
	_scaleEffectX = 1;
	_scaleEffectY = 1;
	_typeEffect = typeEffect;
	_stateEffet = 0;
	_timeDelay = timeDlay;
}
/*
void Button::SetAnim(CSprite *sprite,int lockID, int normalID, int selectID, int activeID)
{
_anim->SetAnimSprite(sprite);
_anim_overplay->SetAnimSprite(sprite);
_lockID = lockID;
_normalID = normalID;
_selectID = selectID;
_activeID = activeID;
_anim_overplay->SetAnim(activeID);
SetState(_state);
}
*/
/*
void Button::SetAnim(CSprite *sprite,int lockID)
{
SetAnim(sprite,lockID,lockID+1,lockID+2,lockID+3);
}
*/
void Button::SetSound(int soundID)
{
	_soundID = soundID;
}

void Button::SetAnim(int spriteID, int lockID, int normalID, int selectID, int activeID)
{
//#	_anim_lock->SetAnimSprite(GET_SPRITE(spriteID), spriteID);
//#	_anim_lock->SetAnim(lockID);

	_anim->SetAnimSprite(GET_SPRITE(spriteID), spriteID);
	_anim_overplay->SetAnimSprite(GET_SPRITE(spriteID), spriteID);
	_lockID = lockID;
	_normalID = normalID;
	_selectID = selectID;
	_activeID = activeID;
	_anim_overplay->SetAnim(_selectID);
	SetState(_state);
}
void Button::SetAnim(int spriteID, int lockID)
{
	SetAnim(spriteID, lockID, lockID + 1, lockID + 2, lockID + 3);
}
void Button::SetExt(int width, int height)
{
	_extWidth = width;
	_extHeight = height;
}
float Button::GetWidth()
{
	if (!_anim->IsCurrentAnimStop())
	{
		//return _anim->GetCurrentAnimWidth();
		CSprite* _target_sprite = _anim->_anim_sprite;
		INT32 frame_index = _anim->_current_aframes;
		INT32 frame_id = _target_sprite->_aframe_id[frame_index];
		return GetFrameWidth(_target_sprite, frame_id);
	}
	return 0;
}
float Button::GetHeight()
{
	if (!_anim->IsCurrentAnimStop())
	{
		CSprite* _target_sprite = _anim->_anim_sprite;
		INT32 frame_index = _anim->_current_aframes;
		INT32 frame_id = _target_sprite->_aframe_id[frame_index];
		return GetFrameHeight(_target_sprite, frame_id);
	}
	return 0;
}
void Button::SetPos(float x, float y)
{
	_posX = x;
	_posY = y;
}
void Button::SetState(int state)
{
	_state = state;

	switch (state)
	{
	case BUTTON_STATE_LOCK:
		//_bt_actived = NULL;
		_anim->SetAnim(_lockID, false);
		break;
	case BUTTON_STATE_NORMAL:
		if (this == Button::GetButtonActive())
		{
			_bt_actived = NULL;
		}
		_anim->SetAnim(_normalID, false);

		break;
	case BUTTON_STATE_SELECT:
		if (_bt_actived != this)
		{
			if(_soundID != -1)
			{
				CGame::_this->PlaySFX(_soundID);
			}
		}

		_bt_actived = this;
		_anim->SetAnim(_selectID, false);
		break;
	case BUTTON_STATE_ACTIVE:
		
		_anim->SetAnim(_activeID, false);
		//_bt_actived = NULL;
		break;
	}
}
bool Button::NeedAction()
{
	if (_state == BUTTON_STATE_ACTIVE && _anim->IsCurrentAnimStop())
	{
		_anim->SetAnim(_activeID, false);
        _bt_actived = NULL;
		return true;
	}
	return false;
}
void Button::Render(GLESLib* g, int offx, int offy, bool overplay)
{
	if (_state == BUTTON_STATE_HIDE)
	{
		return;
	}
	if (overplay && _state)
	{

		_anim_overplay->SetPos(_posX + GetFixX() + offx, _posY + GetFixY() + offy);
		_anim_overplay->DrawAnimObject(g, true);
		_anim_overplay->UpdateAnimObject();
	}
	else
	if (!_anim->IsCurrentAnimStop())
	{
		//_anim->SetPos((_posX+offx)*_pos_scalex,(_posY+offy)*_pos_scaley);
		_anim_overplay->SetPos(_posX + GetFixX() + offx, _posY + GetFixY() + offy);
		_anim->SetPos(_posX + GetFixX() + offx + _offX, _posY + GetFixY() + offy + _offY);

		if (_typeEffect == TYPE_EFFECT_MOVE_UP_DOWN)
		{
			float CX = GetCenterX();
			float CY = GetCenterY();
			G()->SetScale(_scaleEffectX, _scaleEffectY, CX, CY);
		}
		switch (_state)
		{
		case BUTTON_STATE_LOCK:
			_anim->DrawAnimObject(g);
			break;
		case BUTTON_STATE_NORMAL:
			_anim->DrawAnimObject(g);
			break;
		case BUTTON_STATE_SELECT:
			_anim->DrawAnimObject(g);
			break;
		case BUTTON_STATE_ACTIVE:
			_anim->DrawAnimObject(g);
			//_anim_overplay->DrawAnimObject(g, true);
			//_anim_overplay->UpdateAnimObject();
			break;
		}
		if (_typeEffect == TYPE_EFFECT_MOVE_UP_DOWN)
		{
			G()->ClearScale();
		}
#if DEBUG_BUTTON
		float x = TZ_GetX();
		float y = TZ_GetY();
		float w = TZ_GetWidth();
		float h = TZ_GetHeight();
		if (x > 0 && y > 0){
			printf("hree\n");
		}
		G()->SetColor(0x44ff0000);
		G()->FillRect(x, y, w, h,true);
#endif
	}
}
void Button::Update()
{
	if (_state == BUTTON_STATE_HIDE)
	{
		return;
	}
	_anim->UpdateAnimObject();
	//#_anim_lock->UpdateAnimObject();
	if (_anim->IsCurrentAnimStop())
	{
		//printf("state = %d",_state);
		switch (_state)
		{
		case BUTTON_STATE_LOCK:
			_anim->SetAnim(_lockID, false);
			break;
		case BUTTON_STATE_NORMAL:
			_anim->SetAnim(_normalID, false);
			break;
		case BUTTON_STATE_SELECT:
			_anim->SetAnim(_selectID, false);
			_anim->SetToFrameLast();
			break;
		case BUTTON_STATE_ACTIVE:
			///_anim->SetAnim(_selectID,true);
			break;
		}
	}


	if (_typeEffect == TYPE_EFFECT_MOVE_UP_DOWN)
	{
		
		switch (_stateEffet)
		{
		case 0:
			_timeDelay--;
			if (_timeDelay <= 0)
			{
				_stateEffet = 1;
			}
			break;
		case 1:
			_scaleEffectX -= 0.002;
			_scaleEffectY += 0.002;
			_offY -= 0.8;
			if (_offY <= -10)
			{
				_stateEffet = 2;
				_offY = -10;
			}
			break;
		case 2:
			_scaleEffectX += 0.002;
			_scaleEffectY -= 0.002;
			_offY += 0.8;
			if (_offY >= 0)
			{
				_stateEffet = 3;
				_offY = 0;
			}
			break;

			
		case 3:
			_scaleEffectX += 0.001;
			_scaleEffectY -= 0.001;
			_offY -= 0.3;
			if (_offY <= -4)
			{
				_offY = -4;
				_stateEffet = 4;
			}
			break;

		case 4:
			_scaleEffectX -= 0.001;
			_scaleEffectY += 0.001;
			_offY += 0.3;
			if (_offY >= 0)
			{
				_offY = 0;
				_stateEffet = 1;
			}
			break;

		}
	}
	/*CSprite* _target_sprite = _anim_lock->_anim_sprite;
	INT32 frame_index = _anim_lock->_current_aframes;
	INT32 frame_id = _target_sprite->_aframe_id[frame_index];
	int modulex = Button::GetCurrentAnimFrameAnimX(_anim_lock);
	int moduley = Button::GetCurrentAnimFrameAnimY(_anim_lock);
	//int modulew = _target_sprite->GetFModuleWidth(frame_id, 0);
	//int moduleh = _target_sprite->GetFModuleHeight(frame_id, 0);

	//PineRectangle rect = _target_sprite->GetFmoduleBound(frame_id, 0);
	//int moduley = _target_sprite->GetFModuleY(frame_id, 0);// + Button::GetCurrentAnimFrameAnimY(_anim_lock);
	int modulew = _target_sprite->GetFModuleWidth(frame_id, 0);
	int moduleh = _target_sprite->GetFModuleHeight(frame_id, 0);*/
	
	CSprite* _target_sprite =_anim->_anim_sprite;
	INT32 frame_index = _anim-> _current_aframes;
	INT32 frame_id = _target_sprite->_aframe_id[frame_index];
	PineRectangle rect = _target_sprite->GetFrameBound(frame_id);
	int modulex = _target_sprite->GetFModuleX(frame_id,0);
	int moduley = _target_sprite->GetFModuleY(frame_id,0);
	int modulew = _target_sprite->GetFModuleWidth(frame_id,0);
	int moduleh = _target_sprite->GetFModuleHeight(frame_id,0);

	_fixedX = rect.X;// + _posX;
	_fixedY = rect.Y;// + _posY;
	_fixedW = rect.W;
	_fixedH = rect.H;
}
bool Button::CurrentAnimStop()
{
	return _anim->IsCurrentAnimStop();
}
float Button::GetFixX()
{
	if (!_anim->IsCurrentAnimStop())
	{

		CSprite* _target_sprite = _anim->_anim_sprite;
		INT32 frame_index = _anim->_current_aframes;
		INT32 frame_id = _target_sprite->_aframe_id[frame_index];
		float w = (GetFrameWidth(_target_sprite, frame_id)*_pos_scalex - GetFrameWidth(_target_sprite, frame_id)) / 2;
		return (GetFrameX(_target_sprite, frame_id)*_pos_scalex - GetFrameX(_target_sprite, frame_id)) + w;

	}
	return 0;
}
float Button::GetFixY()
{
	if (!_anim->IsCurrentAnimStop())
	{

		CSprite* _target_sprite = _anim->_anim_sprite;
		INT32 frame_index = _anim->_current_aframes;
		INT32 frame_id = _target_sprite->_aframe_id[frame_index];
		float h = (GetFrameHeight(_target_sprite, frame_id)*_pos_scalex - GetFrameHeight(_target_sprite, frame_id)) / 2;
		return (GetFrameY(_target_sprite, frame_id)*_pos_scaley - GetFrameY(_target_sprite, frame_id)) + h;

	}
	return 0;
}
int Button::TZ_GetX()
{
	if (!_anim->IsCurrentAnimStop())
	{
		switch (_anchor)
		{
		case A_NORMAL:
			return (_posX - _extWidth / 2)*_pos_scalex;
		case A_HVCENTER:
		{

			CSprite* _target_sprite = _anim->_anim_sprite;
			PinePoint point = _target_sprite->GetAFramePos(_anim->_current_aframes);
			INT32 frame_index = _anim->_current_aframes;
			INT32 frame_id = _target_sprite->_aframe_id[frame_index];
			//printf("\nframe_id = %d",frame_id);
			float w = (GetFrameWidth(_target_sprite, frame_id)*_pos_scalex - GetFrameWidth(_target_sprite, frame_id)) / 2;
			return point.X + (_posX - ((_extWidth) / 2) + GetFrameX(_target_sprite, frame_id))*_pos_scalex + w;
		}
		}
	}
	return 0;
}
int Button::TZ_GetY()
{
	//if (!_anim->IsCurrentAnimStop())
	{
		switch (_anchor)
		{
		case A_NORMAL:
			return (_posY - _extHeight / 2)*_pos_scaley;
		case A_HVCENTER:
		{
			CSprite* _target_sprite = _anim->_anim_sprite;
			PinePoint point = _target_sprite->GetAFramePos(_anim->_current_aframes);
			INT32 frame_index = _anim->_current_aframes;
			INT32 frame_id = _target_sprite->_aframe_id[frame_index];
			float h = (GetFrameHeight(_target_sprite, frame_id)*_pos_scalex - GetFrameHeight(_target_sprite, frame_id)) / 2;
			return point.Y + (_posY - ((_extHeight) / 2) + GetFrameY(_target_sprite, frame_id))*_pos_scaley + h;
		}
		}
	}
	return 0;
}
int Button::TZ_GetWidth()
{
	//if (!_anim->IsCurrentAnimStop())
	{
		return (_extWidth + GetWidth());
	}
	return 0;
}
int Button::TZ_GetHeight()
{
	//if (!_anim->IsCurrentAnimStop())
	{
		return (_extHeight + GetHeight());
	}
	return 0;
}

void Button::ResetPlaySound()
{
	//_bt_actived = NULL;
}
Button* Button::GetButtonActive()
{
	return _bt_actived;
}
int Button::GetFrameCenterX(CSprite* spr, int frameID)
{
	return GetFrameX(spr, frameID) + GetFrameWidth(spr,frameID)/2;
}
int Button::GetFrameCenterY(CSprite* spr, int frameID)
{
	return GetFrameY(spr, frameID) + GetFrameHeight(spr, frameID) / 2;
}

int Button::GetFrameX(CSprite* spr, int frameID)
{
	PineRectangle rect = spr->GetFrameBound(frameID);
	return rect.X;
	int min = spr->GetFModuleX(frameID, 0);
	//printf("\t spr->GetFModuleNum(%d) = %d",frameID,spr->GetFModuleNum(frameID));
	for (int i = 1; i < spr->GetFModuleNum(frameID); i++)
	{
		if (min > spr->GetFModuleX(frameID, i))
		{
			min = spr->GetFModuleX(frameID, i);
		}
	}
	return min;
}
int Button::GetFrameY(CSprite* spr, int frameID)
{
	PineRectangle rect = spr->GetFrameBound(frameID);
	return rect.Y;
	int min = spr->GetFModuleY(frameID, 0);
	for (int i = 0; i < spr->GetFModuleNum(frameID); i++)
	{
		if (min > spr->GetFModuleY(frameID, i))
		{
			min = spr->GetFModuleY(frameID, i);
		}
	}
	return min;
}

int Button::GetFrameWidth(CSprite* sprite, int frame_id)
{
	PineRectangle rect = sprite->GetFrameBound(frame_id);
	return rect.W;
	//
	int fmodule_num = sprite->GetFModuleNum(frame_id);
	float top = sprite->GetFModuleY(frame_id, 0);
	float left = sprite->GetFModuleX(frame_id, 0);
	float bottom = sprite->GetFModuleY(frame_id, 0) + sprite->GetFModuleHeight(frame_id, 0);
	float right = sprite->GetFModuleX(frame_id, 0) + sprite->GetFModuleWidth(frame_id, 0);
	for (INT32 j = 0; j < fmodule_num; j++)
	{
		INT32 mid = sprite->_fmodule_id[j];
		float moduleW = sprite->GetFModuleWidth(frame_id, j);
		float moduleH = sprite->GetFModuleHeight(frame_id, j);
		float mx = sprite->GetFModuleX(frame_id, j);
		float my = sprite->GetFModuleY(frame_id, j);
		if (mx<left)left = mx;
		if (my<top)top = my;
		if ((mx + moduleW)>right)right = mx + moduleW;
		if ((my + moduleH)>bottom)bottom = my + moduleH;
	}
	return (right - left);
}
int Button::GetFrameHeight(CSprite* sprite, int frame_id)
{
	PineRectangle rect = sprite->GetFrameBound(frame_id);
	return rect.H;
	//
	int fmodule_num = sprite->GetFModuleNum(frame_id);
	float top = sprite->GetFModuleY(frame_id, 0);
	float left = sprite->GetFModuleX(frame_id, 0);
	float bottom = sprite->GetFModuleY(frame_id, 0) + sprite->GetFModuleHeight(frame_id, 0);
	float right = sprite->GetFModuleX(frame_id, 0) + sprite->GetFModuleWidth(frame_id, 0);
	for (INT32 j = 0; j < fmodule_num; j++)
	{
		INT32 mid = sprite->_fmodule_id[j];
		float moduleW = sprite->GetFModuleWidth(frame_id, j);
		float moduleH = sprite->GetFModuleHeight(frame_id, j);
		float mx = sprite->GetFModuleX(frame_id, j);
		float my = sprite->GetFModuleY(frame_id, j);
		if (mx<left)left = mx;
		if (my<top)top = my;
		if ((mx + moduleW)>right)right = mx + moduleW;
		if ((my + moduleH)>bottom)bottom = my + moduleH;
	}
	return (bottom - top);
}
int  Button::GetCurrentAnimMarkerX(CAnimObject *anim, int module)
{
	anim->CalculateCurrentAFrame();
	CSprite* _target_sprite = anim->_anim_sprite;
	INT32 frame_index = anim->_current_aframes;
	INT32 frame_id = _target_sprite->_aframe_id[frame_index];
	PineRectangle rect = _target_sprite->GetFmoduleBound(frame_id, module);
	return rect.X + rect.W/2;
	return _target_sprite->GetFModuleX(frame_id, module) + _target_sprite->GetFModuleWidth(frame_id, module) / 2;
}
int  Button::GetCurrentAnimMarkerY(CAnimObject *anim, int module)
{
	anim->CalculateCurrentAFrame();
	CSprite* _target_sprite = anim->_anim_sprite;
	INT32 frame_index = anim->_current_aframes;
	INT32 frame_id = _target_sprite->_aframe_id[frame_index];
	PineRectangle rect = _target_sprite->GetFmoduleBound(frame_id, module);
	return rect.Y + rect.H / 2;
	return _target_sprite->GetFModuleY(frame_id, module) + _target_sprite->GetFModuleHeight(frame_id, module) / 2;
}
int Button::GetModuleWidth(CSprite* _target_sprite, int frame_id, int module)
{
	PineRectangle rect = _target_sprite->GetFmoduleBound(frame_id, module);
	return rect.W;
	return _target_sprite->GetFModuleWidth(frame_id, module);
}
int Button::GetModuleHeight(CSprite* _target_sprite, int frame_id, int module)
{
	PineRectangle rect = _target_sprite->GetFmoduleBound(frame_id, module);
	return rect.W;
	return _target_sprite->GetFModuleHeight(frame_id, module);
}
int Button::GetModuleMarkerX(CSprite* _target_sprite, int frame_id, int module)
{
	PineRectangle rect = _target_sprite->GetFmoduleBound(frame_id, module);
	return rect.X + rect.W/2;
	return _target_sprite->GetFModuleX(frame_id, module) + _target_sprite->GetFModuleWidth(frame_id, module) / 2;
}
int Button::GetModuleMarkerY(CSprite* _target_sprite, int frame_id, int module)
{
	PineRectangle rect = _target_sprite->GetFmoduleBound(frame_id, module);
	return rect.Y + rect.H / 2;
	return _target_sprite->GetFModuleY(frame_id, module) + _target_sprite->GetFModuleHeight(frame_id, module) / 2;
}
int Button::GetCurrentAnimFModuleAngle(CAnimObject *anim, int module)
{
	/*
	anim->CalculateCurrentAFrame();
	CSprite* _target_sprite = anim->_anim_sprite;
	INT32 frame_index = anim-> _current_aframes;
	INT32 frame_id = _target_sprite->_aframe_id[frame_index];
	return _target_sprite->GetFModuleAngle(frame_id,module);
	*/
	return 0;
}
int Button::GetModuleOpacity(CSprite *sprite, int frame_id, int moduleOfFrame)
{

	//return sprite->GetFModuleOpacity(frame_id,moduleOfFrame);
	return 100;
}
int Button::GetModuleNum(CSprite* sprite, int frame_id)
{
	return sprite->GetModuleOfFrame(frame_id);
}
int Button::GetCurrentAnimFModuleX(CAnimObject *anim, int module)
{
	anim->CalculateCurrentAFrame();
	CSprite* _target_sprite = anim->_anim_sprite;
	INT32 frame_index = anim->_current_aframes;
	INT32 frame_id = _target_sprite->_aframe_id[frame_index];
	PineRectangle rect = _target_sprite->GetFmoduleBound(frame_id, module);
	return rect.X;
	return _target_sprite->GetFModuleX(frame_id, module);
}
int Button::GetCurrentAnimFModuleY(CAnimObject *anim, int module)
{
	anim->CalculateCurrentAFrame();
	CSprite* _target_sprite = anim->_anim_sprite;
	INT32 frame_index = anim->_current_aframes;
	INT32 frame_id = _target_sprite->_aframe_id[frame_index];
	PineRectangle rect = _target_sprite->GetFmoduleBound(frame_id, module);
	return rect.Y;
	return _target_sprite->GetFModuleY(frame_id, module);
}

int Button::GetCurrentAnimFModuleWidth(CAnimObject *anim, int module)
{
	anim->CalculateCurrentAFrame();
	CSprite* _target_sprite = anim->_anim_sprite;
	INT32 frame_index = anim->_current_aframes;
	INT32 frame_id = _target_sprite->_aframe_id[frame_index];
	PineRectangle rect = _target_sprite->GetFmoduleBound(frame_id, module);
	return rect.W;
	return _target_sprite->GetFModuleWidth(frame_id, module);
}
int Button::GetCurrentAnimFModuleHeight(CAnimObject *anim, int module)
{
	anim->CalculateCurrentAFrame();
	CSprite* _target_sprite = anim->_anim_sprite;
	INT32 frame_index = anim->_current_aframes;
	INT32 frame_id = _target_sprite->_aframe_id[frame_index];
	PineRectangle rect = _target_sprite->GetFmoduleBound(frame_id, module);
	return rect.H;
	return _target_sprite->GetFModuleHeight(frame_id, module);
}

int Button::GetCurrentAnimFrameX(CAnimObject *anim, int frameID)
{
	anim->CalculateCurrentAFrame();
	CSprite* _target_sprite = anim->_anim_sprite;
	INT32 frame_index = anim->_current_aframes;
	INT32 frame_id = _target_sprite->_aframe_id[frame_index];
	return GetFrameX(_target_sprite, frame_id);
}
int Button::GetCurrentAnimFrameY(CAnimObject *anim, int frameID)
{
	anim->CalculateCurrentAFrame();
	CSprite* _target_sprite = anim->_anim_sprite;
	INT32 frame_index = anim->_current_aframes;
	INT32 frame_id = _target_sprite->_aframe_id[frame_index];
	return GetFrameY(_target_sprite, frame_id);
}
int Button::GetCurrentAnimFrameWidth(CAnimObject *anim, int frameID)
{
	anim->CalculateCurrentAFrame();
	CSprite* _target_sprite = anim->_anim_sprite;
	INT32 frame_index = anim->_current_aframes;
	INT32 frame_id = _target_sprite->_aframe_id[frame_index];
	return GetFrameWidth(_target_sprite, frame_id);
}
int Button::GetCurrentAnimFrameHeight(CAnimObject *anim, int frameID)
{
	anim->CalculateCurrentAFrame();
	CSprite* _target_sprite = anim->_anim_sprite;
	INT32 frame_index = anim->_current_aframes;
	INT32 frame_id = _target_sprite->_aframe_id[frame_index];
	return GetFrameHeight(_target_sprite, frame_id);
}

int Button::GetModuleID(CSprite *sprite, int frame_id, int moduleOfFrame)
{
	INT32 fmodule_min = 0;
	INT32 fmodule_max = 0;

	if (frame_id < (sprite->_num_frames - 1))
	{
		fmodule_min = sprite->_frame_offset[frame_id];
		fmodule_max = sprite->_frame_offset[frame_id + 1] - 1;
	}
	else if (frame_id == (sprite->_num_frames - 1))
	{
		fmodule_min = sprite->_frame_offset[frame_id];
		fmodule_max = sprite->_num_fmodules - 1;
	}
	return sprite->_fmodule_id[fmodule_min + moduleOfFrame];
}

int Button::GetCurrentAnimFrameAnimX(CAnimObject *anim)
{
	return anim->_anim_sprite->_aframe_x[anim->_current_aframes];
}

int Button::GetCurrentAnimFrameAnimY(CAnimObject *anim)
{
	return anim->_anim_sprite->_aframe_y[anim->_current_aframes];
}