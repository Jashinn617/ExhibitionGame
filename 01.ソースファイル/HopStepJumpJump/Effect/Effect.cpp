#include "Effect.h"
#include "../Util/Time.h"

EffectLoopFollow::EffectLoopFollow(const char* FileName, ObjectBase* pObject):
	m_pObject(pObject)
{
	SetData(FileName);
}

EffectLoopFollow::~EffectLoopFollow()
{
	StopEffekseer3DEffect(m_data.playingEffectH);
}

void EffectLoopFollow::Update()
{
	if (m_isEnd)
	{
		return;
	}

	// オブジェクトのポインタがnullになったら
	// エフェクトの再生を終了する
	if (m_pObject == nullptr)
	{
		m_isEnd = true;
		return;
	}

	// オブジェクトが存在していなかったら
	// エフェクトの再生を終了する
	if (!m_pObject->GetInfo().isExist)
	{
		m_isEnd = true;
		return;
	}

	// エフェクトの再生
	Play();

	// エフェクトの角度の設定
	SetRotationPlayingEffekseer3DEffect
		(m_data.playingEffectH,
		m_pObject->GetInfo().rot.x,
		m_pObject->GetInfo().rot.y,
		m_pObject->GetInfo().rot.z);

	// エフェクトの座標の設定
	SetPosPlayingEffekseer3DEffect
		(m_data.playingEffectH,
		m_pObject->GetInfo().pos.x,
		m_pObject->GetInfo().pos.y,
		m_pObject->GetInfo().pos.z);
}

void EffectLoopFollow::Play()
{
	// リザルトの初期化
	int result = 0;

	// エフェクトの再生中は再生しない
	if (IsEffekseer3DEffectPlaying(m_data.playingEffectH) == 0) return;

	// エフェクトの再生
	result = m_data.playingEffectH = PlayEffekseer3DEffect(m_data.resouceH);
	assert(result != -1);

	// エフェクトの大きさの設定
	result = SetScalePlayingEffekseer3DEffect(
		m_data.playingEffectH,
		m_data.size,
		m_data.size,
		m_data.size);
	assert(result != -1);

	// エフェクトの再生速度の設定
	result = SetSpeedPlayingEffekseer3DEffect
	(m_data.playingEffectH, m_data.speed);

	assert(result != -1);
}

EffectFollow::EffectFollow(const char* FileName, ObjectBase* pObject):
	m_pObject(pObject)
{
	SetData(FileName);
	m_data.playingEffectH = PlayEffekseer3DEffect(m_data.resouceH);
	Play();
}

EffectFollow::~EffectFollow()
{
	StopEffekseer3DEffect(m_data.playingEffectH);
}

void EffectFollow::Update()
{
	if (m_isEnd) return;

	// エフェクトの再生が終了したら終了する
	if (IsEffekseer3DEffectPlaying(m_data.playingEffectH) != 0)
	{
		m_isEnd = true;
		return;
	}

	// オブジェクトのポインタがnullになったら
	// エフェクトの再生を終了する
	if (m_pObject == nullptr)
	{
		m_isEnd = true;
		return;
	}

	// オブジェクトが存在していなかったら
	// エフェクトの再生を終了する
	if (!m_pObject->GetInfo().isExist)
	{
		m_isEnd = true;
		return;
	}

	// エフェクトの再生
	Play();

}

void EffectFollow::Play()
{
	// リザルトの初期化
	int result = 0;

	// エフェクトの角度の設定
	SetRotationPlayingEffekseer3DEffect
	(m_data.playingEffectH,
		m_pObject->GetInfo().rot.x,
		m_pObject->GetInfo().rot.y,
		m_pObject->GetInfo().rot.z);

	// エフェクトの座標の設定
	SetPosPlayingEffekseer3DEffect
	(m_data.playingEffectH,
		m_pObject->GetInfo().pos.x,
		m_pObject->GetInfo().pos.y,
		m_pObject->GetInfo().pos.z);

	// エフェクトの大きさの設定
	result = SetScalePlayingEffekseer3DEffect(
		m_data.playingEffectH,
		m_data.size,
		m_data.size,
		m_data.size);
	assert(result != -1);

	// エフェクトの再生速度の設定
	result = SetSpeedPlayingEffekseer3DEffect
	(m_data.playingEffectH, m_data.speed);

	assert(result != -1);
}

EffectLoop::EffectLoop(const char* FileName, VECTOR pos, VECTOR rot):
	m_pos(pos),
	m_rot(rot)
{
	SetData(FileName);
	Play();
}

EffectLoop::~EffectLoop()
{
	StopEffekseer3DEffect(m_data.playingEffectH);
}

void EffectLoop::Update()
{
	if (m_isEnd) return;

	Play();
}

void EffectLoop::Play()
{
	// エフェクトの再生中は再生しない
	if (IsEffekseer3DEffectPlaying(m_data.playingEffectH) == 0) return;

	// リザルトの初期化
	int result = 0;
	// エフェクトの再生
	result = m_data.playingEffectH = PlayEffekseer3DEffect(m_data.resouceH);
	assert(result != -1);
	

	// エフェクトの座標の設定
	SetPosPlayingEffekseer3DEffect
	(m_data.playingEffectH,
		m_pos.x,
		m_pos.y,
		m_pos.z);

	// エフェクトの角度の設定
	SetRotationPlayingEffekseer3DEffect
	(m_data.playingEffectH,
		m_rot.x,
		m_rot.y,
		m_rot.z);

	// エフェクトの大きさの設定
	result = SetScalePlayingEffekseer3DEffect(
		m_data.playingEffectH,
		m_data.size,
		m_data.size,
		m_data.size);
	assert(result != -1);

	// エフェクトの再生速度の設定
	result = SetSpeedPlayingEffekseer3DEffect
	(m_data.playingEffectH, m_data.speed);
	assert(result != -1);
}

EffectNormal::EffectNormal(const char* FileName, VECTOR pos, VECTOR rot):
	m_pos(pos),
	m_rot(rot)
{
	SetData(FileName);
	Play();
}

EffectNormal::~EffectNormal()
{
	StopEffekseer3DEffect(m_data.playingEffectH);
}

void EffectNormal::Update()
{
	if (m_isEnd) return;

	if (IsEffekseer3DEffectPlaying(m_data.playingEffectH) != 0)
	{
		m_isEnd = true;
		return;
	}
}

void EffectNormal::Play()
{
	// リザルトの初期化
	int result = 0;
	// エフェクトの再生
	result = m_data.playingEffectH = PlayEffekseer3DEffect(m_data.resouceH);
	assert(IsEffekseer3DEffectPlaying(m_data.playingEffectH) == 0);


	// エフェクトの座標の設定
	SetPosPlayingEffekseer3DEffect
	(m_data.playingEffectH,
		m_pos.x,
		m_pos.y,
		m_pos.z);

	// エフェクトの角度の設定
	SetRotationPlayingEffekseer3DEffect
	(m_data.playingEffectH,
		m_rot.x,
		m_rot.y,
		m_rot.z);

	// エフェクトの大きさの設定
	result = SetScalePlayingEffekseer3DEffect(
		m_data.playingEffectH,
		m_data.size,
		m_data.size,
		m_data.size);

	// エフェクトの再生速度の設定
	result = SetSpeedPlayingEffekseer3DEffect
	(m_data.playingEffectH, m_data.speed);
	assert(result != -1);
}
