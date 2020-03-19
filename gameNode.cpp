#include "stdafx.h"
#include "gameNode.h"


gameNode::gameNode()
{
}


gameNode::~gameNode()
{

}

HRESULT gameNode::init()
{
	_hdc = GetDC(_hWnd);
	_managerInit = false;

	return S_OK;
}

HRESULT gameNode::init(bool managerInit)
{
	_hdc = GetDC(_hWnd);
	_managerInit = managerInit;

	if (_managerInit)
	{
		KEYMANAGER->init();
		IMAGEMANAGER->init();
		TIMEMANAGER->init();
		EFFECTMANAGER->init();
		SOUNDMANAGER->init();
		SCENEMANAGER->init();
		KEYANIMANAGER->init();
	}

	return S_OK;
}

void gameNode::release()
{
	if (_managerInit)
	{
		KEYMANAGER->release();
		KEYMANAGER->releaseSingleton();
		IMAGEMANAGER->release();
		IMAGEMANAGER->releaseSingleton();
		TIMEMANAGER->release();
		TIMEMANAGER->releaseSingleton();
		EFFECTMANAGER->release();
		EFFECTMANAGER->releaseSingleton();
		SOUNDMANAGER->release();
		SOUNDMANAGER->releaseSingleton();
		SCENEMANAGER->release();
		SCENEMANAGER->releaseSingleton();
		KEYANIMANAGER->release();
		KEYANIMANAGER->releaseSingleton();
	}

	ReleaseDC(_hWnd, _hdc);
}

void gameNode::update()
{
	
	SOUNDMANAGER->update();
}

void gameNode::render()
{
}


LRESULT gameNode::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{

	PAINTSTRUCT ps;
	HDC			hdc;

	switch (iMessage)
	{
		case WM_CREATE:

		break;

		//���� ���ļ� ��� �ּ����� ���� ���������ض�
		//���� �̷��� �ϴ� ������ �������� �������� ��ȹ�� ��ȹ�ڵ��� �ϴٺ���
		//MFC ������ ������ ������ ���� ��ȹ��Ʈ�� �Ѱ��ִ� ���� �ٹݻ翴��
		//�׷��� �ɹ��������� ���� ������ ��ɸ� ž���ؼ� �Ѱ����.
		//���� ������ �޽����� ������� �ʰ� �츮 ��ҿ� �ϴ���� �ϸ� 
		//�̷� ������ ���ĵ� �˴ϴ�
		case WM_MOUSEMOVE:
			_ptMouse.x = static_cast<float>(LOWORD(lParam));
			_ptMouse.y = static_cast<float>(HIWORD(lParam));

		
		break;

		case WM_KEYDOWN:
		{
			switch (wParam)
			{
				case VK_ESCAPE:
					PostQuitMessage(0);
				break;

			}
		}
		break;


		case WM_DESTROY:
			PostQuitMessage(0);
		break;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}
