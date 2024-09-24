#pragma once

#include <memory>
#include <array>

class CountGetItem
{
public:
	CountGetItem(int maxCount);
	virtual ~CountGetItem();

	void Draw();
	void Add();

	bool IsCountMax();

	int GetGetItemCount()const { return m_count; }
	
private:
	int m_count;						// カウント数
	int m_maxCount;						// カウントの最大値
	int m_H1;							// 画像ハンドル1
	int m_H2;							// 画像ハンドル2
	std::array<int, 10>m_numberH{};		// 数字の画像ハンドル
};

