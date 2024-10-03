#pragma once
#include "DxLib.h"
#include <memory>

class Ranking;

/// <summary>
/// 各シーンの基底クラス
/// </summary>
class SceneBase : public std::enable_shared_from_this<SceneBase>
{
public:
	// シーンの種類
	enum class SceneKind
	{
		Debug,			// デバッグ	
		Title,			// タイトル
		Stage,			// ゲームステージ
		StageSelect,	// ステージセレクト
		Ranking,		// ランキング
	};

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneBase();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~SceneBase();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	/// <returns>現在のシーン</returns>
	virtual std::shared_ptr<SceneBase> Update() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// 終了処理
	/// </summary>
	virtual void End() = 0;

	/// <summary>
	/// シーンの種類の取得
	/// </summary>
	/// <returns></returns>
	virtual SceneKind GetSceneKind() = 0;

protected:	// 関数
	/// <summary>
	/// フェードの更新
	/// </summary>
	void UpdateFade();

	/// <summary>
	/// フェードの描画
	/// </summary>
	void DrawFade();

	/// <summary>
	/// フェードインのスキップ
	/// </summary>
	void SkipFadeIn();

	/// <summary>
	/// フェードアウトのスキップ
	/// </summary>
	void SkipFadeOut();

	/// <summary>
	/// フェードイン中か
	/// </summary>
	/// <returns></returns>
	bool IsFadeIn() const;

	/// <summary>
	/// フェードアウト中か
	/// </summary>
	/// <returns></returns>
	bool IsFadeOut() const;

	/// <summary>
	/// フェード中か
	/// </summary>
	/// <returns></returns>
	bool IsFade()const { return IsFadeIn() || IsFadeOut(); }

	/// <summary>
	/// フェードアウトの開始
	/// </summary>
	void StartFadeOut();

	/// <summary>
	/// フェードの明るさの取得
	/// </summary>
	/// <returns></returns>
	int GetFadeBright()const { return m_fadeBright; }

protected:
	std::shared_ptr<Ranking> m_pRanking;	// ランキング

private:	// 変数
	int m_fadeColor;		// フェードの色
	int m_fadeBright;		// フェードの明るさ
	int m_fadeSpeed;		// フェードのスピード
};
