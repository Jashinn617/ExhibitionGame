#pragma once
#include "DxLib.h"
#include "../../Util/CharacterData.h"
#include "../CharacterBase.h"

class Player;
class ObjectBase;
class Model;
class Circle;
class Time;

class ItemBase : public CharacterBase
{
public:
	ItemBase();
	ItemBase(VECTOR pos);
	virtual ~ItemBase();

	virtual void Init();
	virtual void Update() override;

	virtual void Draw(std::shared_ptr<ToonShader> pToonShader);

	// 存在するかどうか
	bool GetExist()const { return m_info.isExist; }

	// ゲットされたとき
	void OnGet();

	// 自身の当たり判定の情報
	virtual ColType GetColType()const { return ColType::Item; }

	/// <summary>
	/// ゲーム終了時処理
	/// </summary>
	void GameEnd() override final;

protected:
	// 移動速度の初期化
	void InitMoveSpeed(float moveSpeed);

	// 移動方向の更新
	virtual void MoveDirectionUpdate();
	// 移動方向と角度を考慮した移動の更新
	virtual VECTOR MoveUpdate();



protected:	// 変数
	float m_colHeight;					// 当たり判定の高さをモデルの中心にする
	VECTOR m_moveDirectionVec;			// 移動方向ベクトル
	int m_createTime;					// 生成されてから立った時間
	std::shared_ptr<Time> m_deadTime;	// 死んでからの時間
};