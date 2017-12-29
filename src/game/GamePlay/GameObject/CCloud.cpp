#include "pch.h"
#include "../../GameCore/CGame.h"

void CCloud::Init()
{
	for (int i = 0; i < MAX_CLOUD_INGAME; i++)
	{
		_cloud[i]._state = enumStateCloud::STATE_CLOUD_NONE;
	}
}
void CCloud::AddCloud(float x, float y)
{
	for (int i = 0; i < MAX_CLOUD_INGAME; i++)
	{
		if (_cloud[i]._state == enumStateCloud::STATE_CLOUD_NONE)
		{
			_cloud[i]._state = enumStateCloud::STATE_CLOUD_ZOOM_IN;
			_cloud[i]._frame = 1 + CMath::RANDOM(0, 4);
			_cloud[i]._x = x;
			_cloud[i]._y = y + OFFSET_IPAD(-100) + OFFSET_I4(-50);
			_cloud[i]._opacity = 100;
			_cloud[i]._zoom = 1;
			_cloud[i]._delZoom = CMath::RANDOM(100, 500)*0.00001;
			_cloud[i]._speed = 0.5 + CMath::RANDOM(-10, 100)*0.01;

			
			_cloud[i]._coll._w = GET_SPRITE(SPRITEID_OBJECTS_CLOUD)->GetFModuleWidth(_cloud[i]._frame,0);
			_cloud[i]._coll._h = GET_SPRITE(SPRITEID_OBJECTS_CLOUD)->GetFModuleHeight(_cloud[i]._frame, 0);

			break;
		}
	}
}
void CCloud::Update()
{
	for (int i = 0; i < MAX_CLOUD_INGAME; i++)
	{
		if (_cloud[i]._state != enumStateCloud::STATE_CLOUD_NONE)
		{
			
			switch (_cloud[i]._state)
			{
			case enumStateCloud::STATE_CLOUD_ZOOM_IN:
				_cloud[i]._zoom -= _cloud[i]._delZoom;
				if (_cloud[i]._zoom < 0.9)
				{
					_cloud[i]._state = enumStateCloud::STATE_CLOUD_ZOOM_OUT;
				}
				break;

			case enumStateCloud::STATE_CLOUD_ZOOM_OUT:
				_cloud[i]._zoom += _cloud[i]._delZoom;
				if (_cloud[i]._zoom > 1.1)
				{
					_cloud[i]._state = enumStateCloud::STATE_CLOUD_ZOOM_IN;
				}
				break;

			case enumStateCloud::STATE_CLOUD_HIDE:
				_cloud[i]._zoom += _cloud[i]._delZoom;
				_cloud[i]._opacity -= 3;
				if (_cloud[i]._opacity <= 0)
				{
					_cloud[i]._state = enumStateCloud::STATE_CLOUD_NONE;
					AddCloud(k_SCREEN_WIDTH + 100, CMath::RANDOM(100, 250));
				}
				break;

			}
			if (_cloud[i]._state != enumStateCloud::STATE_CLOUD_NONE)
			{
				_cloud[i]._x -= _cloud[i]._speed;
				if (_cloud[i]._x < -100)
				{
					_cloud[i]._state = enumStateCloud::STATE_CLOUD_NONE;
					AddCloud(k_SCREEN_WIDTH + 100, CMath::RANDOM(100, 250));
				}
			}

			_cloud[i]._coll._x = _cloud[i]._x + GET_SPRITE(SPRITEID_OBJECTS_CLOUD)->GetFModuleX(_cloud[i]._frame, 0);
			_cloud[i]._coll._y = _cloud[i]._y + GET_SPRITE(SPRITEID_OBJECTS_CLOUD)->GetFModuleY(_cloud[i]._frame, 0);
		}
	}

	
}
void CCloud::Render()
{
	for (int i = 0; i < MAX_CLOUD_INGAME; i++)
	{
		if (_cloud[i]._state != enumStateCloud::STATE_CLOUD_NONE)
		{
			G()->SetScale(_cloud[i]._zoom, _cloud[i]._zoom, _cloud[i]._x, _cloud[i]._y);
			G()->SetOpacity(_cloud[i]._opacity);
			GET_SPRITE(SPRITEID_OBJECTS_CLOUD)->DrawFrame(G(), _cloud[i]._frame, _cloud[i]._x, _cloud[i]._y);
			G()->SetOpacity(100);
			G()->ClearScale();

			//G()->SetColor(0xaaff0000);
			//G()->FillRect(_cloud[i]._coll._x, _cloud[i]._coll._y, _cloud[i]._coll._w, _cloud[i]._coll._h,true);
		}
	}
}

bool CCloud::CheckCloudHide(Position bullseye)
{
	bool res = false;
	for (int i = 0; i < MAX_CLOUD_INGAME; i++)
	{
		if (_cloud[i]._state != enumStateCloud::STATE_CLOUD_NONE && _cloud[i]._state != enumStateCloud::STATE_CLOUD_HIDE)
		{
			if (CHECK_RECT_IN_CIRCLE(_cloud[i]._coll._x, _cloud[i]._coll._y, _cloud[i]._coll._w, _cloud[i]._coll._y, bullseye._x, bullseye._y, 20))
			{
				_cloud[i]._state = enumStateCloud::STATE_CLOUD_HIDE;
				res =  true;
			}
		}
	}
	return res;
}