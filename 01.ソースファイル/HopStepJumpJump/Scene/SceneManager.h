#pragma once
#include <memory>

class SceneBase;

/// <summary>
/// シーン遷移等、各シーンを管理するためのクラス
/// </summary>
class SceneManager
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneManager();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SceneManager();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	bool Update();

	/// <summary>
	///	描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 終了処理
	/// </summary>
	void End();

private:	// 変数
	float m_updateTime;		// 更新にかかる時間
	float m_drawTime;		// 描画にかかる時間

	std::shared_ptr<SceneBase> m_pScene;	// 描画するシーンのポインタ
};
