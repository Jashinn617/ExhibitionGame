#pragma once
class SaveData
{
public:
	~SaveData();

	static SaveData& GetInstance()
	{
		// 唯一の実態
		static SaveData instance;

		// 実態の参照を返す
		return instance;
	}

	/// <summary>
	/// データの読み込み
	/// </summary>
	void Load();

	/// <summary>
	/// データの書き込み
	/// </summary>
	void Write();

	/// <summary>
	/// BGMの音量変更
	/// </summary>
	/// <param name="BGMVol"></param>
	void ChangeBGMVol(int BGMVol);

	/// <summary>
	/// SEの音量変更
	/// </summary>
	/// <param name="SEVol"></param>
	void ChangeSEVol(int SEVol);

	/*コンフィグ*/

	int GetBGMVol() const;
	
	int GetSEVol() const;

	/// <summary>
	/// セーブデータの上書き(新規保存)
	/// </summary>
	void CreateNewSaveData();

private:
	/// <summary>
	/// コンストラクタ
	/// シングルトンパターンなので
	/// コンストラクタはprivateに置く
	/// </summary>
	SaveData();
	
	// コピーを禁止する
	SaveData(const SaveData&) = delete;	
	// 代入を禁止する
	void operator = (const SaveData&) = delete;

private:	// 関数
	/// <summary>
	///	データのクリア
	/// </summary>
	void ClearSaveData();

private:	// 構造体
	struct Data
	{
		int version = 0;		// データのバージョン
		unsigned int flag = 0;	// 各種フラグ

		// コンフィグ
		int volBGM = 2;		// BGM音量
		int volSE = 2;		// SE音量
	};

	Data m_data;	// セーブデータ
};

