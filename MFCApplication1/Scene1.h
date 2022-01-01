#pragma once
#include "Scene.h"
#include "MFC_Terrain.h"
#include "Player.h"


class CDynamicCamera;

class Scene1 :
	public CScene
{
private:
	explicit Scene1(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~Scene1(void);

public:
	virtual HRESULT Ready_Scene(void) override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void Render_Scene(void) override;

	_int Late_Update_Scene(_int Objecttype, int ObjectID);

	_int Load_Update_Scene(ObjectInfo* pInfo);

	int count = 0;


private:
	HRESULT			Ready_Environment_Layer(const _tchar* pLayerTag);
	HRESULT			Ready_GameLogic_Layer(const _tchar* pLayerTag);
	HRESULT			Ready_UI_Layer(const _tchar* pLayerTag);
	CDynamicCamera* m_pCamera = nullptr;

	vector<ObjectInfo*> m_vecStatic_Object;

	int m_vecObjectCount = 0;


public:
	static Scene1*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void);
};