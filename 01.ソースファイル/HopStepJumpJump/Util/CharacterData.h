#pragma once
#include "DxLib.h"

// キャラクターの持っている情報
namespace CharacterData
{
	// キャラクター情報
	struct CharacterInfo
	{
		VECTOR pos;		// 座標
		VECTOR vec;		// 移動ベクトル
		VECTOR rot;		// 回転	
		int modelH;		// モデルハンドル
		int isExist;	// 存在しているかどうか
	};
}
