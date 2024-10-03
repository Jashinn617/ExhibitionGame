#pragma once
#include "SceneBase.h"

class SceneTitle : public SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneTitle();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SceneTitle();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init();

	/// <summary>
	/// 更新
	/// </summary>
	/// <returns>次のシーン</returns>
	virtual std::shared_ptr<SceneBase> Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// 終了処理
	/// </summary>
	virtual void End();

	/// <summary>
	/// シーンの種類の取得
	/// </summary>
	/// <returns>シーンの種類</returns>
	virtual SceneKind GetSceneKind() { return SceneKind::Title; }

private:	// 変数
	int m_titleLogoH;		// タイトルロゴ画像ハンドル
	int m_textH;			// テキスト画像ハンドル
	int m_movieH;			// 動画ハンドル
	int m_titlePosY;		// タイトル位置
	bool m_isPlayFallSE;	// タイトルが落ちきったときにSEを鳴らしたかどうか
};

