/// \file renderer.cpp
/// \brief Direct3D rendering.
/// DirectX stuff that won't change much is hidden away in this file
/// so you won't have to keep looking at it.

#include "renderer.h"
#include "defines.h"
#include "abort.h"
#include "debug.h"

extern int g_nScreenWidth;
extern int g_nScreenHeight;

CRenderer::CRenderer():m_pDev2(nullptr){
  m_matWorld = XMMatrixIdentity();
  m_matView = XMMatrixIdentity();
  m_matProj = XMMatrixIdentity();
} //constructor

/// All D3D objects used in the game are released - the release function is kind
/// of like a destructor for DirectX entities, which are COM objects.

void CRenderer::Release(){ 
  SAFE_RELEASE(m_pDC2);
  SAFE_RELEASE(m_pRasterizerState);
  SAFE_RELEASE(m_pSwapChain2);
  SAFE_RELEASE(m_pRTV);
  SAFE_RELEASE(m_pDSV);
  SAFE_RELEASE(m_pDev2);
} //Release

/// Creates a D3D device and swap chain, creates a vertex buffer for the background.
///  \param hInstance Instance handle
///  \param hwnd Window handle
///  \return TRUE if it succeeded

BOOL CRenderer::InitD3D(HINSTANCE hInstance, HWND hwnd){ 
  HRESULT hr; //result from D3D function calls

  //create d3d device and swap chain
  hr = CreateD3DDeviceAndSwapChain(hwnd);
  if(FAILED(hr)){ //bail if failed
    m_pDev2 = nullptr; return FALSE;
  } //if

  //graphics settings
  if(FAILED(CreateDepthBuffer()))
    return FALSE;
  if(FAILED(CreateRasterizer()))
    return FALSE;
  CreateViewport();
  
  //transformation matrices
  SetViewMatrix(Vector3(1024, 384, -350.0f), Vector3(1024, 384, 1000));
  SetProjectionMatrix();
  
  return TRUE; //success exit
} //InitD3D

/// Set and initialize the device, device context, and swap chain.
/// Assign them to the member pointers m_pDev2, m_pDC2, and m_pSwapChain2.
/// \param hwnd Window handle
/// \return The error code returned by D3D

HRESULT CRenderer::CreateD3DDeviceAndSwapChain(HWND hwnd){ 
  //set up device specs data structure
  DXGI_SWAP_CHAIN_DESC d3dscd; //Swap chain specifications.
  ZeroMemory(&d3dscd, sizeof(DXGI_SWAP_CHAIN_DESC)); //null it out
  d3dscd.BufferCount = 1;
  d3dscd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //color mode
  d3dscd.BufferDesc.Height = g_nScreenHeight; //height
  d3dscd.BufferDesc.RefreshRate.Denominator = 1;
  d3dscd.BufferDesc.RefreshRate.Numerator = 60;
  d3dscd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
  d3dscd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
  d3dscd.BufferDesc.Width = g_nScreenWidth; //width
  d3dscd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  d3dscd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
  d3dscd.OutputWindow = hwnd;
  d3dscd.SampleDesc.Count = 1;
  d3dscd.SampleDesc.Quality = 0;
  d3dscd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
  d3dscd.Windowed = TRUE;

  //now we can create the device and swap chain
  
  IDXGISwapChain* pSwapChain; //Swap chain.
  ID3D11Device* pDev; //D3D device.
  ID3D11DeviceContext* pDC; //Device context.
  D3D_FEATURE_LEVEL fl; //Feature level.

  HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
    nullptr, 0, nullptr, 0, D3D11_SDK_VERSION,
    &d3dscd, &pSwapChain, &pDev, &fl, &pDC);
  
  m_pDev2 = (ID3D11Device2*)pDev;
  m_pDC2 = (ID3D11DeviceContext2*)pDC;
  m_pSwapChain2 = (IDXGISwapChain2*)pSwapChain;

  ID3D11Texture2D* backBuffer;
  
  hr = pSwapChain->ResizeBuffers(1, g_nScreenWidth, g_nScreenHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
  hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
  hr = m_pDev2->CreateRenderTargetView(backBuffer, 0, &m_pRTV);
  SAFE_RELEASE(backBuffer);

  return hr;
} //CreateD3DDeviceAndSwapChain

/// Create the Z or depth buffer using a depth stencil.
/// This function assumes that m_pDev2 has been initialized.
/// \return The error code returned by D3D, 1 if m_pDev2 is NULL

HRESULT CRenderer::CreateDepthBuffer(){ 
  if(!m_pDev2)return 1; //bail and fail

  D3D11_TEXTURE2D_DESC depthStencilDesc;
  depthStencilDesc.Width = g_nScreenWidth;
  depthStencilDesc.Height = g_nScreenHeight;
  depthStencilDesc.MipLevels = 1;
  depthStencilDesc.ArraySize = 1;
  depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  depthStencilDesc.SampleDesc.Count = 1; // multisampling must match
  depthStencilDesc.SampleDesc.Quality = 0; // swap chain values.
  depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
  depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
  depthStencilDesc.CPUAccessFlags = 0;
  depthStencilDesc.MiscFlags = 0;

  ID3D11Texture2D* depthStencilBuffer;
  m_pDev2->CreateTexture2D(&depthStencilDesc, 0, &depthStencilBuffer);
  m_pDev2->CreateDepthStencilView(depthStencilBuffer, 0, &m_pDSV);

  m_pDC2->OMSetRenderTargets(1, &m_pRTV, m_pDSV);
  
  ID3D11DepthStencilState* pDepthStencilState;

  D3D11_DEPTH_STENCIL_DESC dhStencilDesc;

  // Depth test parameters
  dhStencilDesc.DepthEnable = true;
  dhStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
  dhStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

  // Stencil test parameters
  dhStencilDesc.StencilEnable = false;
  dhStencilDesc.StencilReadMask = 0xFF;
  dhStencilDesc.StencilWriteMask = 0xFF;

  // Stencil operations if pixel is front-facing
  dhStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  dhStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
  dhStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  dhStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

  // Stencil operations if pixel is back-facing.
  dhStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  dhStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
  dhStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  dhStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

  HRESULT hr = m_pDev2->CreateDepthStencilState(&dhStencilDesc, &pDepthStencilState);
  m_pDC2->OMSetDepthStencilState(pDepthStencilState, 1);

  return hr;
} //CreateDepthBuffer

/// Create the rasterizer and attach it to the device context.
/// This function assumes that m_pDev2 has been initialized.
/// \return The error code returned by D3D, 1 if m_pDev2 is NULL

HRESULT CRenderer::CreateRasterizer(){
  if(!m_pDev2)return 1; //bail and fail

  ZeroMemory(&m_rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC1));
  m_rasterizerDesc.CullMode = D3D11_CULL_BACK;
  m_rasterizerDesc.FillMode = D3D11_FILL_SOLID;
  m_rasterizerDesc.FrontCounterClockwise = false;

  HRESULT hr = m_pDev2->CreateRasterizerState1(&m_rasterizerDesc, &m_pRasterizerState);
  m_pDC2->RSSetState(m_pRasterizerState);

  return hr;
} //CreateRasterizer

/// Create the viewport and attach it to the device context.
/// This function assumes that m_pDev2 has been initialized.

void CRenderer:: CreateViewport(){
  if(!m_pDev2)return; //bail and fail
  
  D3D11_VIEWPORT vp;
  vp.Height = (float)g_nScreenHeight;
  vp.MaxDepth = 1.0f;
  vp.MinDepth = 0.0f;
  vp.TopLeftX = 0;
  vp.TopLeftY = 0;
  vp.Width = (float)g_nScreenWidth;

  m_pDC2->RSSetViewports(1, &vp);
} //CreateViewport

/// Set the world matrix m_matWorld to a translation matrix.
/// \param v Translation vector.

void CRenderer::SetWorldMatrix(const Vector3& v){
  m_matWorld = XMMatrixTranslation(v.x, v.y, v.z);
} //SetWorldMatrix

/// Set the view matrix m_matView to a given position, 
/// in a hard-coded orientation, and direction.
/// \param s Camera position.
/// \param p Camera look-at point.

void CRenderer::SetViewMatrix(const Vector3& s, const Vector3& p){
  m_matView = XMMatrixLookAtLH(s, p, Vector3(0, 1, 0));
} //SetViewMatrix

/// Set the projection matrix m_matProj to a perspective projection matrix
/// with the correct aspect ratio and a 90 degree field of view.

void CRenderer::SetProjectionMatrix(){
  m_matProj = XMMatrixPerspectiveFovLH((float)XM_PI/4.0f, (float)g_nScreenWidth/g_nScreenHeight, 1.0f, 10000.0f);
} //SetProjectionMatrix

/// Compose the world, view, and projection transformations.
/// \return Product of the world, view, and projection matrices.

XMFLOAT4X4 CRenderer::CalculateWorldViewProjectionMatrix(){
  XMFLOAT4X4 f;
  XMStoreFloat4x4(&f, XMMatrixTranspose(m_matWorld * m_matView * m_matProj));
  return f;
} //CalculateWorldViewProjectionMatrix

/// Load an image from a file into a D3D texture. 
/// \param v Pointer to D3D texture to receive the image
/// \param fname Name of the file containing the texture
/// \param w Pointer to a variable that receives the texture width
/// \param h Pointer to a variable that receives the texture width 

void CRenderer::LoadTexture(ID3D11ShaderResourceView* &v, char* fname, int* w, int* h){
  wchar_t  ws[100];
  swprintf(ws, 100, L"%hs", fname);
  CreateWICTextureFromFile(m_pDev2, m_pDC2, ws, nullptr, &v, 0);

  //get texture width and height
  ID3D11Resource* r;
  D3D11_TEXTURE2D_DESC desc;
  v->GetResource(&r);
  ((ID3D11Texture2D*)r)->GetDesc(&desc);

  if(w)*w = desc.Width;
  if(h)*h = desc.Height;
} //LoadTexture

/// Set wireframe mode on or off.
/// \param on TRUE iff wireframe mode is to be turned on. 

void CRenderer::SetWireFrameMode(BOOL on){
  HRESULT hr;
  m_rasterizerDesc.FillMode = on? D3D11_FILL_WIREFRAME: D3D11_FILL_SOLID;
  hr = m_pDev2->CreateRasterizerState1(&m_rasterizerDesc, &m_pRasterizerState);
    
  if(SUCCEEDED(hr))
    m_pDC2->RSSetState(m_pRasterizerState);
} //SetWireFrameMode