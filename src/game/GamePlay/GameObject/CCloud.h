#if !defined(CCLOUD_H)
#define CCLOUD_H

#define MAX_CLOUD_INGAME			20

struct Cloud
{
	GameRect _coll,collTranf;
	float _x, _y;
	float _zoom,_delZoom,_speed;
	int _opacity,_state,_frame;
};
struct Position;
class CCloud
{
public:
	enum enumStateCloud
	{
		STATE_CLOUD_NONE,
		STATE_CLOUD_ZOOM_IN,
		STATE_CLOUD_ZOOM_OUT,
		STATE_CLOUD_HIDE,
	};

	Cloud _cloud[MAX_CLOUD_INGAME];
	void Init();
	void AddCloud(float x, float y);
	void Update();
	void Render();
	bool CheckCloudHide(Position bullseye);
};
#endif