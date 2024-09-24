#pragma once
#include "DxLib.h"
#include <memory>

class Input;
class Ranking;

/// <summary>
/// 各シーンの基底クラス
/// </summary>
class SceneBase : public std::enable_shared_from_this<SceneBase>
{
public:
	// シーン遷移時の処理
	enum class Transition
	{
		Leave,		// 残す
		Delete,		// 削除
	};

	// シーンの種類
	enum class SceneKind
	{
		Debug,			// デバッグ	
		Title,			// タイトル
		GameClear,		// ゲームクリア	
		Stage,			// ゲームステージ
		Option,			// オプション
		StageSelect,	// ステージセレクト
		Tutorial,		// チュートリアル
		Ranking,		// ランキング
	};

public:
	SceneBase();
	~SceneBase();

	virtual void Init();
	virtual std::shared_ptr<SceneBase> Update(Input& input);
	virtual void Draw();
	virtual void End();

	/// <summary>
	/// シーンの種類の取得
	/// </summary>
	/// <returns></returns>
	virtual SceneKind GetSceneKind() = 0;

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

	/// <summary>
	/// シーン遷移時の処理の種類の取得
	/// </summary>
	/// <returns></returns>
	virtual Transition GetTransitionType() { return m_transition; }

protected:
	std::shared_ptr<Ranking> m_pRanking;

private:
	int m_fadeColor;		// フェードの色
	int m_fadeBright;		// フェードの明るさ
	int m_fadeSpeed;		// フェードのスピード

	Transition m_transition;	// シーン遷移の種類
};
