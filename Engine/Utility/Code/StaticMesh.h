#ifndef StaticMesh_h__
#define StaticMesh_h__

#include "Component.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CStaticMesh : public CComponent
{
private:
	explicit CStaticMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CStaticMesh(const CStaticMesh& rhs);
	virtual ~CStaticMesh(void);

public:
	const _vec3*			Get_VtxPos(void) { return m_pVtxPos; }
	const _ulong&			Get_VtxSize(void) { return m_dwVtxSize; }
	const _ulong&			Get_NumVtx(void) { return m_dwNumVtx; }

public:
	HRESULT			Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName);
	void			Render_Meshes(void);
	
private:

	LPD3DXBUFFER		m_pAdjacency;	// ������ �������� ù��° �ּ�
	LPD3DXBUFFER		m_pSubset;		// �޽��� �����ϴ� �������� ���� �Ǵ� ����
										
										// ����� ���� == ������ ���� == �ؽ����� ����
	_ulong				m_dwSubsetCnt;	// ������� ����


	D3DXMATERIAL*		m_pMtrl;		// �޽��� ���� ���� ������ �ؽ����� �̸��� �����ϱ� ���� ����ü ������
	LPDIRECT3DTEXTURE9*	m_ppTexture;	// �ؽ����� �İ�ü, ���� �迭�� �Ҵ��Ͽ� ����� ����

	_ulong				m_dwVtxSize;	// �޽��� ���� ������ ũ��
	_vec3*				m_pVtxPos;		// �޽��� ���� ���� ���� �� ������ ������ �����ϱ� ���� ������(���� �迭�� �Ҵ��� ����)
	_ulong				m_dwNumVtx;		// �޽��� ���� ������ ����

	LPD3DXMESH			m_pOriMesh;		// ������ ������ �޽� ������ ���ϰ� �ִ� �İ�ü
	LPD3DXMESH			m_pMesh;		// �븻������ ���ԵǾ� ������ ���� �޽� �İ�ü


public:
	static CStaticMesh*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName);
	virtual CComponent*		Clone(void);
	virtual void Free(void);
};

END
#endif // StaticMesh_h__
