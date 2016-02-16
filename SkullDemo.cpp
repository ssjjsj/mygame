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
#include "d3dx11Effect.h"
#include "GeometryGenerator.h"
#include "MathHelper.h"
#include "Vertex.h"
#include "OrgeMeshPaser.h"
#include "Mesh.h"
 


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

private:
	void BuildGeometryBuffers();
	void BuildFX();
	void BuildVertexLayout();
	void BuildTexture();
	void UpdateGeometryBuffers();

private:
	vector<ID3D11Buffer*> mVB;
	vector<ID3D11Buffer*> mIB;

	ID3DX11Effect* mFX;
	ID3DX11EffectTechnique* mTech;
	ID3DX11EffectMatrixVariable* mfxWorldViewProj;
	ID3DX11EffectShaderResourceVariable *diffuseMap;
	map<string, ID3D11ShaderResourceView*> mDiffuseMap;
	vector<Matrial> matrialList;

	ID3D11RasterizerState* mWireframeRS;

	// Define transformations from local spaces to world space.
	XMFLOAT4X4 mSkullWorld;

	UINT mSkullIndexCount;

	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;

	float mTheta;
	float mPhi;
	float mRadius;

	Mesh *m;

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
: D3DApp(hInstance), mVB(0), mIB(0), mFX(0), mTech(0),
  mfxWorldViewProj(0), mWireframeRS(0), mSkullIndexCount(0),
  mTheta(1.5f*MathHelper::Pi), mPhi(0.1f*MathHelper::Pi), mRadius(20.0f)
{
	mMainWndCaption = L"Skull Demo";
	
	mLastMousePos.x = 0;
	mLastMousePos.y = 0;

	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&mView, I);
	XMStoreFloat4x4(&mProj, I);

	XMMATRIX T = XMMatrixScaling(5.0f, 5.0f, 5.0f);
	XMStoreFloat4x4(&mSkullWorld, T);
}

SkullApp::~SkullApp()
{
	for (int i = 0; i < mVB.size(); i++)
	{
		ReleaseCOM(mVB[i]);
		ReleaseCOM(mIB[i]);
	}
	ReleaseCOM(mFX);
	ReleaseCOM(mWireframeRS);
	delete m;
	//FreeConsole();
}

bool SkullApp::Init()
{
	if(!D3DApp::Init())
		return false;

	//AllocConsole();
	//freopen("CONOUT$", "w+t", stdout);

	Matrial m;
	m.matrialName = "Sinbad/Body";
	m.texName = L"sinbad_body.dds";
	matrialList.push_back(m);

	m.matrialName = "Sinbad/Gold";
	m.texName = L"sinbad_clothes.dds";
	matrialList.push_back(m);

	m.matrialName = "Sinbad/Teeth";
	m.texName = L"sinbad_body.dds";
	matrialList.push_back(m);

	m.matrialName = "Sinbad/Sheaths";
	m.texName = L"sinbad_sword.dds";
	matrialList.push_back(m);

	m.matrialName = "Sinbad/Clothes";
	m.texName = L"sinbad_clothes.dds";
	matrialList.push_back(m);

	m.matrialName = "Sinbad/Eyes";
	m.texName = L"sinbad_body.dds";
	matrialList.push_back(m);

	
	m.matrialName = "Cat";
	m.texName = L"cat_pet.dds";
	matrialList.push_back(m);

	BuildTexture();
	BuildGeometryBuffers();
	BuildFX();
	BuildVertexLayout();

	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireframeDesc.CullMode = D3D11_CULL_BACK;
	wireframeDesc.FrontCounterClockwise = false;
	wireframeDesc.DepthClipEnable = true;

	//HR(md3dDevice->CreateRasterizerState(&wireframeDesc, &mWireframeRS));

	return true;
}

void SkullApp::OnResize()
{
	D3DApp::OnResize();

	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f*MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);
	XMStoreFloat4x4(&mProj, P);
}

void SkullApp::UpdateScene(float dt)
{
	// Convert Spherical to Cartesian coordinates.
	float x = mRadius*sinf(mPhi)*cosf(mTheta);
	float z = mRadius*sinf(mPhi)*sinf(mTheta);
	float y = mRadius*cosf(mPhi);

	// Build the view matrix.
	XMVECTOR pos    = XMVectorSet(x, y, z, 1.0f);
	XMVECTOR target = XMVectorZero();
	XMVECTOR up     = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX V = XMMatrixLookAtLH(pos, target, up);
	XMStoreFloat4x4(&mView, V);

	if (m != NULL)
	{
		if (!m->IsPlayAnimation())
			m->playAnimation("Sinbad");
		m->update(dt);
	}
}

void SkullApp::DrawScene()
{
	//UpdateGeometryBuffers();
	//md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&Colors::LightSteelBlue));
	//md3dImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);

	////md3dImmediateContext->RSSetState(mWireframeRS);

	//// Set constants

	//XMMATRIX view = XMLoadFloat4x4(&mView);
	//XMMATRIX proj = XMLoadFloat4x4(&mProj);
	//XMMATRIX world = XMLoadFloat4x4(&mSkullWorld);
	//XMMATRIX worldViewProj = world*view*proj;
	//mfxWorldViewProj->SetMatrix(reinterpret_cast<float*>(&worldViewProj));
 //
	//vector<MyVertex::ModelData>& datas = m->getModelData();
	//for (int i = 0; i <mVB.size(); i++)
	//{
	//	mSkullIndexCount = datas[i].indexs.size();

	//	md3dImmediateContext->IASetInputLayout(mInputLayout);
	//	md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//	UINT stride = sizeof(MyVertex::Vertex);
	//	UINT offset = 0;
	//	md3dImmediateContext->IASetVertexBuffers(0, 1, &mVB[i], &stride, &offset);
	//	md3dImmediateContext->IASetIndexBuffer(mIB[i], DXGI_FORMAT_R32_UINT, 0);

	//	diffuseMap->SetResource(mDiffuseMap[datas[i].materialName]);

	//	D3DX11_TECHNIQUE_DESC techDesc;
	//	mTech->GetDesc(&techDesc);
	//	for (UINT p = 0; p < techDesc.Passes; ++p)
	//	{
	//		mTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
	//		md3dImmediateContext->DrawIndexed(mSkullIndexCount, 0, 0);
	//	}
	//}

	//HR(mSwapChain->Present(0, 0));
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

void SkullApp::OnMouseMove(WPARAM btnState, int x, int y)
{
	if( (btnState & MK_LBUTTON) != 0 )
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f*static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(0.25f*static_cast<float>(y - mLastMousePos.y));

		// Update angles based on input to orbit camera around box.
		mTheta += dx;
		mPhi   += dy;

		// Restrict the angle mPhi.
		mPhi = MathHelper::Clamp(mPhi, 0.1f, MathHelper::Pi-0.1f);
	}
	else if( (btnState & MK_RBUTTON) != 0 )
	{
		// Make each pixel correspond to 0.2 unit in the scene.
		float dx = 0.05f*static_cast<float>(x - mLastMousePos.x);
		float dy = 0.05f*static_cast<float>(y - mLastMousePos.y);

		// Update the camera radius based on input.
		mRadius += dx - dy;

		// Restrict the radius.
		mRadius = MathHelper::Clamp(mRadius, 5.0f, 50.0f);
	}

	mLastMousePos.x = x;
	mLastMousePos.y = y;
}


void SkullApp::UpdateGeometryBuffers()
{
	//vector<MyVertex::ModelData>& datas = m->getModelData();

	//if (datas.size() == 0)
	//	return;

	//for (int i = 0; i < datas.size(); i++)
	//{
	//	std::vector<MyVertex::Vertex>& vertices = datas[i].vertexs;

	//	UINT vcount = vertices.size();
	//	D3D11_MAPPED_SUBRESOURCE vResource;
	//	HRESULT hResult = md3dImmediateContext->Map(mVB[i], 0,
	//		D3D11_MAP_WRITE_DISCARD, 0, &vResource);

	//	if (FAILED(hResult))
	//	{
	//		int i = 0;
	//	}

	//	MyVertex::Vertex* v = (MyVertex::Vertex*)(vResource.pData);
	//	for (int j = 0; j < vertices.size(); j++)
	//	{
	//		v[j] = vertices[j];
	//	}

	//	md3dImmediateContext->Unmap(mVB[i], 0);
	//}
}
 
void SkullApp::BuildGeometryBuffers()
{
	//m= new Mesh("Sinbad.mesh.xml");
	////m->playAnimation("Sinbad");
	////m->update(1.0f);
	//vector<MyVertex::ModelData>& datas = m->getModelData();



	//for (int i = 0; i < datas.size(); i++)
	//{

	//	std::vector<UINT> indices = datas[i].indexs;

	//	std::vector<MyVertex::Vertex> vertices = datas[i].vertexs;
	//	UINT vcount = vertices.size();
	//	UINT tcount = indices.size();
	//	mSkullIndexCount = tcount;

	//	ID3D11Buffer *VB;
	//	D3D11_BUFFER_DESC vbd;
	//	vbd.Usage = D3D11_USAGE_DYNAMIC;
	//	vbd.ByteWidth = sizeof(MyVertex::Vertex) * vcount;
	//	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//	vbd.MiscFlags = 0;
	//	D3D11_SUBRESOURCE_DATA vinitData;
	//	vinitData.pSysMem = &vertices[0];
	//	HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &VB));
	//	mVB.push_back(VB);

	//	//
	//	// Pack the indices of all the meshes into one index buffer.
	//	//

	//	ID3D11Buffer *IB;
	//	D3D11_BUFFER_DESC ibd;
	//	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	//	ibd.ByteWidth = sizeof(UINT) * tcount;
	//	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//	ibd.CPUAccessFlags = 0;
	//	ibd.MiscFlags = 0;
	//	D3D11_SUBRESOURCE_DATA iinitData;
	//	iinitData.pSysMem = &indices[0];
	//	HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &IB));
	//	mIB.push_back(IB);
	//}
}
 
void SkullApp::BuildFX()
{
	//std::ifstream fin("fx/color.fxo", std::ios::binary);

	//fin.seekg(0, std::ios_base::end);
	//int size = (int)fin.tellg();
	//fin.seekg(0, std::ios_base::beg);
	//std::vector<char> compiledShader(size);

	//fin.read(&compiledShader[0], size);
	//fin.close();
	//
	//HR(D3DX11CreateEffectFromMemory(&compiledShader[0], size, 
	//	0, md3dDevice, &mFX));

	//mTech    = mFX->GetTechniqueByName("ColorTech");
	//mfxWorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	//diffuseMap = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
}

void SkullApp::BuildTexture()
{
	//for (int i = 0; i < matrialList.size(); i++)
	//{
	//	ID3D11ShaderResourceView *tex;
	//	D3DX11CreateShaderResourceViewFromFile(md3dDevice, matrialList[i].texName, NULL, NULL, &tex, NULL);
	//	mDiffuseMap[matrialList[i].matrialName] = tex;
	//}
}

void SkullApp::BuildVertexLayout()
{
	//// Create the vertex input layout.
	//D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	//{
	//	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	//	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
	//	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	//};

	//// Create the input layout
 //   D3DX11_PASS_DESC passDesc;
 //   mTech->GetPassByIndex(0)->GetDesc(&passDesc);
	//HR(md3dDevice->CreateInputLayout(vertexDesc, 4, passDesc.pIAInputSignature, 
	//	passDesc.IAInputSignatureSize, &mInputLayout));
}