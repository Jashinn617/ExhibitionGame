#pragma once
#include "CharacterData.h"
#include "CsvLoad.h"

#include <unordered_map>
#include <string>

using namespace CharacterData;

/// <summary>
/// サウンドの管理を行うクラス
/// シングルトンクラス
/// </summary>
class SoundManager
{
public:
	~SoundManager();

	static SoundManager& GetInstance()
	{
		// 唯一の実態
		static SoundManager instance;

		// 実態の参照を返す
		return instance;
	}

	/// <summary>
	/// 指定のサウンドを鳴らす
	/// </summary>
	/// <param name="name">サウンド名</param>
	void Play(const char* name);

	/// <summary>
	/// 全てのサウンドを停止する
	/// </summary>
	void StopSound();

	/// <summary>
	/// 全てのSEを停止する
	/// </summary>
	void StopSE();

	/// <summary>
	/// 指定のサウンドを停止する
	/// </summary>
	/// <param name="str"></param>
	void DesignationStopSound(std::string str);

	/// <summary>
	/// 指定のサウンドが流れているかどうか
	/// </summary>
	/// <param name="str"></param>
	/// <returns></returns>
	bool IsDesignationCheckPlaySound(std::string str);

	/// <summary>
	/// BGMの音量変更
	/// </summary>
	void ChangeBGMVolume();

	/// <summary>
	/// SEの音量変更
	/// </summary>
	void ChangeSEVolume();

private:	// 構造体
	/// <summary>
	/// 変更したサウンド情報をファイルに書き込む
	/// </summary>
	struct SoundConfigInfo
	{
		char signature[5];			// "SND_"
		float version;				// 1.0f
		unsigned short	volumeSE;	// 0～255
		unsigned short	volumeBGM;	// 0～255
	};

private:	// 関数
	/// <summary>
	/// コンストラクタ
	/// シングルトンパターンなので
	/// コンストラクタはprivateに置く
	/// </summary>
	SoundManager();

	// コピーを禁止する
	// コピーコンストラクタ
	SoundManager(const SoundManager&) = delete;
	// 代入を禁止する
	void operator=(const SoundManager&) = delete;

private:	// 変数
	std::unordered_map<std::string, LoadData::SoundData> m_data;	// サウンドデータ
};

