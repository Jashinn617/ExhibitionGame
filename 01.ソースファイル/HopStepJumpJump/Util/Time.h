#pragma once
class Time
{
public:
	Time(int rimitTime);
	~Time();

	/// <summary>
	/// タイムリセット
	/// </summary>
	void Reset();

	/// <summary>
	/// タイム更新
	/// </summary>
	/// <returns>リミット時間を超えているかどうか</returns>
	bool Update();

private:	// 変数
	int m_time;			// 経った時間
	int m_rimitTime;	// リミット時間

};

