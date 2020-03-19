#pragma once
#include "singletonBase.h"

class cameraManager : public singletonBase<cameraManager>
{
private: // 내부 변수
	POINTFLOAT _playerCenterPos;
	POINTFLOAT _mapTotalSize;
	POINTFLOAT _cameraXY_Size;
	POINTFLOAT _cameraPOS;
	POINT _currentRenderTileID_LT;
	POINT _currentRenderTileID_RB;
	RECT _renderArea;

public:
	cameraManager();
	~cameraManager();

	HRESULT init(POINTFLOAT PlayerCenterPos, POINTFLOAT MapTotalSize, POINTFLOAT CameraXY_Size);
	void release();
	void update();

public:
	void render_Camera_ImageRender(const char* ImageName, HDC hdc, POINTFLOAT Pos);
	void render_Camera_ImageRender(RECT ObjRC, const char* ImageName, HDC hdc, POINTFLOAT Pos);
	void render_Camera_FrameRender(const char* ImageName, HDC hdc, POINTFLOAT Pos, POINT FrameID);
	void render_Camera_FrameRender(RECT ObjRC, const char* ImageName, HDC hdc, POINTFLOAT Pos, POINT FrameID);

	void Set_PlayerCenterPos(POINTFLOAT PlayerCenterPos) { _playerCenterPos = PlayerCenterPos; }

	POINTFLOAT get_CameraPos() { return _cameraPOS; }
	POINT get_currentRenderTileID_LT() { return _currentRenderTileID_LT; }
	POINT get_currentRenderTileID_RB() { return _currentRenderTileID_RB; }
	RECT get_renderArea() { return _renderArea; }
};