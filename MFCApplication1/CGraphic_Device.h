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
	// 장치를 제어하기 위한 과정 -> 
	// 1.장치의 수준을 조사하기 위한 컴객체를 생성해야 한다. 
	// 2. 장치의 수준을 조사. 
	// 3. 그 수준을 토대로 장치를 제어하기 위한 컴객체를 생성. 
	LPDIRECT3D9 m_pSDK; // 장치의 수준을 조사하는 컴객체. 
	// com객체란? 
	// 약자 . Component Object Model 
	/*
	컴객체는 단순하게 부품이라고 생각을 하세요 .
	그리고 사용자는 이 부품을 가지고 최종적인 결과물을 만들어 내는 것.
	*/
	LPDIRECT3DDEVICE9 m_pDevice;// 이게 이제 실질적으로 장치(GPU)를 제어하기 위한 컴객체. 
	LPD3DXSPRITE m_pSprite;
	LPD3DXFONT m_pFont;
	LPD3DXLINE m_pLine;

};

