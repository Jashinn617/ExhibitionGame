#include "SkyDome.h"
#include "Model.h"

namespace
{
	constexpr float kRotSpeed = 0.0004f;	// 回転速度
	const char* const kSkyDomeFileName = "Data/Model/Sky/Dome.mv1";		// モデルのファイルパス
	const VECTOR kSkyDomeScale = VGet(35.0f, 35.0f, 35.0f);			// モデルのスケール
}

SkyDome::SkyDome():
	m_rot(0.0f)
{
	// モデルポインタの生成
	m_pModel = std::make_shared<Model>(kSkyDomeFileName);

	// スカイドームのスケールを設定する
	m_pModel->SetScale(kSkyDomeScale);
}

SkyDome::~SkyDome()
{
}

void SkyDome::Update(VECTOR pos)
{
	// 回転
	m_rot += kRotSpeed;

	// 一回転したら値をリセットする
	if (m_rot >= DX_PI_F)m_rot = 0.0f;

	// スカイドームの更新
	m_pModel->SetRot(VGet(0.0f, m_rot, 0.0f));
	m_pModel->SetPos(VGet(pos.x, 0.0f, pos.z));
}

void SkyDome::Draw()
{
	m_pModel->Draw();

}
