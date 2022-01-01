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
		



class CStatic_Object : public CGameObject
{


private:
	explicit CStatic_Object(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CStatic_Object(LPDIRECT3DDEVICE9 pGraphicDev, wstring Mesh_proto);

	explicit CStatic_Object(const CStatic_Object& rhs);
	virtual ~CStatic_Object(void);

public:
	 HRESULT Ready_CStatic_Object(wstring Mesh_proto);
	virtual HRESULT Ready_Object(void) override;

	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;
	void Release_CStatic_Object();

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
	vector<ObjectInfo*> m_vecObjectInfo;
	vector<CGameObject*> m_vecStatic_Object;

public:
	CTransform* Get_Transform() {
		return m_pTransformCom;
	}

public:
	void Change_CStatic_Object(const D3DXVECTOR3& vPos, const BYTE& byDrawID, const BYTE& byOption = 0);

private:
	int Get_Index(const D3DXVECTOR3& vPos);
	bool IsPicking(const D3DXVECTOR3& vPos, const int iIndex);


private:
	virtual void		Free(void);

public:
	 HRESULT					Add_Component(wstring Mesh_proto);

	static CStatic_Object*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring Mesh_proto);


private:

};

