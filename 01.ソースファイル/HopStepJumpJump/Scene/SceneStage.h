#pragma
#include "SceneBase.h"
#include "../Util/Game.h"

#include <list>
#include <array>

class StageSceneManager;

class SceneStage : public SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="stage">ステージの種類</param>
	SceneStage(Game::Stage stage);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SceneStage();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() override final;

	/// <summary>
	/// 更新
	/// </summary>
	/// <returns>次のシーン</returns>
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
	/// ステージの種類の取得
	/// </summary>
	/// <returns>ステージの種類</returns>
	virtual SceneKind GetSceneKind()  override final { return SceneKind::Stage; }

private:	// 関数

	/// <summary>
	/// オプションの描画
	/// </summary>
	void DrawOption();

	/// <summary>
	/// オプションの更新
	/// </summary>
	void UpdateOption();

private:	// 変数
	int m_cursorCount;					// オプション時のカーソルカウント
	bool m_isOption;					// オプション中か
	bool m_isSceneEnd;					// シーンが終了したかどうか
	std::array<int, 4> m_optionH;		// オプション画像ハンドル
	std::array<float, 3> m_buttonSize;	// ボタンサイズ
	Game::Stage m_stageKind;			// ステージの種類

	std::shared_ptr<StageSceneManager> m_pStageSceneManager;	// ステージシーンマネージャーポインタ
	std::shared_ptr<SceneBase> m_nextScene;						// 次のシーンのポインタ
};