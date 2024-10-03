#include "DxLib.h"
#include "SaveData.h"

#include <fstream>
#include <cassert>

namespace
{
	constexpr int kCurrentSaveVersion = 0;	// セーブデータのバージョン
	const char* const kSaveDataFileName = "SaveData.dat";	// セーブデータファイル名
}

SaveData::SaveData()
{
	/*処理無し*/
}

SaveData::~SaveData()
{
	/*処理無し*/
}

void SaveData::Load()
{
	// データのクリア
	ClearSaveData();

	std::ifstream ifs;
	ifs.open(kSaveDataFileName, std::ios_base::binary);
	if (ifs.fail())
	{
		//ファイル読み込み失敗 セーブデータを作る
		CreateNewSaveData();
		return;
	}
	else
	{
		Data data;
		ifs.read((char*)&data, sizeof(Data));
		ifs.close();

		if (data.version == kCurrentSaveVersion)
		{
			// 読み込んだデータを実際に使用するデータにコピー
			m_data = data;
		}
		else
		{
			// リリース後にセーブデータのバージョンが変わったら実装しよう
			assert(false);
		}
	}

}

void SaveData::Write()
{
	// データをファイルに書き込む
	std::ofstream ofs(kSaveDataFileName, std::ios_base::binary);
	ofs.write((char*)&m_data, sizeof(Data));
}

void SaveData::ChangeBGMVol(int BGMVol)
{
	m_data.volBGM = BGMVol;
}

void SaveData::ChangeSEVol(int SEVol)
{
	m_data.volSE = SEVol;
}

int SaveData::GetBGMVol() const
{
	return m_data.volBGM;
}

int SaveData::GetSEVol() const
{
	return m_data.volSE;
}

void SaveData::CreateNewSaveData()
{
	m_data.volBGM = 2;		// BGM音量	0~5
	m_data.volSE = 2;		// SE音量	0~5
}

void SaveData::ClearSaveData()
{
	std::ofstream ofs(kSaveDataFileName, std::ios_base::binary);
	ofs.write((char*)&m_data, sizeof(SaveData));
}
