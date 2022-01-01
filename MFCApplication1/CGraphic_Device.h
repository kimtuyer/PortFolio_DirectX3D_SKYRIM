#pragma once
class CGraphic_Device
{
	DECLARE_SINGLETON(CGraphic_Device)

private:

	CGraphic_Device();
	~CGraphic_Device();
public:
	LPDIRECT3DDEVICE9 Get_Device() { return m_pDevice; }
	LPD3DXSPRITE Get_Sprite() { return m_pSprite; }
	LPD3DXFONT Get_Font() { return m_pFont; }
	LPD3DXLINE  Get_Line() { return m_pLine; }
public:
	HRESULT Ready_Graphic_Device();
	void Release_Graphic_Device();
public:
	void Render_Begin();
	void Render_End(HWND hWnd = nullptr);
private:
	// ��ġ�� �����ϱ� ���� ���� -> 
	// 1.��ġ�� ������ �����ϱ� ���� �İ�ü�� �����ؾ� �Ѵ�. 
	// 2. ��ġ�� ������ ����. 
	// 3. �� ������ ���� ��ġ�� �����ϱ� ���� �İ�ü�� ����. 
	LPDIRECT3D9 m_pSDK; // ��ġ�� ������ �����ϴ� �İ�ü. 
	// com��ü��? 
	// ���� . Component Object Model 
	/*
	�İ�ü�� �ܼ��ϰ� ��ǰ�̶�� ������ �ϼ��� .
	�׸��� ����ڴ� �� ��ǰ�� ������ �������� ������� ����� ���� ��.
	*/
	LPDIRECT3DDEVICE9 m_pDevice;// �̰� ���� ���������� ��ġ(GPU)�� �����ϱ� ���� �İ�ü. 
	LPD3DXSPRITE m_pSprite;
	LPD3DXFONT m_pFont;
	LPD3DXLINE m_pLine;

};

