#include "DxLib.h"
#include "CountTime.h"

#include "Game.h"

namespace
{
	const char* const kFileName = "Data/Img/Time/Time.png";
	const char* const kMyTimeNumFileName[10] =
	{
		 "Data/Img/Time/TimeNum0.png",
		 "Data/Img/Time/TimeNum1.png",
		 "Data/Img/Time/TimeNum2.png",
		 "Data/Img/Time/TimeNum3.png",
		 "Data/Img/Time/TimeNum4.png",
		 "Data/Img/Time/TimeNum5.png",
		 "Data/Img/Time/TimeNum6.png",
		 "Data/Img/Time/TimeNum7.png",
		 "Data/Img/Time/TimeNum8.png",
		 "Data/Img/Time/TimeNum9.png",
	};

	/*画像の座標1*/
	constexpr int kImagePosX1 = static_cast<int>(Game::kScreenWidth * 0.5f);	// X
	constexpr int kImagePosY1 = 230;	// Y
	/*分数の座標*/
	/*数字の座標(一の位)*/
	constexpr int kMinutesFirstPosX = static_cast<int>(Game::kScreenWidth * 0.45f);	// X
	constexpr int kMinutesFirstPosY = 230;	// Y
	/*数字の座標(十の位)*/
	constexpr int kMinutesSecondPosX = static_cast<int>(Game::kScreenWidth * 0.39f);	// X
	constexpr int kMinutesSecondPosY = 230;	// Y
	/*秒数の座標*/
	/*数字の座標(一の位)*/
	constexpr int kSecondFirstPosX = static_cast<int>(Game::kScreenWidth * 0.61f);	// X
	constexpr int kSecondFirstPosY = 230;	// Y
	/*数字の座標(十の位)*/
	constexpr int kSecondSecondPosX = static_cast<int>(Game::kScreenWidth * 0.55f);	// X
	constexpr int kSecondSecondPosY = 230;	// Y

	constexpr float kFontSize = 0.5f;	// 文字サイズ
}

CountTime::CountTime():
	m_time(0),
	m_H(-1)
{
	m_H = LoadGraph(kFileName);

	// 数字画像ハンドルロード
	for (int i = 0; i < m_numberH.size(); i++)
	{
		m_numberH[i] = LoadGraph(kMyTimeNumFileName[i]);
	}
}

CountTime::~CountTime()
{
	DeleteGraph(m_H);
	for (int i = 0; i < m_numberH.size(); i++)
	{
		DeleteGraph(m_numberH[i]);
	}
}

void CountTime::Update(int time)
{
	m_time = time;
}

void CountTime::Draw()
{
	// 画像の描画
	DrawRotaGraph(kImagePosX1, kImagePosY1,
		kFontSize, 0.0f,
		m_H, true);

	// 現在の秒数計算
	int nowTime = m_time / 60;

	// 分数の計算と描画
	int minutesTime = nowTime / 60;
	int minutesTimeFirst = minutesTime % 10;	// 一の位
	int minutesTimeSecond = minutesTime / 10;	// 十の位

	DrawRotaGraph(kMinutesFirstPosX, kMinutesFirstPosY,
		kFontSize, 0.0f,
		m_numberH[minutesTimeFirst], true);
	DrawRotaGraph(kMinutesSecondPosX, kMinutesSecondPosY,
		kFontSize, 0.0f,
		m_numberH[minutesTimeSecond], true);

	// 秒数の計算と描画
	int secondTime = nowTime % 60;
	int secondTimeFirst = secondTime % 10;	// 一の位
	int secondTimeSecond = secondTime / 10;	// 十の位

	DrawRotaGraph(kSecondFirstPosX, kSecondFirstPosY,
		kFontSize, 0.0f,
		m_numberH[secondTimeFirst], true);
	DrawRotaGraph(kSecondSecondPosX, kSecondSecondPosY,
		kFontSize, 0.0f,
		m_numberH[secondTimeSecond], true);
}
