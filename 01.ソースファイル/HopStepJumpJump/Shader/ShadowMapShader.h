#pragma once
#include "DxLib.h"

class ShadowMapShader
{
public:
	ShadowMapShader();
	~ShadowMapShader();

	// シャドウマップへの書き込みの開始
	void WriteStart(VECTOR targetPos);

	/// <summary>
	/// シャドウマップのカメラから見たターゲットを設定する
	/// </summary>
	/// <param name="targetPos"></param>
	void SetShadowMapCameraTarget(VECTOR targetPos);

	void SetShader(int shaderType);
	void SetShaderField(VECTOR targetPos);

	/// <summary>
	/// シャドウマップへの書き込みを終わる
	/// </summary>
	void WriteEnd();

private:	// 関数
	/// <summary>
	/// ビュープロジェクション行列を作るための関数
	/// </summary>
	/// <param name="targetPos"></param>
	/// <returns></returns>
	MATRIX ViewProjectionMatrix(VECTOR targetPos);

private:	// 変数
	/*頂点シェーダ*/
	int m_vertexShader1FrameH;				//頂点シェーダー1フレームハンドル
	int m_vertexShader4FrameH;				//頂点シェーダー4フレームハンドル
	int m_vertexShader8FrameH;				//頂点シェーダー8フレームハンドル
	int m_vertexShaderNormal4FrameH;		//頂点シェーダー4フレーム法線マップありハンドル
	int m_vertexShaderNormal8FrameH;		//頂点シェーダー8フレーム法線マップありハンドル
	/*ピクセルシェーダ*/
	int m_vertexShaderShadowMapFieldH;		// 頂点シェーダのシャドウマップをフィールドに貼り付けるための頂点シェーダ
	int m_pixelShaderH;						// ピクセルシェーダのハンドル
	int m_pixelShaderFieldH;				// ピクセルシェーダのフィールドに適用するハンドル

	/*定数バッファのハンドル*/
	int m_cbufferLightDirH;				// ライトの向き
	int m_cbufferCameraTargetPosH;		// カメラのターゲットの位置
	int m_cbufferCameraPosH;			// カメラの位置
	int m_cbufferViewProjectionMatH;	// ビュープロジェクション行列

	/*定数バッファをいじるためのアドレスのコピー*/
	VECTOR* m_pCbufferLightDir;				// ライトの向き
	VECTOR* m_pCbuffferCameraTargetPos;		// カメラのターゲットの位置
	VECTOR* m_pCbufferCameraPos;			// カメラの位置

	/// <summary>
	/// ビュープロジェクション行列
	/// シャドウマップテクスチャをフィールドモデルに
	///	貼り付ける際に座標の変換を行うために必要な行列
	/// </summary>
	MATRIX* m_viewProjectionMat;
	int m_shadowMap;	// シャドウマップ用の変数
};

