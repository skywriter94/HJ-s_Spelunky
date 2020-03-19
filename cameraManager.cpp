#include "stdafx.h"
#include "cameraManager.h"



cameraManager::cameraManager()
{
}

cameraManager::~cameraManager()
{
}



HRESULT cameraManager::init(POINTFLOAT PlayerCenterPos, POINTFLOAT MapTotalSize, POINTFLOAT CameraXY_Size)
{
	_cameraPOS = _playerCenterPos = PlayerCenterPos;
	_mapTotalSize = MapTotalSize;
	_cameraXY_Size = CameraXY_Size;
	_renderArea = RectMakeCenter(_cameraPOS.x, _cameraPOS.y, _cameraXY_Size.x, _cameraXY_Size.y);
	return S_OK;
}

void cameraManager::release()
{
}

void cameraManager::update()
{
	if (_playerCenterPos.x < _cameraXY_Size.x * 0.5f)
	{
		_cameraPOS.x = _cameraXY_Size.x * 0.5f;
	}
	else if (_playerCenterPos.x > _mapTotalSize.x - _cameraXY_Size.x * 0.5f)
	{
		_cameraPOS.x = _mapTotalSize.x - _cameraXY_Size.x * 0.5f;
	}
	else
	{
		_cameraPOS.x = _playerCenterPos.x;
	}

	if (_playerCenterPos.y < _cameraXY_Size.y * 0.5f)
	{
		_cameraPOS.y = _cameraXY_Size.y * 0.5f;
	}
	else if (_playerCenterPos.y > _mapTotalSize.y - _cameraXY_Size.y * 0.5f)
	{
		_cameraPOS.y = _mapTotalSize.y - _cameraXY_Size.y * 0.5f;
	}
	else
	{
		_cameraPOS.y = _playerCenterPos.y;
	}
	_renderArea = RectMakeCenter(_cameraPOS.x, _cameraPOS.y, _cameraXY_Size.x, _cameraXY_Size.y);
	_currentRenderTileID_LT = { _renderArea.left / 64, _renderArea.top / 64 }; // 64는 한 타일의 사이즈
	_currentRenderTileID_RB = { _renderArea.right / 64, _renderArea.bottom / 64 };
}



void cameraManager::render_Camera_ImageRender(const char* ImageName, HDC hdc, POINTFLOAT Pos)
{
	IMAGEMANAGER->render(ImageName, hdc, Pos.x, Pos.y);
}

void cameraManager::render_Camera_ImageRender(RECT ObjRC, const char* ImageName, HDC hdc, POINTFLOAT Pos)
{
	RECT tempRC;
	if (IntersectRect(&tempRC, &_renderArea, &ObjRC))
	{
		IMAGEMANAGER->render(ImageName, hdc, Pos.x, Pos.y);
	}
}

void cameraManager::render_Camera_FrameRender(const char* ImageName, HDC hdc, POINTFLOAT Pos, POINT FrameID)
{
	IMAGEMANAGER->frameRender(ImageName, hdc, Pos.x, Pos.y, FrameID.x, FrameID.y);
}

void cameraManager::render_Camera_FrameRender(RECT ObjRC, const char* ImageName, HDC hdc, POINTFLOAT Pos, POINT FrameID)
{
	RECT tempRC;
	if (IntersectRect(&tempRC, &_renderArea, &ObjRC))
	{
		IMAGEMANAGER->frameRender(ImageName, hdc, Pos.x, Pos.y, FrameID.x, FrameID.y);
	}
}