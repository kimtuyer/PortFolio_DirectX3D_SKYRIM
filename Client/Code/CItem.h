#pragma once
//#include "Engine_Struct.h"

#include "GameObject.h"

BEGIN(Engine)

class CDynamicMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CCollider;
class CNaviMesh;

END
		



class CItem : public CGameObject
{


private:
	explicit CItem(LPDIRECT3DDEVICE9 pGraphicDev);
	//explicit CItem(LPDIRECT3DDEVICE9 pGraphicDev);

	explicit CItem(const CItem& rhs);
	virtual ~CItem(void);

public:
	 HRESULT Ready_CItem(wstring Mesh_proto);
	virtual HRESULT Ready_Object(void) override;

	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;
	void Release_CItem();

private:
	void					SetUp_OnTerrain(void);
	_bool					Collision_ToObject(const _tchar* pLayerTag, const OBJECT_ID pObjTag);

private:
	
private:
	CStaticMesh*			m_pMeshCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CCalculator*			m_pCalculatorCom = nullptr;

	CMFC_Calculator*			m_pCalculatorCom2 = nullptr;



	CCollider*				m_pColliderCom = nullptr;
	COptimization*			m_pOptimizationCom = nullptr;

	_bool					m_bColl = false;
	_uint					m_iFlag;
	_bool					m_bDraw = false;


public:

	BYTE m_byDrawID = 0;
	//vector<ObjectInfo*> m_vecObjectInfo;
	//vector<CGameObject*> m_vecItem;

public:
	CTransform* Get_Transform() {
		return m_pTransformCom;
	}

	vector<ItemInfo> Get_ItemLIst() { return m_vecItemList; }

public:
	void Change_CItem(const D3DXVECTOR3& vPos, const BYTE& byDrawID, const BYTE& byOption = 0);

private:
	int Get_Index(const D3DXVECTOR3& vPos);
	bool IsPicking(const D3DXVECTOR3& vPos, const int iIndex);


private:
	virtual void		Free(void);

public:
	 HRESULT					Add_Component(wstring Mesh_proto);

	static CItem*		Create(LPDIRECT3DDEVICE9 pGraphicDev);


private:


	vector<ItemInfo>	m_vecItemList;
};

