#include "AniCtrl.h"

USING(Engine)

Engine::CAniCtrl::CAniCtrl(LPD3DXANIMATIONCONTROLLER pAniCtrl)
	: m_pAniCtrl(pAniCtrl)
	, m_iCurrentTrack(0)
	, m_iNewTrack(1)
	, m_fAccTime(0.f)
	, m_iOldIndex(999)
	, m_dPeriod(0.0)

{
	m_pAniCtrl->AddRef();
}

Engine::CAniCtrl::CAniCtrl(const CAniCtrl& rhs)
	: m_iCurrentTrack(rhs.m_iCurrentTrack)
	, m_iNewTrack(rhs.m_iNewTrack)
	, m_fAccTime(rhs.m_fAccTime)
	, m_iOldIndex(rhs.m_iOldIndex)
	, m_dPeriod(0.0)
{
	// �ִϸ��̼� ��Ʈ�ѷ��� �������ִ� �Լ�
	rhs.m_pAniCtrl->CloneAnimationController(rhs.m_pAniCtrl->GetMaxNumAnimationOutputs(),	// ������ ���� ��ü���� �����ϴ� �ִϸ��̼��� ����
										rhs.m_pAniCtrl->GetMaxNumAnimationSets(),		// ���� ������ �ִϸ��̼��� �ִ� ����(��κ� ù ��° ���ڰ��� ���� ��)
										rhs.m_pAniCtrl->GetMaxNumTracks(),				// �ִ� Ʈ���� ����(�츮�� ��¥ ���� ����غ��� �� ������)
										rhs.m_pAniCtrl->GetMaxNumEvents(),				// �ִϸ��̼ǿ� ���Ե� �̺�Ʈ ����(�츮�� ������� ���Ѵ�)
										&m_pAniCtrl);
}

Engine::CAniCtrl::~CAniCtrl(void)
{

}

HRESULT Engine::CAniCtrl::Ready_AniCtrl(void)
{
	return S_OK;
}

void Engine::CAniCtrl::Set_AnimationIndex(const _uint& iIndex)
{
	if (m_iOldIndex == iIndex)
		return;

	m_iNewTrack = (m_iCurrentTrack == 0 ? 1 : 0);
	
	LPD3DXANIMATIONSET		pAS = NULL;
	
	// �ε��� ���� �ش��ϴ� �ִϸ��̼� ������ ������ �Լ�
	m_pAniCtrl->GetAnimationSet(iIndex, &pAS);
	// m_pAniCtrl->GetAnimationSetByName(); // ���ڿ� �Է��� ���� ���ϴ� �ִϸ��̼� ���� ������ �Լ�

	// ���� �ִϸ��̼� ���� ��ü ��� �ð��� ��ȯ�ϴ� �Լ�
	m_dPeriod = pAS->GetPeriod();

	// ������ Ʈ���� �ִϸ��̼� �� ������ ��ġ�ϴ� �Լ�
	m_pAniCtrl->SetTrackAnimationSet(m_iNewTrack, pAS);

	// �ִϸ��̼� ���� ���� �ִ� ������ �̺�Ʈ ������ ������Ѿ���(���� ���� ���� ��� ��� ������ �����ϰ� ��)

	m_pAniCtrl->UnkeyAllTrackEvents(m_iCurrentTrack);
	m_pAniCtrl->UnkeyAllTrackEvents(m_iNewTrack);

	// ������ Ʈ���� Ȱ��ȭ �Ǵ� ��Ȱ��ȭ ���� �����ϰ�, ��� ������ �װ��� ������ �� �����ϴ� �Լ�
	m_pAniCtrl->KeyTrackEnable(m_iCurrentTrack, FALSE, m_fAccTime + 0.25);
	m_pAniCtrl->KeyTrackSpeed(m_iCurrentTrack, 1.f, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);

	m_pAniCtrl->KeyTrackWeight(m_iCurrentTrack, 0.1f, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);

	// Ʈ���� Ȱ��ȭ�ϵ��� �����ϴ� �Լ�
	m_pAniCtrl->SetTrackEnable(m_iNewTrack, TRUE);
	m_pAniCtrl->KeyTrackSpeed(m_iNewTrack, 1.f, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_iNewTrack, 0.9f, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);

	m_pAniCtrl->ResetTime(); // AdvanceTime �Լ� ȣ�� �� ���������� �����ϴ� �ð� ���� �����ϴ� �Լ�
	m_fAccTime = 0.f;

	m_pAniCtrl->SetTrackPosition(m_iNewTrack, 0.0);

	m_iOldIndex = iIndex;
	m_iCurrentTrack = m_iNewTrack;
}

void Engine::CAniCtrl::Play_Animation(const _float& fTimeDelta)
{
	m_pAniCtrl->AdvanceTime(fTimeDelta, NULL);	// ���� �ִϸ��̼��� �����ϴ� �Լ�(2���� : �ִϸ��̼��� ����� �� ���ۿ� ���� ���峪 ����Ʈ�� ���� ó��)
	m_fAccTime += fTimeDelta;
}

_bool CAniCtrl::Is_AnimationsetFinish(void)
{
	D3DXTRACK_DESC			TrackInfo;
	ZeroMemory(&TrackInfo, sizeof(D3DXTRACK_DESC));

	m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &TrackInfo);

	// 0.1�̶� ���� ũ�� �ǹ̰� ����, ������ ������ Ư�� �Ǵ� ���⿡ ���缭 ���� ���� ����
	if (TrackInfo.Position >= m_dPeriod - 0.1)
		return true;
	
	return false;
}

CAniCtrl* Engine::CAniCtrl::Create(LPD3DXANIMATIONCONTROLLER pAniCtrl)
{
	CAniCtrl*	pInstance = new CAniCtrl(pAniCtrl);

	if (FAILED(pInstance->Ready_AniCtrl()))
		Safe_Release(pInstance);

	return pInstance;
}

CAniCtrl* Engine::CAniCtrl::Create(const CAniCtrl& rhs)
{
	CAniCtrl*	pInstance = new	CAniCtrl(rhs);

	if (FAILED(pInstance->Ready_AniCtrl()))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CAniCtrl::Free(void)
{
	Safe_Release(m_pAniCtrl);
}

