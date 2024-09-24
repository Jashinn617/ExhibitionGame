#pragma
#include "SceneBase.h"
#include "../Util/Game.h"

#include <list>
#include <array>

class StageSceneManager;
class Input;


class SceneStage : public SceneBase
{
public:
	SceneStage(Game::Stage stage);
	virtual ~SceneStage();

	virtual void Init() override final;
	virtual std::shared_ptr<SceneBase> Update(Input& input) override final;
	virtual void Draw() override final;
	virtual void End() override final;

	virtual SceneKind GetSceneKind() { return SceneKind::Stage; }

private:
	void DrawOption();

	void UpdateOption();

private:
	void GameClear();

private:
	int m_count;

	bool m_isOption;	// オプション中か
	bool m_isSceneEnd;		// シーンが終了したかどうか

	std::array<int, 4> m_optionH;
	std::array<float, 3> m_buttonSize;

	Game::Stage m_stageKind;	// ステージの種類

	std::shared_ptr<StageSceneManager> m_pStageSceneManager;
	std::shared_ptr<SceneBase> m_nextScene;		// 次のシーンのポインタ
};