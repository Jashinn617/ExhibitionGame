#pragma once
#include "DxLib.h"

#include <vector>
#include <memory>
#include <functional>

class Stamina;
class Time;
class Input;

class PlayerState
{
public:
	// 状態
	enum class StateKind
	{
		Idle,		// 待機
		Walk,		// 歩き
		Dash,		// 走り
		Jump,		// ジャンプ
		KnockBack,	// ノックバック
		StateClear,	// ステージクリア
	};

public:
	PlayerState();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~PlayerState() {};

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// ダメージを受けた
	/// </summary>
	void OnDamage();

	/// <summary>
	/// ステージクリア
	/// </summary>
	void StageClear();

	/// <summary>
	/// 状態情報の追加
	/// </summary>
	/// <param name="stateInit">初期化</param>
	/// <param name="stateUpdate">更新</param>
	/// <param name="stateKind">状態の種類</param>
	void AddState(std::function<void(void)> stateInit, 
		std::function<void(void)> stateUpdate, 
		StateKind stateKind);

	/// <summary>
	/// 状態の設定
	/// </summary>
	/// <param name="state">状態</param>
	void SetState(StateKind stateKind);

	/// <summary>
	/// 状態の終了処理
	/// </summary>
	void EndState();

	/// <summary>
	/// 現在の状態の取得
	/// </summary>
	/// <returns></returns>
	StateKind GetState() const { return m_pNowState.stateKind; }

private:	// 構造体

	// 状態情報
	struct StateData
	{
		std::function<void(void)> stateInit{};		// 状態初期化処理
		std::function<void(void)> stateUpdate{};	// 状態更新処理
		StateKind stateKind{};						// 状態の種類
	};

private:	// 関数
	/// <summary>
	/// 待機状態への変更
	/// </summary>
	void StateTransitionIdle();
	/// <summary>
	/// 歩き状態への変更
	/// </summary>
	void StateTransitionWalk();
	/// <summary>
	/// ダッシュ状態への変更
	/// </summary>
	void StateTransitionDash();
	/// <summary>
	/// ジャンプ状態への変更
	/// </summary>
	void StateTransitionJump();

	/// <summary>
	/// 各状態遷移
	/// </summary>
	void StateTransition();

	/// <summary>
	/// 状態変更
	/// </summary>
	/// <param name="state">状態</param>
	void StateChange(StateKind stateKind);

private:	// 変数
	bool m_isAction;						// アクション行動中か
	std::vector<StateData> m_pState;		// 状態の格納
	StateData m_pNowState;					// 現在の状態
};

