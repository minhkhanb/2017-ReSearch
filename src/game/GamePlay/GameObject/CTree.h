#if !defined(CTREE_H)
#define CTREE_H

#define NUM_BODY_TREE			3
#define NUM_LEAF_TREE			5

struct Node
{
	float _x, _y,_offX,_offY;
	float _rota,_offRoat;
	int _state, _frame;
};
class CTree
{
public:	
	float _treeX, _treeY,_speed;
	Node _root, _body[NUM_BODY_TREE], _leaf[NUM_LEAF_TREE];

	CAnimObject _animTree;
	void Init(float x, float y);
    void SetSpeed(float speed){_speed = speed;}
	void Update();
	void Render();
	//Root
	void UpdateRoot();
	void RenderRoot();
	
	//body
	void UpdateBody();
	void RenderBody();

	//leaf
	void UpdateLeaf();
	void RenderLeaf();
};
#endif