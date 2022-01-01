#include "InputDev.h"

USING(Engine)
IMPLEMENT_SINGLETON(CInputDev)

Engine::CInputDev::CInputDev(void)
	: m_pInputSDK(nullptr)
	, m_pKeyBoard(nullptr)
	, m_pMouse(nullptr)
{

}

Engine::CInputDev::~CInputDev(void)
{
	Free();
}

HRESULT Engine::CInputDev::Ready_InputDev(HINSTANCE hInst, HWND hWnd)
{
	// DINPUT�� �̿��Ͽ� ���� �ϵ��� ����ϴ� ��ü�� �����ϱ� ���� ��ü ��ü�� ����
	FAILED_CHECK_RETURN(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInputSDK, NULL), E_FAIL);
	
	// �� ��ǻ�Ϳ� ����Ǿ� �ִ� Ű���� �İ�ü�� ����
	FAILED_CHECK_RETURN(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, NULL), E_FAIL);

	// ������ Ű���� ��ü�� ���� ������ �����ϴ� ���
	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);

	// ������ Ű������ ���� ������ ����
	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// ������ Ű���带 Ȱ��ȭ
	m_pKeyBoard->Acquire();

	// �� ��ǻ�Ϳ� ����Ǿ� �ִ� ���콺 �İ�ü�� ����
	FAILED_CHECK_RETURN(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, NULL), E_FAIL);

	// ������ ���콺 ��ü�� ���� ������ �����ϴ� ���
	m_pMouse->SetDataFormat(&c_dfDIMouse);

	// ������ ���콺 ���� ������ ����
	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// ������ ���콺 Ȱ��ȭ
	m_pMouse->Acquire();


	return S_OK;
}

void Engine::CInputDev::Update_InputDev(void)
{
	m_pKeyBoard->GetDeviceState(256, m_byKeyState);
	m_pMouse->GetDeviceState(sizeof(m_MouseState), &m_MouseState);
}

void Engine::CInputDev::Free(void)
{
	Safe_Release(m_pMouse);
	Safe_Release(m_pKeyBoard);
	Safe_Release(m_pInputSDK);
}

