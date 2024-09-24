#pragma once
class ActionTime
{
public:
	ActionTime(int maxTime);
	~ActionTime();

	/// <summary>
	/// 更新
	/// </summary>
	bool Update();

	/// <summary>
	/// タイムリセット
	/// </summary>
	void Reset();

private:
	int m_time;			// 経過時間
	int m_maxTime;		// 最大時間
	bool m_isAction;	// アクションを実行したかどうか
};

