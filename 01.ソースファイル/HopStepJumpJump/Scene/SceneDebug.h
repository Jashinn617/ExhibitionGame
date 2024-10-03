#pragma once
#include "SceneBase.h"

#include <string>
#include <array>

class SceneDebug : public SceneBase
{
public:
	enum class SceneType
	{
		Debug,			// デバッグ
		Stage1,			// ステージ1
		Stage2,			// ステージ2
		Title,			// タイトル
		StageSelect,	// ステージセレクト
		Ranking,		// ランキング
		Test,			// テスト

		SceneNum,		// シーン数
	};

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneDebug();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SceneDebug();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() override final;

	/// <summary>
	/// 更新
	/// </summary>
	/// <returns>現在のシーン</returns>
	virtual std::shared_ptr<SceneBase> Update() override final;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() override final;

	/// <summary>
	/// 終了処理
	/// </summary>
	virtual void End() override final;

	/// <summary>
	/// シーン種類の取得
	/// </summary>
	/// <returns></returns>
	virtual SceneKind GetSceneKind()  override final { return SceneKind::Debug; }

private:	// 関数
	/// <summary>
	/// カーソル更新
	/// </summary>
	void UpdateCursor();

	/// <summary>
	/// 次のシーンの更新
	/// </summary>
	/// <returns>次のシーン</returns>
	std::shared_ptr<SceneBase> UpdateNextScene();


private:	// 変数
	int m_count;		// カーソルのカウント

	// シーン移動用の文字
	std::array<std::string, static_cast<int>(SceneType::SceneNum)> m_sceneString;
};