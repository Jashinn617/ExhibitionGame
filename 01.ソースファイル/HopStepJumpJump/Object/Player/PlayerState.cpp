#include "PlayerState.h"

#include "../../Util/Pad.h"
#include "../../Util/SoundManager.h"

PlayerState::PlayerState():
	m_isAction(false)
{
}

void PlayerState::Update()
{
	// 状態遷移
	StateTransition();

	m_pNowState.stateUpdate();

	// 歩き、走り状態ではないときはサウンドを止める
	if (m_pNowState.stateKind != StateKind::Walk)
	{
		SoundManager::GetInstance().DesignationStopSound("Walk");
	}
	if (m_pNowState.stateKind != StateKind::Dash)
	{
		SoundManager::GetInstance().DesignationStopSound("Dash");
	}
}

void PlayerState::OnDamage()
{
	// 現在の状態がノックバックの時は何もせずに終了する
	if (m_pNowState.stateKind == StateKind::KnockBack) return;

	// ノックバック状態に遷移する
	for (auto& changeState : m_pState)
	{
		// 状態がノックバックの時
		if (changeState.stateKind == StateKind::KnockBack)
		{
			m_isAction = true;
			// 状態変更
			m_pNowState = changeState;
			// 状態の初期化
			m_pNowState.stateInit();
			return;
		}
	}
}

void PlayerState::StageClear()
{
	// 現在の状態がステージクリアの時は何もせずに終了する
	if (m_pNowState.stateKind == StateKind::StateClear) return;

	// ステージクリア状態に遷移する
	for (auto& changeState : m_pState)
	{
		// 状態がステージクリアの時
		if (changeState.stateKind == StateKind::StateClear)
		{
			m_isAction = true;
			// 状態変更
			m_pNowState = changeState;
			// 状態の初期化
			m_pNowState.stateInit();
			return;
		}
	}
}

void PlayerState::AddState(std::function<void(void)> stateInit, std::function<void(void)> stateUpdate, StateKind stateKind)
{
	// 状態追加用の変数の設定
	StateData state;
	state.stateInit = stateInit;
	state.stateUpdate = stateUpdate;
	state.stateKind = stateKind;

	// 状態追加
	m_pState.push_back(state);
}

void PlayerState::SetState(StateKind stateKind)
{
	for (auto& changeState : m_pState)
	{
		// 状態の種類が引数の状態の時
		if (changeState.stateKind == stateKind)
		{
			// 状態の変更
			m_pNowState = changeState;
			return;
		}
	}
}

void PlayerState::EndState()
{
	// アクションの終了
	m_isAction = false;
}

void PlayerState::StateTransitionIdle()
{
	// アクション中の場合は何もせずに終了する
	if (m_isAction) return;

	// 移動ボタンが何も押されていなかった場合
	if (!Pad::isPress(PAD_INPUT_LEFT) && !Pad::isPress(PAD_INPUT_RIGHT) &&
		!Pad::isPress(PAD_INPUT_UP) && !Pad::isPress(PAD_INPUT_DOWN))
	{
		StateChange(StateKind::Idle);
	}
}

void PlayerState::StateTransitionWalk()
{
	// アクション中の場合は何もせずに終了する
	if (m_isAction) return;

	// 移動ボタンが何かしら押されていた場合
	if (Pad::isPress(PAD_INPUT_LEFT) || Pad::isPress(PAD_INPUT_RIGHT) ||
		Pad::isPress(PAD_INPUT_UP) || Pad::isPress(PAD_INPUT_DOWN))
	{
		StateChange(StateKind::Walk);
	}
}

void PlayerState::StateTransitionDash()
{
	// アクション中の場合は何もせずに終了する
	if (m_isAction) return;

	//移動ボタンが押されていなかったら処理を返す
	if (!Pad::isPress(PAD_INPUT_LEFT) && !Pad::isPress(PAD_INPUT_RIGHT) &&
		!Pad::isPress(PAD_INPUT_UP) && !Pad::isPress(PAD_INPUT_DOWN)) return;

	// RBが押されていたらダッシュする
	if (Pad::isPress(PAD_INPUT_6))
	{
		StateChange(StateKind::Dash);
	}
}

void PlayerState::StateTransitionJump()
{
	// アクション中の場合は何もせずに終了する
	if (m_isAction) return;

	// Aボタンが押されたらジャンプ
	if (Pad::isTrigger(PAD_INPUT_1))
	{
		m_isAction = true;
		StateChange(StateKind::Jump);
	}
}

void PlayerState::StateTransition()
{
	// 状態遷移
	StateTransitionIdle();
	StateTransitionWalk();
	StateTransitionDash();
	StateTransitionJump();
}

void PlayerState::StateChange(StateKind stateKind)
{
	for (auto& changeState : m_pState)
	{
		// 状態の種類が引数の状態の時
		if (changeState.stateKind == stateKind)
		{
			// 状態変更
			m_pNowState = changeState;
			// 状態の初期化処理
			m_pNowState.stateInit();
			return;
		}
	}
}
