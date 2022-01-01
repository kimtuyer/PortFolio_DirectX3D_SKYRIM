#ifndef DynamicCamera_h__
#define DynamicCamera_h__

#include "Camera.h"
#include "Define.h"
#include"Player.h"
class CDynamicCamera : public CCamera
{
private:
	explicit CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDynamicCamera(const CDynamicCamera& rhs);
	virtual ~CDynamicCamera(void);

public:
	HRESULT Ready_Object(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar);
	virtual _int Update_Object(const _float& fTimeDelta) override;




	_vec3			Get_Eye() {
		return m_vEye
			;
	}
	_vec3			Get_At() {
		return m_vAt
			;
	}
	_vec3			Get_Up() {
		return m_vUp
			;
	}

	_matrix Get_Matrix() {
		return m_matView
			;
	}
	

	_bool Get_RotCheck() {
		return m_bRotCheck
			;
	}


	_matrix	Get_RotMatrix() {
		return matRot
			;
	}

	
	_vec3		Get_Rot_Look() {
		return vRot_Look
			;
	}


	void			Set_POV(int i) { m_iPOV = i; }

private:
	void			Key_Input(const _float& fTimeDelta, const _float& fSpeed);
	void			Mouse_Move(const _float& fTimeDelta);
	void			Mouse_Fix(void);
	

private:
	_bool			m_bClick	= false;
	_bool			m_bFix		= true;

	_bool			m_bRotCheck = false;
	_matrix			matRot;

	_vec3			vRot_Look;

	CTransform*				m_pTransformCom = nullptr;


	int			m_iSetPos = 0;
	_vec3		m_vCameraPos;
	_vec3		vLook;
	_bool				_bDimY = false;
	_bool		m_bMoveState = false;	
	int			m_iMoveDir = 1;
	_vec3		m_vCurDir;


	_bool		m_Y1key = false;
	_bool		m_Y2key = false;
	int			m_iPOV = OBJECT_PLAYER;     //OBJECT_PLAYER; //카메라 시점 변환

	float		m_fTime = 0;
public:
	static CDynamicCamera*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
										const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, 
										const _float& fFov, const _float& fAspect, 
										const _float& fNear, const _float& fFar);

private:
	virtual void		Free(void);
};


#endif // DynamicCamera_h__
