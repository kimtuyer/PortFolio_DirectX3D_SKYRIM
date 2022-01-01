#ifndef AniCtrl_h__
#define AniCtrl_h__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CAniCtrl : public CBase
{
private:
	explicit CAniCtrl(LPD3DXANIMATIONCONTROLLER pAniCtrl);
	explicit CAniCtrl(const CAniCtrl& rhs);
	virtual ~CAniCtrl(void);

public:
	HRESULT			Ready_AniCtrl(void);
	void			Set_AnimationIndex(const _uint& iIndex);		// ���� ���ϴ� �ִϸ��̼����� ��ü�ϱ� ���� �Լ�
	void			Play_Animation(const _float& fTimeDelta);		// ���������� �ִϸ��̼� ����� ����ϴ� �Լ�
	_bool			Is_AnimationsetFinish(void);
private:
	LPD3DXANIMATIONCONTROLLER			m_pAniCtrl;

	// Ʈ���� �ִ� 2�� �̻� ����(�� ���� ���� ���� ���̸� �ȵȴ�)
	_uint								m_iCurrentTrack;
	_uint								m_iNewTrack;
	
	// �ִϸ��̼��� ����Ǵ� �ð��� ������Ű�� ���� ����
	_float								m_fAccTime;

	// ������ ���õǾ��ִ� �ִϸ��̼� ���� �ε����� �����ϱ� ���� �ִ� ����
	_uint								m_iOldIndex;

	_double								m_dPeriod;

public:
	static CAniCtrl*		Create(LPD3DXANIMATIONCONTROLLER pAniCtrl);
	static CAniCtrl*		Create(const CAniCtrl& rhs);
	virtual void			Free(void);
};

END
#endif // AniCtrl_h__
