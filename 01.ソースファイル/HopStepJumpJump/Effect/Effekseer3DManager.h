#pragma once
#include "EffekseerForDXLib.h"
#include "../Util/CsvLoad.h"

#include <unordered_map>
#include <string>
#include <vector>
#include <list>

class ObjectBase;

class Effekseer3DManager
{
public:	// enumクラス
	enum class PlayType
	{
		LoopFollow,	// ループ、追従
		Loop,		// ループのみ
		Follow,		// 追従のみ
		Normal,		// その他
	};
public:		// クラス
	class EffectBehavior
	{
	public:
		virtual ~EffectBehavior() {};
		virtual void Update() {};
		virtual bool IsEnd() { return m_isEnd; }

		/// <summary>
		/// エフェクトデータの取得
		/// </summary>
		/// <returns></returns>
		LoadData::EffectData GetData() const { return m_data; }

		void SetData(const char* FileName)
		{
			m_data = GetInstance().m_dataTable[FileName];
		}

	protected:	// 関数
		// 再生
		virtual void Play() {};

	protected:	// 変数
		bool m_isEnd;	// エフェクトの再生終了
		LoadData::EffectData m_data;	// エフェクトデータ
	};

public:
	~Effekseer3DManager();

	static Effekseer3DManager& GetInstance()
	{
		// 唯一の実態
		static Effekseer3DManager instance;

		// 実態の参照を返す
		return instance;
	}

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// エフェクトの追加
	/// </summary>
	/// <param name="fileName">ファイルパス</param>
	/// <param name="type">エフェクトのタイプ</param>
	/// <param name="pObject">オブジェクトポインタ</param>
	/// <param name="pos">座標</param>
	/// <param name="rot">角度</param>
	void Add(const char* fileName, PlayType type, 
		ObjectBase* pObject = nullptr,
		VECTOR pos = VGet(0.0f, 0.0f, 0.0f), 
		VECTOR rot = VGet(0.0f, 0.0f, 0.0f));

	/// <summary>
	/// エフェクトの消去
	/// </summary>
	void Delete();

private:	// 関数
	/// <summary>
	/// コンストラクタ
	/// シングルトンパターンなので
	/// コンストラクタはprivateに置く
	/// </summary>
	Effekseer3DManager();

	// コピーを禁止する
	// コピーコンストラクタ
	Effekseer3DManager(const Effekseer3DManager&) = delete;
	// 代入を禁止する
	void operator=(const Effekseer3DManager&) = delete;

private:	// 変数
	// エフェクトのデータテーブル
	std::unordered_map<std::string, LoadData::EffectData> m_dataTable;

	// エフェクトの管理リスト
	std::list<std::shared_ptr<EffectBehavior>> m_pEffect;
};

