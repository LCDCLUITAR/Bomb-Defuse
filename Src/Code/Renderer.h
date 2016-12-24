/// \file renderer.h 
/// \brief Definition of the renderer class CRenderer.

#pragma once

#include <vector>
#include <D3Dcompiler.h>

#include "WICTextureLoader.h"
#include "defines.h"

using namespace std;
using namespace DirectX;

/// \brief The Renderer.
///
/// The renderer class handles all the nasty Direct3D details associated
/// with general rendering tasks.

class CRenderer{
  friend class CShader;
  friend class C3DSprite;

  protected:
    HRESULT CreateD3DDeviceAndSwapChain(HWND hwnd); ///< Create D3d device.
    HRESULT CreateDepthBuffer(); ///< Create Z buffer.
    HRESULT CreateRasterizer(); ///< Create rasterizer.
    void CreateViewport(); ///< Create viewport.

    void SetWorldMatrix(const Vector3& v=Vector3(0.0f)); ///< Set the world matrix.
    void SetViewMatrix(const Vector3& s, const Vector3& p); ///< Set the view matrix.
    void SetProjectionMatrix(); ///< Set the projection matrix.

    ID3D11RasterizerState1* m_pRasterizerState; ///< Rasterizer state.
    D3D11_RASTERIZER_DESC1 m_rasterizerDesc; ///< Rasterizer description.

  protected:
    ID3D11Device2* m_pDev2; ///< D3D device.
    IDXGISwapChain2* m_pSwapChain2; ///< Swap chain.
    ID3D11DeviceContext2* m_pDC2; ///< Device context.

    ID3D11RenderTargetView* m_pRTV; ///< Render target view.
    ID3D11DepthStencilView* m_pDSV; ///< Depth stencil view.

    XMMATRIX m_matWorld; ///< World matrix.
    XMMATRIX m_matView; ///< View matrix.
    XMMATRIX m_matProj; ///< Projection matrix.
  
  public:
    CRenderer(); ///< Constructor.
    BOOL InitD3D(HINSTANCE hInstance, HWND hwnd); ///< Initialize Direct3D 11.2.
    void LoadTexture(ID3D11ShaderResourceView* &v, char* fname,
      int* w=0, int* h=0); ///< Load texture from a file.
    XMFLOAT4X4 CalculateWorldViewProjectionMatrix(); ///< Compute product of world, view, and projection matrices. 
    void SetWireFrameMode(BOOL on); ///< Turn wireframe mode on or off.
    virtual void Release(); ///< Release D3D stuff.
}; //CRenderer