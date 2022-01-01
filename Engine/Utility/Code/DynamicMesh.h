#ifndef DynamicMesh_h__
#define DynamicMesh_h__

#include "Component.h"
#include "HierarchyLoader.h"
#include "AniCtrl.h"

BEGIN(Engine)

class ENGINE_DLL CDynamicMesh : public CComponent
{
private:
	explicit CDynamicMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDynamicMesh(const CDynamicMesh& rhs);
	virtual ~CDynamicMesh(void);
public:
	const _vec3*			Get_VtxPos(void) { return m_pVtxPos; }
	const _ulong&			Get_VtxSize(void) { return m_dwVtxSize; }
	const _ulong&			Get_NumVtx(void) { return m_dwNumVtx; }

public:
	void			Set_AnimationIndex(const _uint& iIndex);	
	void			Play_Animation(const _float& fTimeDelta);	
	const	D3DXFRAME_DERIVED*		Get_FrameByName(char* pFrameName);
	_bool			Is_AnimationsetFinish(void);
public:
	HRESULT		Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName);
	void		Render_Meshes(void);

private:
	// �� �����Ӹ��� ��� ������ ��ȸ�ϸ鼭 ������ ������ �ִ� ����� ��ȯ�ϱ� ���� �Լ�
	void		Update_FrameMatrices(D3DXFRAME_DERIVED* pFrame, const _matrix* pParentMatrix);
	void		SetUp_FrameMatrixPointer(D3DXFRAME_DERIVED* pFrame);


private:
	D3DXFRAME*								m_pRootFrame;
	CHierarchyLoader*						m_pLoader = nullptr;
	list<D3DXMESHCONTAINER_DERIVED*>		m_MeshcontainerList;

	CAniCtrl*								m_pAniCtrl;

	_ulong				m_dwVtxSize;	// �޽��� ���� ������ ũ��
	_vec3*				m_pVtxPos;		// �޽��� ���� ���� ���� �� ������ ������ �����ϱ� ���� ������(���� �迭�� �Ҵ��� ����)
	_ulong				m_dwNumVtx;		// �޽��� ���� ������ ����
	
public:
	static CDynamicMesh*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName);
	virtual CComponent*		Clone(void);
	virtual void			Free(void);	
};

END
#endif // DynamicMesh_h__

// �޽��� �̷�� �� ������ �����ϱ� ���� ����ü
//typedef struct _D3DXFRAME
//{
//	LPSTR                   Name;			// ���� �̸��� �����ϱ� ���� ���ڿ� ������(��Ƽ ����Ʈ ���)
//	D3DXMATRIX              TransformationMatrix;	// ���� �ε� ������ ���� ��� ����
//
//	LPD3DXMESHCONTAINER     pMeshContainer;		// ����ƽ �޽� ����
//
//	struct _D3DXFRAME       *pFrameSibling;		// ���� ���� ������ �����ϱ� ���� ������
//	struct _D3DXFRAME       *pFrameFirstChild;	// �ڽ� ���� ������ �����ϱ� ���� ������
//} D3DXFRAME, *LPD3DXFRAME;
//
//// �޽��� ���� ������ ��� �ִ� ���� ����ü(����ƽ �޽�)
//typedef struct _D3DXMESHCONTAINER
//{
//	LPSTR                   Name;		// �޽� �����̳��� �̸��� �����ϱ� ���� ���ڿ� ������(���� ������� �ʰų� ��� ����)
//
//	D3DXMESHDATA            MeshData;	// �޽��� ������ �����ϰ� �޽� ������ ��� ���� �İ�ü�� ������ ����ü
//
//	LPD3DXMATERIAL          pMaterials;	// ����ƽ �޽����� D3DXMATERIAL* �� ���� �༮(�������� + �ؽ����� �̸�)
//	LPD3DXEFFECTINSTANCE    pEffects;	// �޽��� ���� ������ ����Ʈ ������ ��� �ִ� �İ�ü(�츮�� ����� �� ����)
//	DWORD                   NumMaterials;	// ������ ���� == ������� ���� == �ؽ����� ����
//	DWORD                  *pAdjacency;		// ������ �������� ������ ��ȸ�ϱ� ���� ù��° �ּҸ� �����ϱ� ���� ������
//
//	LPD3DXSKININFO          pSkinInfo;		// ��Ű�� �ִϸ��̼��� �����ϱ� ���� ���� ����� �����ϴ� �İ�ü
//
//	struct _D3DXMESHCONTAINER *pNextMeshContainer;		// ���� �޽������̳��� �ּҰ��� �����ϱ� ���� ������
//
//} D3DXMESHCONTAINER, *LPD3DXMESHCONTAINER;