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
}

bool SkullApp::Init()
{
	if(!D3DApp::Init())
		return false;

	//Mesh *m = new Mesh("Sinbad.mesh.xml");
	//m->setMaterial("ogre.material.xml");
	//m->playAnimation("Sinbad");
	//gSceneManager.addMesh(m);

	gSceneManager.createTerrain();

	XMVECTOR pos = XMVectorSet(0, 5, 50, 1.0f);
	XMVECTOR target = XMVectorSet(50, 0, 50.0f, 0.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	//XMMATRIX V = XMMatrixLookAtLH(pos, target, up);
	gRender->getCamera()->LookAt(pos, target, up);
	gRender->getCamera()->UpdateViewMatrix();

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
	// Convert Spherical to Cartesian coordinates.
	float x = mRadius*sinf(mPhi)*cosf(mTheta);
	float z = mRadius*sinf(mPhi)*sinf(mTheta);
	float y = mRadius*cosf(mPhi);

	Camera *camera = gRender->getCamera();
	XMFLOAT3 pos = camera->GetPosition();
	camera->SetPosition(pos.x, pos.y - 0.1f*mPhi, pos.z - 0.1f*mTheta);
	camera->UpdateViewMatrix();

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

void SkullApp::OnMouseWheel(int delta)
{
	if (delta > 120)
		mWheel = delta / 120;
	else
		mWheel = -delta / 120;
}

void SkullApp::OnMouseMove(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_MBUTTON) != 0)
	{

	}
	else if( (btnState & MK_LBUTTON) != 0 )
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f*static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(0.25f*static_cast<float>(y - mLastMousePos.y));

		// Update angles based on input to orbit camera around box.
		mTheta += dx;
		mPhi   += dy;

		// Restrict the angle mPhi.
		//mPhi = MathHelper::Clamp(mPhi, 0.1f, MathHelper::Pi-0.1f);
	}
	else if( (btnState & MK_RBUTTON) != 0 )
	{
		// Make each pixel correspond to 0.2 unit in the scene.
		float dx = 0.05f*static_cast<float>(x - mLastMousePos.x);
		float dy = 0.05f*static_cast<float>(y - mLastMousePos.y);

		// Update the camera radius based on input.
		mRadius += dx - dy;

		// Restrict the radius.
		//mRadius = MathHelper::Clamp(mRadius, 5.0f, 50.0f);
	}

	mLastMousePos.x = x;
	mLastMousePos.y = y;
}