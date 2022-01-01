#include "PSystem.h"


USING(Engine)




Engine::CPSystem::CPSystem(void)
{
}


Engine::CPSystem::~CPSystem(void)
{
}

HRESULT Engine::CPSystem::init(LPDIRECT3DDEVICE9  device, _tchar* texFileName)
{
	m_dwFVF = D3DFVF_XYZ; //| D3DFVF_DIFFUSE; //| D3DFVF_PSIZE;

	if (FAILED(device->CreateVertexBuffer(
		vbSize * sizeof(Particle),
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
		m_dwFVF,
		D3DPOOL_DEFAULT,
		&vb, 0)))
		return E_FAIL;

	if (FAILED(D3DXCreateTextureFromFile(
		device,
		texFileName,
		&tex)))
		return E_FAIL;

	m_pGraphicDev = device;


	return  S_OK;
}

void Engine:: CPSystem::reset()
{

	std::list<Attribute>::iterator i;
	for (i = particles.begin(); i != particles.end(); i++)
	{
		resetParticle(&(*i));
		
	}
}

void CPSystem::resetParticle(Attribute * attribute)
{
	//attribute->isAlive = true;
	//
	//D3DXVECTOR3 cameraPos;
	////_camera->getPosition(&cameraPos);
	//
	//D3DXVECTOR3 cameraDir;
	////_camera->getLook(&cameraDir);
	//
	//// change to camera position
	//attribute->vPos = cameraPos;
	//
	//
	//attribute->velocity = cameraDir * 100.0f;
	//
	//
	//attribute->color = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	//
	//
	//attribute->age = 0.0f;
	//attribute->lifetime = 2.0f; // lives for 2 seconds


}

void CPSystem::addParticle()
{
	Attribute attribute;

	//reset();
	resetParticle(&attribute);

	particles.push_back(attribute);
}

void CPSystem::Update(float fTimedelta)
{


	list<Attribute>::iterator i;

	for (i = particles.begin(); i != particles.end(); i++)
	{
		i->vPos += i->velocity*fTimedelta;

		i->age += fTimedelta;

		if (i->age > i->lifetime)
			i->isAlive = false;

	}

	removeDeadParticles();


}

void CPSystem::preRender()
{
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, true);

	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, true);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);


	m_pGraphicDev->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALEENABLE, true);

	m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE, DWORD(32));
	
	//m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE_MIN, 100);
	//m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE_MAX, 1);

	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_A, 0.0f);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_B, 0.0f);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_C, 1.0f);
	
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x00000077); //0x00000077 //0xc0


	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	


	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//








}

void CPSystem::Render()
{

	if (!particles.empty())
	{

		preRender();

		m_pGraphicDev->SetTexture(0, tex);
		m_pGraphicDev->SetFVF(m_dwFVF);
		m_pGraphicDev->SetStreamSource(0, vb, 0, sizeof(Particle));



		if (vbOffset >= vbSize)
			vbOffset = 0;
		Particle* v = 0;

		vb->Lock(

			vbOffset * sizeof(Particle),
			vbBatchSize * sizeof(Particle),
			(void**)&v,
			vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD



		);
		DWORD numParticlesInBatch = 0;

		list<Attribute>::iterator i;
		for (i = particles.begin(); i != particles.end(); i++)
		{

			if (i->isAlive)
			{
				v->vPos = i->vPos;
				v->color = (D3DCOLOR)i->color;
				v++;
				numParticlesInBatch++;

				if (numParticlesInBatch == vbBatchSize)
				{


					vb->Unlock();

					m_pGraphicDev->DrawPrimitive(D3DPT_POINTLIST, vbOffset, vbBatchSize);



					vbOffset += vbBatchSize;


					if (vbOffset >= vbSize)
						vbOffset = 0;

					vb->Lock(

						vbOffset * sizeof(Particle),
						vbBatchSize * sizeof(Particle),
						(void**)&v,
						vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD


					);

					numParticlesInBatch = 0;



				}



			}


		}

		vb->Unlock();

		if (numParticlesInBatch)
		{



			m_pGraphicDev->DrawPrimitive(D3DPT_POINTLIST, vbOffset, vbBatchSize);
		}

		vbOffset += vbBatchSize;

		postRender();



	}

}

void CPSystem::postRender()
{
	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, false);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALEENABLE, false);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

bool CPSystem::isEmpty()
{
	if (particles.empty())
		return true;

	else
		return false;
}

bool CPSystem::isDead()
{
	std::list<Attribute>::iterator i;
	for (i = particles.begin(); i != particles.end(); i++)
	{
		// is there at least one living particle?  If yes,
		// the system is not dead.
		if (i->isAlive)
			return false;
	}
	// no living particles found, the system must be dead.
	return true;
}

void CPSystem::removeDeadParticles()
{

	list<Attribute>::iterator i;

	i = particles.begin();

	while (i != particles.end())
	{
	
		if (i->isAlive == false)
		{

			i = particles.erase(i);

		}
		else
		{

			i++;

		}



	}



}



void CPSystem::Free(void)
{
		
}
