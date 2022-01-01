#include "stdafx.h"
#include "Breath.h"

CBreath::CBreath(D3DXVECTOR3 * origin, _vec3 Dir, int numParticles)
{
	vOrgin = *origin;
	size = 0.f;
	vbSize = 2048;
	vbOffset = 0;
	vbBatchSize = 512;

	vDir = Dir;

	for (int i = 0; i < numParticles; i++)
		addParticle();
}

CBreath::~CBreath()
{
}

HRESULT CBreath::init(LPDIRECT3DDEVICE9  device, _tchar * texFileName)
{

	CPSystem::init(device, texFileName);
	return  S_OK;
}



void CBreath::reset()
{
	CPSystem::reset();
}

void CBreath::addParticle()
{
	CPSystem::addParticle();

}

void CBreath::resetParticle(Attribute * attribute)
{
	count++;
	attribute->isAlive = true;

	D3DXVECTOR3 cameraPos;
	//_camera->getPosition(&cameraPos);

	D3DXVECTOR3 cameraDir;
	//_camera->getLook(&cameraDir);

	// change to camera position
	attribute->vPos = vOrgin;
	//attribute->vPos += vTargetPos * count * 0.5;
	//attribute->vPos.x += count * 0.5;
	//attribute->vPos.z += count * 0.5;

	//attribute->vPos.y -= 1.0f; // slightly below camera
							 // so its like we're carrying a gun

	// travels in the direction the camera is looking
	attribute->velocity = vTargetPos * 100.0f;

	// green
	attribute->color = D3DXCOLOR(1, 0, 0.0f, 1.0f);

	attribute->age = 0.0f;
	attribute->lifetime = 3.0f; // lives for 1 seconds

}

void CBreath::Update(float fTimedelta)
{
	CPSystem::Update(fTimedelta);

}

void CBreath::preRender()
{
	CPSystem::preRender();

}

void CBreath::Render()
{
	CPSystem::Render();

}

void CBreath::postRender()
{
	CPSystem::postRender();

}
