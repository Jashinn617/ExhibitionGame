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
		Option,			// オプション
		GameClear,		// ゲームクリア
		Title,			// タイトル
		StageSelect,	// ステージセレクト
		Tutorial,		// チュートリアル
		Ranking,		// ランキング
		Test,			// テスト

		SceneNum,		// シーン数
	};

public:
	SceneDebug();
	virtual ~SceneDebug();

	virtual void Init() override final;

	virtual std::shared_ptr<SceneBase> Update(Input& input) override final;
	virtual void Draw() override final;
	virtual void End() override final;

	/// <summary>
	/// シーン種類の取得
	/// </summary>
	/// <returns></returns>
	virtual SceneKind GetSceneKind() { return SceneKind::Debug; }

private:	// 関数
	/// <summary>
	/// カーソルのアップデート
	/// </summary>
	void UpdateCursor(Input& input);

	/// <summary>
	/// 次のシーンのアップデート
	/// </summary>
	/// <returns></returns>
	std::shared_ptr<SceneBase> UpdateNextScene();


private:	// 変数
	int m_count;		// カーソルのカウント
	std::array<std::string, static_cast<int>(SceneType::SceneNum)> m_sceneString;	// シーン移動用の文字
};