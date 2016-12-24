cbuffer cbPerObject: register(b0){
  float4x4 gWVP;
}; //cbPerObject

void main(float3 iPosL: POSITION, float2 texC: TEXCOORD,
  out float4 oPosH: SV_POSITION, out float2 texo: TEXCOORD)
{
  oPosH = mul(float4(iPosL, 1.0f), gWVP); //transform to homogeneous clip space
  texo = texC; //just pass vertex color into the pixel shader
} //main