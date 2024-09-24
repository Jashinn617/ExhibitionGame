#pragma once
#include "Game.h"
#include <string>
#include <vector>

/// <summary>
/// ランキング情報を更新する
/// </summary>
class Ranking
{
public:
	Ranking();
	~Ranking();
	void CreateRanking(Game::Stage stageKind);			  // ランキング作成
	void UpdateRanking(Game::Stage stageKind, int clearTime); // ランキング更新
	std::vector<int> GetRanking(Game::Stage stageKind);						  // ランキング取得

private:
	std::string HttpGet(const char* domain, const char* url); // Http通信でGet命令を送る

private:	// 定数
	static const int kDataSize = 2560;	// データサイズ

private:	// 変数
	int m_netH;						// ネットワークハンドル
	int m_dataLength;						// 受信データ量保存用変数
	int m_lineCount;					// 表示する行数をカウントする
	char m_strBuf[kDataSize] = "";		// データバッファ

	std::vector<int> m_rankingList;		// クリアタイムを保存する
	std::string uri;
	std::string createRank;
	std::string getRank;

	IPDATA m_ip;							// 接続用ＩＰアドレスデータ
	size_t m_pos;						// 文字の長さを取得する


};

