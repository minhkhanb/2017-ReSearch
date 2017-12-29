#include "pch.h"
#include "../../GameCore/CGame.h"

void CTree::Init(float x, float y)
{
	_treeX = x;
	_treeY = y;
	//
	_root._x = _treeX;
	_root._y = _treeY;
	_root._rota = 0;
	_root._offRoat = 0.01;
	_root._state = 1;
    _speed = 0;
	for (int i = 0; i < NUM_BODY_TREE; i++)
	{
		_body[i]._x = _treeX;
		_body[i]._y = _treeY;
		_body[i]._rota = 0;
		_body[i]._offRoat = 0.01;
		_body[i]._state = 1;
	}

	for (int i = 0; i < NUM_LEAF_TREE; i++)
	{
		_leaf[i]._x = _treeX;
		_leaf[i]._y = _treeY;
		_leaf[i]._rota = 0;
		_leaf[i]._offRoat = 0.01;
		_leaf[i]._state = 1;
	}

	_animTree.SetAnimSprite(GET_SPRITE(SPRITEID_OBJECTS_TREE), SPRITEID_OBJECTS_TREE);
	_animTree.SetAnim(0, true);
	_animTree.SetPos(_treeX, _treeY);

}
void CTree::Update()
{
    _treeX+= _speed;
	if (_treeX <= - 100)
	{
		_treeX = -100;
	}
	if (_treeX >= k_SCREEN_WIDTH + 100)
	{
		_treeX = k_SCREEN_WIDTH + 100;
	}
    _animTree.SetPos(_treeX, _treeY);
	UpdateRoot();
	UpdateBody();
	UpdateLeaf();
}
void CTree::Render()
{
	_animTree.DrawAnimObject(G());
	_animTree.UpdateAnimObject();
	return;
	G()->SetRotate(_root._rota, _root._x, _root._y);
	RenderBody();
	RenderRoot();
	RenderLeaf();
	G()->ClearRotate();
}
//Root
void CTree::UpdateRoot()
{
	return;
	if (_root._offRoat <= 0.01)
	{
		_root._offRoat = 0.01;
	}
	if (_root._offRoat >= 0.2)
	{
		_root._offRoat = 0.2;
	}
	switch (_root._state)
	{
	case 1:
		_root._rota -= _root._offRoat;
		_root._offRoat *= 1.05;
		if (_root._rota <= -2)
		{
			_root._rota = -2;
			_root._state = 2;
		}
		break;

	case 2:
		_root._rota += _root._offRoat;
		_root._offRoat *= 0.95;
		if (_root._rota >= 3)
		{
			_root._rota = 3;
			_root._state = 1;
		}
		break;
	}
}
void CTree::RenderRoot()
{
	//G()->SetRotate(_root._rota, _root._x, _root._y);
	GET_SPRITE(SPRITEID_OBJECTS_TREE)->DrawFrame(G(), 1, _root._x, _root._y);
	//G()->ClearRotate();
}

//body
void CTree::UpdateBody()
{
	for (int i = 0; i < NUM_BODY_TREE; i++)
	{
		if (_body[i]._offRoat <= 0.01)
		{
			_body[i]._offRoat = 0.01;
		}
		if (_body[i]._offRoat >= 0.25)
		{
			_body[i]._offRoat = 0.25;
		}
		switch (_body[i]._state)
		{
		case 1:
			_body[i]._rota -= _body[i]._offRoat;
			_body[i]._offRoat *= 1.055;
			if (_body[i]._rota <= -3)
			{
				_body[i]._rota = -3;
				_body[i]._state = 2;
			}
			break;

		case 2:
			_body[i]._rota += _body[i]._offRoat;
			_body[i]._offRoat *= 0.95;
			if (_body[i]._rota >= 3)
			{
				_body[i]._rota = 3;
				_body[i]._state = 1;
			}
			break;
		}
	}
	
}
void CTree::RenderBody()
{

	for (int i = 0; i < NUM_BODY_TREE; i++)
	{
		//G()->SetRotate(_body[i]._rota, _body[i]._x, _body[i]._y);
		GET_SPRITE(SPRITEID_OBJECTS_TREE)->DrawFrame(G(), 2 + i, _body[i]._x + _root._rota, _body[i]._y);
		//G()->ClearRotate();
	}
}

//leaf
void CTree::UpdateLeaf()
{
	for (int i = 0; i < NUM_LEAF_TREE; i++)
	{
		if (_leaf[i]._offRoat <= 0.01)
		{
			_leaf[i]._offRoat = 0.01;
		}
		if (_leaf[i]._offRoat >= 0.2)
		{
			_leaf[i]._offRoat = 0.2;
		}
		switch (_leaf[i]._state)
		{
		case 1:
			_leaf[i]._rota -= _leaf[i]._offRoat;
			_leaf[i]._offRoat *= 1.055;
			if (_leaf[i]._rota <= -3)
			{
				_leaf[i]._rota = -3;
				_leaf[i]._state = 2;
			}
			break;

		case 2:
			_leaf[i]._rota += _leaf[i]._offRoat;
			_leaf[i]._offRoat *= 0.95;
			if (_leaf[i]._rota >= 3)
			{
				_leaf[i]._rota = 3;
				_leaf[i]._state = 1;
			}
			break;
		}
	}
}
void CTree::RenderLeaf()
{
	for (int i = 0; i < NUM_LEAF_TREE; i++)
	{
		//G()->SetRotate(_leaf[i]._rota, _leaf[i]._x, _leaf[i]._y);
		GET_SPRITE(SPRITEID_OBJECTS_TREE)->DrawFrame(G(), 5 + i, _leaf[i]._x + _root._rota, _leaf[i]._y);
		//G()->ClearRotate();
	}
}