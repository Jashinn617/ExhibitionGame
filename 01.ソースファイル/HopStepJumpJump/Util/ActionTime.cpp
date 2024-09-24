#include "ActionTime.h"

ActionTime::ActionTime(int maxTime):
	m_time(0),
	m_maxTime(0),
	m_isAction(false)
{
}

ActionTime::~ActionTime()
{
	/*処理無し*/
}

bool ActionTime::Update()
{
	// アクションが実行されていたらfalse
	if (m_isAction) return false;

	// リミット時間を超えたらフラグをtrueにする
	if (m_time >= m_maxTime)
	{
		m_isAction = true;
		return true;
	}
	m_time++;

	return false;
}

void ActionTime::Reset()
{
	// フラグのリセット
	m_isAction = false;
	// タイムのリセット
	m_time = 0;
}
