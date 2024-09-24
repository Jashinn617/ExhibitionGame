#include "ToonShader.h"
#include "../Util/Game.h"

#include <cassert>
#include <array>


ToonShader::ToonShader()
{

	/*頂点シェーダ*/
	m_vertexShader1FrameH = LoadVertexShader("Data/Shader/Vertex/VertexShader3D.vso");
	assert(m_vertexShader1FrameH != -1);
	m_vertexShader4FrameH = LoadVertexShader("Data/Shader/Vertex/VertexShader3D4Frame.vso");
	assert(m_vertexShader4FrameH != -1);
	m_vertexShader8FrameH = LoadVertexShader("Data/Shader/Vertex/VertexShader3D8Frame.vso");
	assert(m_vertexShader8FrameH != -1);
	m_vertexShaderNormal4FrameH = LoadVertexShader("Data/Shader/Vertex/VertexShader3DNormal4Frame.vso");
	assert(m_vertexShaderNormal4FrameH != -1);
	m_vertexShaderNormal8FrameH = LoadVertexShader("Data/Shader/Vertex/VertexShader3DNormal8Frame.vso");
	assert(m_vertexShaderNormal8FrameH != -1);
	/*ピクセルシェーダ*/
	m_pixelShaderH = LoadPixelShader("Data/Shader/Pixel/ToonPixelShader.pso");
	assert(m_pixelShaderH != -1);
	m_pixelShaderFieldH = LoadPixelShader("Data/Shader/Pixel/ToonPixelShaderField.pso");
	assert(m_pixelShaderFieldH != -1);

	m_graphH = MakeScreen(Game::kScreenWidth, Game::kScreenHeight);
	assert(m_graphH != -1);

	/*ライトの定数バッファの作成*/
	m_cbufferLightDirH = CreateShaderConstantBuffer(sizeof(float) * 4);
	assert(m_cbufferLightDirH != -1);
	// アドレスの取得
	m_pCbufferLightDir = static_cast<VECTOR*>(GetBufferShaderConstantBuffer(m_cbufferLightDirH));

	/*カメラのターゲットの位置の定数バッファの作成*/
	m_cbufferCameraTargetPosH = CreateShaderConstantBuffer(sizeof(float) * 4);
	assert(m_cbufferCameraTargetPosH != -1);
	// アドレスの取得
	m_pCbuffferCameraTargetPos = static_cast<VECTOR*>(GetBufferShaderConstantBuffer(m_cbufferCameraTargetPosH));

	/*カメラの位置の定数バッファの作成*/
	m_cbufferCameraPosH = CreateShaderConstantBuffer(sizeof(float) * 4);
	assert(m_cbufferCameraPosH != -1);
	m_pCbufferCameraPos = static_cast<VECTOR*>(GetBufferShaderConstantBuffer(m_cbufferCameraPosH));
}

ToonShader::~ToonShader()
{
	// シェーダのデリート
	DeleteShader(m_vertexShader1FrameH);
	DeleteShader(m_vertexShader4FrameH);
	DeleteShader(m_vertexShader8FrameH);
	DeleteShader(m_vertexShaderNormal4FrameH);
	DeleteShader(m_vertexShaderNormal8FrameH);

	DeleteShader(m_pixelShaderH);
	DeleteShader(m_pixelShaderFieldH);

	DeleteGraph(m_graphH);

	DeleteShaderConstantBuffer(m_cbufferLightDirH);
	DeleteShaderConstantBuffer(m_cbufferCameraTargetPosH);
	DeleteShaderConstantBuffer(m_cbufferCameraPosH);
}

void ToonShader::Update()
{
	/*処理無し*/
}

void ToonShader::SetShader(int shaderType)
{
	// ライトの向きの取得
	m_pCbufferLightDir[0] = GetLightDirection();
	// カメラのターゲットの位置の取得
	m_pCbuffferCameraTargetPos[0] = GetCameraTarget();
	// カメラの位置の取得
	m_pCbufferCameraPos[0] = GetCameraPosition();

	/*書き込んだ値をGPUに反映する*/
	// ライトの向き
	UpdateShaderConstantBuffer(m_cbufferLightDirH);
	SetShaderConstantBuffer(m_cbufferLightDirH, DX_SHADERTYPE_PIXEL, 0);
	// カメラのターゲットの位置
	UpdateShaderConstantBuffer(m_cbufferCameraTargetPosH);
	SetShaderConstantBuffer(m_cbufferCameraTargetPosH, DX_SHADERTYPE_PIXEL, 1);
	// カメラの位置
	UpdateShaderConstantBuffer(m_cbufferCameraPosH);
	SetShaderConstantBuffer(m_cbufferCameraPosH, DX_SHADERTYPE_PIXEL, 2);

	// シェーダを適用する
	MV1SetUseOrigShader(true);

	//モデルのタイプによって適応させるシェーダーを変える
	if (shaderType == DX_MV1_VERTEX_TYPE_1FRAME)
	{
		SetUseVertexShader(m_vertexShader1FrameH);

	}
	else if (shaderType == DX_MV1_VERTEX_TYPE_4FRAME)
	{
		SetUseVertexShader(m_vertexShader4FrameH);
	}
	else if (shaderType == DX_MV1_VERTEX_TYPE_8FRAME)
	{
		SetUseVertexShader(m_vertexShader8FrameH);
	}
	else if (shaderType == DX_MV1_VERTEX_TYPE_NMAP_1FRAME)
	{
		SetUseVertexShader(m_vertexShader1FrameH);
	}
	else if (shaderType == DX_MV1_VERTEX_TYPE_NMAP_4FRAME)
	{
		SetUseVertexShader(m_vertexShaderNormal4FrameH);
	}
	else if (shaderType == DX_MV1_VERTEX_TYPE_NMAP_8FRAME)
	{
		SetUseVertexShader(m_vertexShaderNormal8FrameH);
	}
	else if (shaderType == DX_MV1_VERTEX_TYPE_NMAP_FREE_FRAME)
	{
		SetUseVertexShader(m_vertexShader1FrameH);
	}
	else
	{
		MV1SetUseOrigShader(false);
	}

	// ピクセルシェーダのセット
	SetUsePixelShader(m_pixelShaderH);
}

void ToonShader::SetShaderField(int shaderType)
{
	// ライトの向きの取得
	m_pCbufferLightDir[0] = GetLightDirection();
	// カメラのターゲットの位置の取得
	m_pCbuffferCameraTargetPos[0] = GetCameraTarget();
	// カメラの位置の取得
	m_pCbufferCameraPos[0] = GetCameraPosition();

	/*書き込んだ値をGPUに反映する*/
	// ライトの向き
	UpdateShaderConstantBuffer(m_cbufferLightDirH);
	SetShaderConstantBuffer(m_cbufferLightDirH, DX_SHADERTYPE_PIXEL, 0);
	// カメラのターゲットの位置
	UpdateShaderConstantBuffer(m_cbufferCameraTargetPosH);
	SetShaderConstantBuffer(m_cbufferCameraTargetPosH, DX_SHADERTYPE_PIXEL, 1);
	// カメラの位置
	UpdateShaderConstantBuffer(m_cbufferCameraPosH);
	SetShaderConstantBuffer(m_cbufferCameraPosH, DX_SHADERTYPE_PIXEL, 2);

	// シェーダを適用する
	MV1SetUseOrigShader(true);

	//モデルのタイプによって適応させるシェーダーを変える
	if (shaderType == DX_MV1_VERTEX_TYPE_1FRAME)
	{
		SetUseVertexShader(m_vertexShader1FrameH);

	}
	else if (shaderType == DX_MV1_VERTEX_TYPE_4FRAME)
	{
		SetUseVertexShader(m_vertexShader4FrameH);
	}
	else if (shaderType == DX_MV1_VERTEX_TYPE_8FRAME)
	{
		SetUseVertexShader(m_vertexShader8FrameH);
	}
	else if (shaderType == DX_MV1_VERTEX_TYPE_NMAP_1FRAME)
	{
		SetUseVertexShader(m_vertexShader1FrameH);
	}
	else if (shaderType == DX_MV1_VERTEX_TYPE_NMAP_4FRAME)
	{
		SetUseVertexShader(m_vertexShaderNormal4FrameH);
	}
	else if (shaderType == DX_MV1_VERTEX_TYPE_NMAP_8FRAME)
	{
		SetUseVertexShader(m_vertexShaderNormal8FrameH);
	}
	else if (shaderType == DX_MV1_VERTEX_TYPE_NMAP_FREE_FRAME)
	{
		SetUseVertexShader(m_vertexShader1FrameH);
	}
	else
	{
		MV1SetUseOrigShader(false);
	}

	// ピクセルシェーダのセット
	SetUsePixelShader(m_pixelShaderFieldH);
}

void ToonShader::ShaderEnd()
{
	/*シェーダを使わない*/
	MV1SetUseOrigShader(false);
}
