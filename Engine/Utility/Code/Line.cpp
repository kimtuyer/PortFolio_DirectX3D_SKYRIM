#include "Line.h"

USING(Engine)

Engine::CLine::CLine(void)
{

}

Engine::CLine::~CLine(void)
{

}

HRESULT Engine::CLine::Ready_Line(const _vec2* pPointA, const _vec2* pPointB)
{
	m_vPoint[POINT_START] = *pPointA;
	m_vPoint[POINT_FINISH] = *pPointB;

	m_vDirection = m_vPoint[POINT_FINISH] - m_vPoint[POINT_START];
	m_vNormal = _vec2(m_vDirection.y *-1.f, m_vDirection.x); //2������, ������ ��ǥ�� ����, x y ��ǥ�� �ٲ۰�(����)
	D3DXVec2Normalize(&m_vNormal, &m_vNormal);

	return S_OK;
}

Engine::CLine::COMPARE Engine::CLine::Compare(const _vec2* pEndPos)//�÷��̾� ��ġ
{
	_vec2		vDir = *pEndPos - m_vPoint[POINT_START]; //

	_float		fDot = D3DXVec2Dot(&m_vNormal, D3DXVec2Normalize(&vDir, &vDir));

	if (0.f <= fDot)
		return COMPARE_OUT;

	else
		return COMPARE_IN;
}

CLine* Engine::CLine::Create(const _vec2* pPointA, const _vec2* pPointB)
{
	CLine*		pInstance = new CLine;

	if (FAILED(pInstance->Ready_Line(pPointA, pPointB)))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CLine::Free(void)
{

}

