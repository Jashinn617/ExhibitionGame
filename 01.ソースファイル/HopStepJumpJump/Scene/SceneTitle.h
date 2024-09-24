#pragma once
#include "SceneBase.h"

class SkyDome;
class Camera;

class SceneTitle : public SceneBase
{
public:
	SceneTitle();
	virtual ~SceneTitle();

	virtual void Init();

	virtual std::shared_ptr<SceneBase> Update(Input& input);
	virtual void Draw();
	virtual void End();

	virtual SceneKind GetSceneKind() { return SceneKind::Title; }

private:	// 変数
	int m_H1;	// 画像ハンドル
	int m_H2;	// 画像ハンドル
	int m_movieH;	// 動画ハンドル
	int m_TitlePosY;	// タイトル位置
	bool m_IsPlayFallSE;	// タイトルが落ちきったときにSEを鳴らしたかどうか
};

