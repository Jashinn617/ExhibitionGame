#pragma once
#include "Effekseer3DManager.h"

#include <memory>

// 追従とループをするエフェクトのクラス
class EffectLoopFollow : public Effekseer3DManager::EffectBehavior
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="FileName">ファイルパス</param>
	/// <param name="pObject">オブジェクト</param>
	EffectLoopFollow(const char* FileName, ObjectBase* pObject);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~EffectLoopFollow();

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override final;

	/// <summary>
	/// エフェクトの再生
	/// </summary>
	void Play() override final;

private:
	ObjectBase* m_pObject;	// オブジェクト
};

// 追従するエフェクトのクラス
class EffectFollow : public Effekseer3DManager::EffectBehavior
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="FileName">ファイルパス</param>
	/// <param name="pObject">オブジェクト</param>
	EffectFollow(const char* FileName, ObjectBase* pObject);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~EffectFollow();

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override final;

	/// <summary>
	/// エフェクトの再生
	/// </summary>
	void Play() override final;

private:
	ObjectBase* m_pObject;	// オブジェクト
};

// ループするエフェクトのクラス
class EffectLoop : public Effekseer3DManager::EffectBehavior
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="FineName">ファイルパス</param>
	/// <param name="pos">座標</param>
	/// <param name="rot">角度</param>
	EffectLoop(const char* FileName,
		VECTOR pos,
		VECTOR rot = VGet(0.0f, 0.0f, 0.0f));

	virtual ~EffectLoop();

	void Update() override final;
	void Play() override final;

private:
	VECTOR m_pos;
	VECTOR m_rot;
};

// その他のエフェクトのクラス
class EffectNormal : public Effekseer3DManager::EffectBehavior
{
public:
	EffectNormal(const char* FileName,
		VECTOR pos,
		VECTOR rot = VGet(0.0f, 0.0f, 0.0f));

	virtual ~EffectNormal();

	void Update() override final;
	void Play() override final;

private:
	VECTOR m_pos;
	VECTOR m_rot;
};

