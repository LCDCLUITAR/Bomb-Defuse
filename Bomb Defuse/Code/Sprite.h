/// \file sprite.h
/// \brief Interface for the base sprite class C3DSprite.

#pragma once

#include <windows.h>
#include <windowsx.h>

#include "defines.h"
#include "Shader.h"

using namespace DirectX;

/// \brief The base sprite. 
///
/// The base sprite contains basic information for managing and drawing a
/// billboard sprite in a 3D world.

class C3DSprite{
  private:
    ID3D11ShaderResourceView* m_pTexture; ///< Pointer to texture containing the sprite image.

    ID3D11Buffer* m_pVertexBuffer; ///< Vertex buffer.
    BILLBOARDVERTEX* m_pVertexBufferData; ///< Vertex buffer data.
    D3D11_BUFFER_DESC m_VertexBufferDesc; ///< Vertex buffer descriptor.
    ID3D11Buffer* m_pConstantBuffer; ///< Constant buffer.
    ID3D11BlendState1* m_pBlendState; ///< Blend state.
    ID3D11RasterizerState1* m_pRasterizerState; ///< Rasterizer state.
    CShader* m_pShader; ///< Pointer to an instance of the shader class.

  public:
    C3DSprite(); ///< Constructor.
    C3DSprite::~C3DSprite(); ///< Destructor.
    BOOL Load(char* filename); ///< Load texture image from file.
    void Draw(const Vector3& p); ///< Draw sprite at point p in 3D space.
    void Release(); ///< Release sprite.
}; //C3DSprite