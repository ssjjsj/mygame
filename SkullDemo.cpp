//***************************************************************************************
// SkullDemo.cpp by Frank Luna (C) 2011 All Rights Reserved.
//
// Demonstrates loading more complicated geometry from file and rendering it.
//
// Controls:
//		Hold the left mouse button down and move the mouse to rotate.
//      Hold the right mouse button down to zoom in and out.
//
//***************************************************************************************

#include "d3dApp.h"
#include "Mesh.h"
#include "sceneManager.h"
#include "global.h"
#include "shaderResource.h"
#include "global.h"
#include "Render.h"
#include "FreeImage.h"
 


class SkullApp : public D3DApp
{
public:
	SkullApp(HINSTANCE hInstance);
	~SkullApp();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene(); 

	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);
	void OnMouseWheel(int delta);
	void OnKeyDown(WPARAM key);

private:
	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;

	float mTheta;
	float mPhi;
	float mRadius;
	float mWheel;

	SceneManager gSceneManager;



	POINT mLastMousePos;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	SkullApp theApp(hInstance);
	
	if( !theApp.Init() )
		return 0;
	
	return theApp.Run();
}
 

SkullApp::SkullApp(HINSTANCE hInstance)
: D3DApp(hInstance)
{
	mPhi = 0.0f;
	mTheta = 0.0f;
	mRadius = 0.0f;
	mWheel = 0.0f;
	mMainWndCaption = L"Skull Demo";
	
	mLastMousePos.x = 0;
	mLastMousePos.y = 0;

	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&mView, I);
	XMStoreFloat4x4(&mProj, I);

	XMMATRIX T = XMMatrixScaling(5.0f, 5.0f, 5.0f);
}

SkullApp::~SkullApp()
{
	FreeImage_DeInitialise();
}

bool SkullApp::Init()
{
	if(!D3DApp::Init())
		return false;

	FreeImage_Initialise(TRUE);

	XMVECTOR pos = XMVectorSet(50.0f, 50.0f, 0.0f, 1.0f);
	XMVECTOR target = XMVectorSet(50.0f, 0.0f, 50.0f, 1.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	//XMMATRIX V = XMMatrixLookAtLH(pos, target, up);
	gRender->getCamera()->LookAt(pos, target, up);
	gRender->getCamera()->UpdateViewMatrix();

	Mesh *m = new Mesh("sponza.obj");
	m->lighted = true;
	m->setMaterial("sponza.material.xml");
	//m->playAnimation("Sinbad");
	gSceneManager.addMesh(m);

	Mesh *sin = new Mesh("Sinbad.mesh.xml");
	sin->lighted = false;
	XMMATRIX t = XMMatrixTranslation(0.0f, 10.0f, 0.0f);
	XMStoreFloat4x4(&sin->localMatrix, t);
	sin->setMaterial("ogre.material.xml");
	//m->playAnimation("Sinbad");
	//gSceneManager.addMesh(sin);



	//gSceneManager.createTerrain();

	float range = 50.0f;
	Light *l = new Light;
	l->ambient = XMFLOAT3(1.0f, 0.0f, 0.0f);
	l->diffuse = XMFLOAT3(1.0f, 0.0f, 0.0f);
	l->specular = XMFLOAT3(1.0f, 0.0f, 0.0f);
	l->pos = XMFLOAT3(0.0f, 10.0f, -30.0f);
	l->k = XMFLOAT3(range, 0.02, 0.02);
	gSceneManager.addLight(l);


	l = new Light();
	l->ambient = XMFLOAT3(0.0f, 1.0f, 0.0f);
	l->diffuse = XMFLOAT3(0.0f, 1.0f, 0.0f);
	l->specular = XMFLOAT3(0.0f, 1.0f, 0.0f);
	l->pos = XMFLOAT3(0.0f, 10.0f, -15.0f);
	l->k = XMFLOAT3(range, 0.02, 0.02);
	gSceneManager.addLight(l);

	l = new Light();
	l->ambient = XMFLOAT3(1.0f, 1.0f, 0.0f);
	l->diffuse = XMFLOAT3(1.0f, 1.0f, 0.0f);
	l->specular = XMFLOAT3(1.0f, 1.0f, 0.0f);
	l->pos = XMFLOAT3(0.0f, 10.0f, 15.0f);
	l->k = XMFLOAT3(range, 0.02, 0.02);
	gSceneManager.addLight(l);

	l = new Light();
	l->ambient = XMFLOAT3(0.0f, 0.0f, 1.0f);
	l->diffuse = XMFLOAT3(0.0f, 0.0f, 1.0f);
	l->specular = XMFLOAT3(0.0f, 0.0f, 1.0f);
	l->pos = XMFLOAT3(0.0f, 10.0f, 30.0f);
	l->k = XMFLOAT3(range, 0.02, 0.02);
	gSceneManager.addLight(l);

	return true;
}

void SkullApp::OnResize()
{
	D3DApp::OnResize();

	//XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f*3.14f, AspectRatio(), 1.0f, 1000.0f);
	//XMStoreFloat4x4(&mProj, P);
}

void SkullApp::UpdateScene(float dt)
{
	gRender->getCamera()->UpdateViewMatrix();
	gSceneManager.update(dt);
	gSceneManager.render();
}

void SkullApp::DrawScene()
{
}

void SkullApp::OnMouseDown(WPARAM btnState, int x, int y)
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;

	SetCapture(mhMainWnd);
}

void SkullApp::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void SkullApp::OnKeyDown(WPARAM key)
{
	Camera *c = gRender->getCamera();
	XMFLOAT3 pos = c->GetPosition();
	XMFLOAT3 d = c->GetLook();
	XMFLOAT3 right = c->GetRight();
	if (key == VK_DOWN)
	{
		c->SetPosition(pos.x-d.x, pos.y-d.y, pos.z - d.z);
	}
	else if (key == VK_RIGHT)
	{
		c->SetPosition(pos.x+right.x, pos.y+right.y, pos.z+right.z);
	}
	else if (key == VK_LEFT)
	{
		c->SetPosition(pos.x - right.x, pos.y-right.y, pos.z-right.z);
	}
	else if (key == VK_UP)
	{
		c->SetPosition(pos.x+d.x, pos.y+d.y, pos.z + d.z);
	}
}

void SkullApp::OnMouseWheel(int delta)
{
	if (delta == 0.0f)
		return;


	if (delta > 0)
		mWheel = delta / 1200000;
	else
		mWheel = delta / 1200000;
	Camera *c = gRender->getCamera();
	XMFLOAT3 pos = c->GetPosition();
	XMFLOAT3 look = c->GetLook();
	XMFLOAT3 newPos;
	newPos.x = pos.x + look.x*mWheel;
	newPos.y = pos.y + look.y*mWheel;
	newPos.z = pos.z + look.z*mWheel;
	c->SetPosition(newPos);
}

void SkullApp::OnMouseMove(WPARAM btnState, int x, int y)
{
	Camera *c = gRender->getCamera();
	if( (btnState & MK_LBUTTON) != 0 )
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f*static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(0.25f*static_cast<float>(y - mLastMousePos.y));

		// Update angles based on input to orbit camera around box.
		mTheta += dx;
		mPhi   += dy;


		c->RotateY(0.07*mTheta);
		c->Pitch(0.07*mPhi);

		// Restrict the angle mPhi.
		//mPhi = MathHelper::Clamp(mPhi, 0.1f, MathHelper::Pi-0.1f);
	}
	else if( (btnState & MK_RBUTTON) != 0 )
	{
		// Make each pixel correspond to 0.2 unit in the scene.
		float dx = 0.05f*static_cast<float>(x - mLastMousePos.x);
		float dy = 0.05f*static_cast<float>(y - mLastMousePos.y);

		XMFLOAT3 pos = c->GetPosition();
		c->SetPosition(pos.x, pos.y + 0.1*dy, pos.z);
	}

	mLastMousePos.x = x;
	mLastMousePos.y = y;
}