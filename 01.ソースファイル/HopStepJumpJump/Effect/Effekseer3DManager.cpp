#include "Effekseer3DManager.h"
#include "Effect.h"

#include "../Object/ObjectBase.h"

#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cassert>
#include <EffekseerForDXLib.h>


Effekseer3DManager::Effekseer3DManager()
{
	CsvLoad::GetInstance().EffectLoad(m_dataTable);
}

Effekseer3DManager::~Effekseer3DManager()
{
	/*処理無し*/
}

void Effekseer3DManager::Update()
{
	// Effekseerにより再生中のエフェクトを更新する
	UpdateEffekseer3D();
}

void Effekseer3DManager::Draw()
{
	Effekseer_Sync3DSetting();
	// Effekseerにより再生中のエフェクトを描画する。
	int result = 0;
	result = DrawEffekseer3D();
}

void Effekseer3DManager::Add(const char* fileName, PlayType type, ObjectBase* pObject, VECTOR pos, VECTOR rot)
{
	switch (type)
	{
	case Effekseer3DManager::PlayType::LoopFollow:
		m_pEffect.push_back(std::make_shared<EffectLoopFollow>(fileName, pObject));
		break;
	case Effekseer3DManager::PlayType::Loop:
		m_pEffect.push_back(std::make_shared<EffectLoop>(fileName, pos, rot));
		break;
	case Effekseer3DManager::PlayType::Follow:
		m_pEffect.push_back(std::make_shared<EffectFollow>(fileName, pObject));
		break;
	case Effekseer3DManager::PlayType::Normal:
		m_pEffect.push_back(std::make_shared<EffectNormal>(fileName, pos, rot));
		break;
	default:
		break;
	}
}

void Effekseer3DManager::Delete()
{
	// Effekseerにより再生中のエフェクトを全て削除する
	m_pEffect.clear();
}
