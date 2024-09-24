#pragma once

#include <memory>
#include <array>

class CountTime
{
public:
	CountTime();
	virtual ~CountTime();

	void Update(int time);
	void Draw();

private:
	int m_time;	// 現在の秒数
	int m_H;	// 画像ハンドル
	std::array<int, 10> m_numberH{};	// 数字の画像ハンドル
};