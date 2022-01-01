#include "VIBuffer.h"

USING(Engine)

Engine::CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
	, m_pVB(nullptr)
	, m_pIB(nullptr)
	, m_dwVtxCnt(0)
	, m_dwVtxSize(0)
	, m_dwTriCnt(0)
	, m_dwFVF(0)
{

}

Engine::CVIBuffer::CVIBuffer(const CVIBuffer& rhs)
	: CComponent(rhs)
	, m_pVB(rhs.m_pVB)
	, m_pIB(rhs.m_pIB)
	, m_dwVtxCnt(rhs.m_dwVtxCnt)
	, m_dwVtxSize(rhs.m_dwVtxSize)
	, m_dwTriCnt(rhs.m_dwTriCnt)
	, m_dwFVF(rhs.m_dwFVF)
{
	m_pVB->AddRef();
	m_pIB->AddRef();
}

Engine::CVIBuffer::~CVIBuffer(void)
{

}

HRESULT Engine::CVIBuffer::Ready_Buffer(void)
{
	// ���ؽ� ���� ����
	if (FAILED(m_pGraphicDev->CreateVertexBuffer(m_dwVtxSize * m_dwVtxCnt,	// �׸����� �ϴ� ������ ������ �뷮(�ﰢ���̴� ���� ���� ũ�⸸ŭ�� �뷮)
													0,					// �������۸� �����(������ �迭 ������ ���), D3DUSAGE_DYNAMIC �ɼ��� �ָ� ��������(��ƼŬ �뵵)
													m_dwFVF,			// ������ �Ӽ� ����
													D3DPOOL_MANAGED,	// ���� ���� ����, MANAGED ���� ���� ��� ��, DEFAULT
													&m_pVB,
													NULL)))				// ������ �ڵ������� �⺻�� NULL				
													return E_FAIL;


	// ���ؽ� ���� ����
	if (FAILED(m_pGraphicDev->CreateIndexBuffer(m_dwIdxSize * m_dwTriCnt,	// �׸����� �ϴ� ������ �ε��� ������ ����� �޸��� �뷮
													0,					// �������۸� �����(������ �迭 ������ ���), D3DUSAGE_DYNAMIC �ɼ��� �ָ� ��������(��ƼŬ �뵵)
													m_IdxFmt,			// �ε����� ����	
													D3DPOOL_MANAGED,	// ���� ���� ����, MANAGED ���� ���� ��� ��, DEFAULT
													&m_pIB,
													NULL)))				// ������ �ڵ������� �⺻�� NULL				
													return E_FAIL;
		

	return S_OK;
}

void Engine::CVIBuffer::Render_Buffer(void)
{
	// ����̽� ��ġ�� ���� �׸������ϴ� ���ؽ� ���۸� �����ϴ� �Լ�
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	// 1. �� �� ���Կ� ���� ����, ����� ���ΰ�
	// 2. ���ؽ����� ������ ��� �ִ� ��ü�� �����̸�, � ���� �Ѱ��� ���ΰ�
	// 3. ��𼭺��� �׸��⸦ ������ ���ΰ�(�޸� �����̱� ������ 0byte����)
	// 4. � ũ��� �׸��⸦ ������ ���ΰ�

	// ���� �׸����� ���ؽ��� �Ӽ��� �����ϴ� �Լ�
	m_pGraphicDev->SetFVF(m_dwFVF);
	// ����̽� ��ġ�� �ε��� ���۸� �������ִ� �Լ�
	
	m_pGraphicDev->SetIndices(m_pIB);


	// ���� �׸��� �Լ�(���ؽ����� �̿��� �׸���)
	// m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_dwTriCnt);
	// 1. �ﰢ���� �׸��� ����� �����ϴ� �÷���
	// 2. �� ��° ���ؽ����� �׸� ���ΰ�
	// 3. �׸������ϴ� �ﰢ���� ����

	// ���� �׸��� �Լ�(�ε��� ���۸� �̿��� �׸���)
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);

}

void Engine::CVIBuffer::Free(void)
{
	Safe_Release(m_pIB);
	Safe_Release(m_pVB);

	CComponent::Free();
}

