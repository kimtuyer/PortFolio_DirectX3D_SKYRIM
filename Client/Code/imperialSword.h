#ifndef imperialSword_h__
#define imperialSword_h__

#include "GameObject.h"
#include "Define.h"
#include "GuardMan.h"
BEGIN(Engine)

class CStaticMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CCollider;
class CColliderSphere;

END
class CimperialSword : public CGameObject
{
private:
	explicit CimperialSword(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CimperialSword(const CimperialSword& rhs);
	virtual ~CimperialSword(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

	CColliderSphere*	Get_ColliderCom() {
		return m_pColliderCom
			;
	}

	void	Set_IndexNumber(int i) { m_iIndex_number = i; }
	int		Get_IndexNumber() { return m_iIndex_number; }
	void	Set_GuardMan(CGameObject* p) { m_pGuardMan = p; }


private:
	HRESULT					Add_Component(void);
	_bool					Collision_ToObject(const _tchar* pLayerTag, const OBJECT_ID pObjTag);

private:	
	CStaticMesh*			m_pMeshCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CCalculator*			m_pCalculatorCom = nullptr;
	CColliderSphere*		m_pColliderCom = nullptr;

	const _matrix*			m_pParentBoneMatrix = nullptr;
	const _matrix*			m_pParentWorldMatrix = nullptr;

	CGameObject*			m_pGuardMan = nullptr;

	int						m_iIndex_number;


public:
	static CimperialSword*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);
};


#endif // imperialSword_h__
