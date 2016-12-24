/// \file Shader.h 
/// \brief Interface for the shader class CShader.

#pragma once

#include <D3Dcompiler.h>

#include "defines.h"

/// \brief The shader class.
///
/// The shader class takes care of the management and operation
/// of the vertex and pixel shaders.

class CShader{
  private:
    D3D11_INPUT_ELEMENT_DESC* m_pIEDesc; ///< Input element descriptor array.
    int m_nMaxDescs; ///< Size of input element descriptor array.
    int m_nNumDescs; ///< Next empty slot in input element descriptor array.

    ID3D11InputLayout* m_pInputLayout; ///< Input layout pointer.
  
    ID3D11VertexShader* m_pVertexShader; ///< Vertex shader pointer.
    ID3D11PixelShader* m_pPixelShader; ///< Pixel shader pointer.

  public:
    CShader(int n);
    ~CShader();

    bool AddInputElementDesc(UINT offset, DXGI_FORMAT fmt, LPCSTR name); ///< Add an input element descriptor to the array.

    bool VSCreateAndCompile(LPCWSTR fileName, LPCSTR entryPoint); ///< Create and compile vertex shader.
    bool PSCreateAndCompile(LPCWSTR fileName, LPCSTR entryPoint);///< Create and compile pixel shader.

    void SetShaders(); ///< Set shaders.
}; //CShader