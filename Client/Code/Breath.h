
#include "PSystem.h"



class CBreath : public CPSystem

{


public:
	CBreath(D3DXVECTOR3* origin, _vec3 Dir, int numParticles);
	~CBreath();


	virtual HRESULT init(LPDIRECT3DDEVICE9 device, _tchar* texFileName);
	virtual void	reset();
	virtual	void	addParticle();


	// CPSystem을(를) 통해 상속됨
	virtual void resetParticle(Attribute * attribute) override;

	virtual void	Update(float fTimedelta);

	virtual void	preRender();
	virtual	void	Render();
	virtual	void	postRender();


	void		Set_Dir(_vec3 dir) { vDir = dir; }
	void		Set_Target(_vec3 vPos) { vTargetPos = vPos; }
private:
	_vec3		vDir;
	_vec3		vTargetPos;
	int		count = 0;


};