#include "DxLib.h"
#include "SoundManager.h"
#include "SaveData.h"

#include <cassert>

namespace
{
	constexpr int kVoluRateOne = 80;	// 音量の基本値
}

SoundManager::SoundManager()
{
	// サウンドデータのロード
	CsvLoad::GetInstance().SoundLoad(m_data);
}

SoundManager::~SoundManager()
{
	/*処理無し*/
}

void SoundManager::Play(const char* name)
{
	// BGMの場合
	if (m_data[name].isBgm)
	{
		// 指定したサウンドが流れていた時は処理を終了する
		if (CheckSoundMem(m_data[name].handle)) return;
		// サウンドを流す(ループ)
		PlaySoundMem(m_data[name].handle, DX_PLAYTYPE_LOOP);

		// 音量調整
		float vol = (static_cast<float>(m_data[name].volRate) / 255.0f);

		ChangeVolumeSoundMem(static_cast<int>(SaveData::GetInstance().GetBGMVol()
			* kVoluRateOne * vol),
			m_data[name].handle);
	}
	// SEの場合
	else if(!m_data[name].isBgm)
	{
		// サウンドを流す(バック)
		PlaySoundMem(m_data[name].handle, DX_PLAYTYPE_BACK);

		// 音量調整
		float vol = (static_cast<float>(m_data[name].volRate) / 255.0f);
		ChangeVolumeSoundMem(static_cast<int>(SaveData::GetInstance().GetSEVol()
			* kVoluRateOne * vol),
			m_data[name].handle);
	}
}

void SoundManager::StopSound()
{
	StopMusic();

	for (auto& handle : m_data)
	{
		// サウンドの停止
		StopSoundMem(handle.second.handle);
	}
}

void SoundManager::StopSE()
{
	for (auto& handle : m_data)
	{
		// BGMの場合は何もしない
		if (handle.second.isBgm) continue;

		// サウンドの停止
		StopSoundMem(handle.second.handle);
	}
}

void SoundManager::DesignationStopSound(std::string str)
{
	// 指定したサウンドの停止
	StopSoundMem(m_data[str].handle);
}

bool SoundManager::IsDesignationCheckPlaySound(std::string str)
{
	// 指定したサウンドが流れているが
	return CheckSoundMem(m_data[str].handle);
}

void SoundManager::ChangeBGMVolume()
{
	for (auto& handle : m_data)	// BGMの音量変更
	{
		// サウンドの種類がBGMではないときは処理をやめる
		if (!handle.second.isBgm)continue;

		ChangeVolumeSoundMem(SaveData::GetInstance().GetBGMVol() * kVoluRateOne,
			handle.second.handle);
	}
}

void SoundManager::ChangeSEVolume()
{
	for (auto& handle : m_data)	// SEの音量変更
	{
		// サウンドの種類がBGMの時(SEではないとき)
		// は処理をやめる
		if (handle.second.isBgm)continue;

		ChangeVolumeSoundMem(SaveData::GetInstance().GetSEVol() * kVoluRateOne,
			handle.second.handle);
	}
}
