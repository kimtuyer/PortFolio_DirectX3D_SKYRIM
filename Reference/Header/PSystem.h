#ifndef PSystem_h__
#define PSystem_h__
#include "Engine_Define.h"
#include "Engine_Struct.h"
#include "PSystem.h"
#include "Base.h"

BEGIN(Engine)





class ENGINE_DLL CPSystem : public CBase
{
	DECLARE_SINGLETON(CPSystem)

public:
	explicit CPSystem(void);
	virtual ~CPSystem(void);

public:
	virtual HRESULT init(LPDIRECT3DDEVICE9 device, _tchar* texFileName);
	virtual void	reset();
	virtual	void	resetParticle(Attribute* attribute) = 0;
	virtual	void	addParticle();

	virtual void	Update(float fTimedelta);

	virtual void	preRender();
	virtual	void	Render();
	virtual	void	postRender();


	bool	isEmpty();
	bool	isDead();

protected:
	virtual void	removeDeadParticles();

	LPDIRECT3DDEVICE9		m_pGraphicDev;
	_vec3					vOrgin;	//��ƼŬ ������ġ
	//BoundingBox			boundingBox;	//��ƼŬ���� ����
	float					emitRate;		//�ʴ� ��ƼŬ ��
	float					size;		
	IDirect3DTexture9*		tex;
	IDirect3DVertexBuffer9*	vb;			
	std::list<Attribute>	particles;
	int						maxParticles;		//�־����ð����� �������ִ� ��ƼŬ�ִ��


	DWORD		vbSize;	//���ؽ� ���۰� �����Ҽ��ִ� ��ƼŬ ��
	DWORD		vbOffset;
	DWORD		vbBatchSize;
	//DWORD		FVF_PTC=0;
public:
	 _ulong							m_dwFVF;	// FVF������

	//HRESULT		Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex);

private:
	//list<CLight*>		m_LightList;

private:
	virtual void	Free(void);
};

END
#endif // LightMgr_h__