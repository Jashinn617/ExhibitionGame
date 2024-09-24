#pragma once
#include <memory>

class SceneBase;
class Input;

/// <summary>
/// シーン遷移等、各シーンを管理するためのクラス
/// </summary>
class SceneManager
{
public:
	SceneManager();
	virtual ~SceneManager();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="handle">ハンドル</param>
	void Init();
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="input">入力</param>
	/// <param name="handle">ハンドル</param>
	bool Update(Input& input);
	/// <summary>
	///	描画
	/// </summary>
	/// <param name="handle">ハンドル</param>
	void Draw();
	/// <summary>
	/// 終了処理
	/// </summary>
	void End();

private:	// 変数
	std::shared_ptr<SceneBase> m_pScene;	// 描画するシーンのポインタ

	float m_updateTime;		// 更新にかかる時間
	float m_drawTime;		// 描画にかかる時間
};
