#include "DxLib.h"
#include "Vec2.h"
#include "Ranking.h"

// 定数
namespace
{
	// ドメイン名
	const char* kDomainName = "jashinkunn.catfood.jp";
	// テスト用URI
	const char* kTestCreateUri = "/HopRank/NetTest/createRanking.php";				// 作成
	const char* kTestUpdateUri = "/HopRank/NetTest/updateRanking.php?clearTime=";  // 更新
	const char* kTestGetUri = "/HopRank/NetTest/getRanking.php";					// 取得
	// ステージ1URI
	const char* kStage1CreateUri = "/HopRank/Stage1/createRanking.php";				// 作成
	const char* kStage1UpdateUri = "/HopRank/Stage1/updateRanking.php?clearTime=";  // 更新
	const char* kStage1GetUri = "/HopRank/Stage1/getRanking.php";					// 取得
	// ステージ2URI
	const char* kStage2CreateUri = "/HopRank/Stage2/createRanking.php";					// 作成
	const char* kStage2UpdateUri = "/HopRank/Stage2/updateRanking.php?clearTime=";		// 更新
	const char* kStage2GetUri = "/HopRank/Stage2/getRanking.php";						// 取得
	constexpr int kPortNum = 80;		// ポート番号
	constexpr int kMaxRankNum = 10;		// 表示するランキング数

	// ランキング表示
	constexpr float kRankingIntervalAdj = 28.0f;		// 表示位置調整
	constexpr int kTextColor = 0xffffff;				// テキストの色
	constexpr float kRankingIntervalWidth = 400.0f;		// 横の表示間隔
	// ステージ選択時
	const Vec2 kStageSlectRankPos = { 900, 320 };		// 表示位置
	constexpr float kStageSlectRankInterval = 60.0f;	// 縦の表示間隔
	// クリア時
	const Vec2 kRankingPos = { 1000.0f, 550.0f };		// 表示位置
	constexpr float kClearRankInterval = 80.0f;			// 縦の表示間隔

}

Ranking::Ranking() :
	m_pos(0),
	m_lineCount(0)
{
	m_rankingList.resize(kMaxRankNum);
}


Ranking::~Ranking()
{
}

void Ranking::CreateRanking(Game::Stage stageKind)
{
	// ステージによってURIを変更する
	switch (stageKind)
	{
	case Game::Stage::Test:
		createRank = HttpGet(kDomainName, kTestCreateUri);
		break;
	case Game::Stage::Stage1:
		createRank = HttpGet(kDomainName, kStage1CreateUri);
		break;
	case Game::Stage::Stage2:
		createRank = HttpGet(kDomainName, kStage2CreateUri);
		break;
	}
}

void Ranking::UpdateRanking(Game::Stage stageKind, int clearTime)
{
	// ステージによってURIを変更する
	switch (stageKind)
	{
	case Game::Stage::Test:
		uri = kTestUpdateUri + std::to_string(clearTime);
		break;
	case Game::Stage::Stage1:
		uri = kStage1UpdateUri + std::to_string(clearTime);
		break;
	case Game::Stage::Stage2:
		uri = kStage2UpdateUri + std::to_string(clearTime);
		break;
	}

	std::string getRank = HttpGet(kDomainName, uri.c_str());
}

std::vector<int> Ranking::GetRanking(Game::Stage stageKind)
{
	// ステージによってURIを変更する
	switch (stageKind)
	{
	case Game::Stage::Test:
		getRank = HttpGet(kDomainName, kTestGetUri);
		break;
	case Game::Stage::Stage1:
		getRank = HttpGet(kDomainName, kStage1GetUri);
		break;
	case Game::Stage::Stage2:
		getRank = HttpGet(kDomainName, kStage2GetUri);
		break;
	}

	m_lineCount = 0;  // ランキングカウントをリセット
	size_t m_pos = 0;

	// 1位～10位まで取得する
	// MEMO:clearTimeだけを抽出する。clearTimeが見つからない場合はnposを返す
	while (m_lineCount < kMaxRankNum && (m_pos = getRank.find("\"clearTime\":")) != std::string::npos)
	{
		m_pos += strlen("\"clearTime\":");
		int clearTime = 0;
		int numChars = 0;

		if (sscanf_s(getRank.c_str() + m_pos, "%d%n", &clearTime, &numChars) == 1)
		{
			if (m_lineCount < kMaxRankNum)
			{
				m_rankingList[m_lineCount] = clearTime;  // ランキングリストに保存
				m_lineCount++;
			}

			// 次の要素に進む
			m_pos += numChars;				  // 読み取った文字数分だけ進める
			getRank = getRank.substr(m_pos);  // 残りの部分文字列を取得
		}
		else
		{
			break;  // sscanf_sが失敗した場合はループを終了
		}
	}

	return m_rankingList;
}

std::string Ranking::HttpGet(const char* domain, const char* uri)
{
	char HttpCmd[128] = ""; 	// Http通信を作成するための変数

	// DxLibの不要な機能をoffにする
	SetUseDXNetWorkProtocol(false);

	// DNSからドメイン名でIPアドレスを取得
	GetHostIPbyName(domain, &m_ip);

	// 通信を確立
	m_netH = ConnectNetWork(m_ip, kPortNum);

	// 確立が成功した場合のみ中の処理をする
	if (m_netH != -1)
	{
		//Http命令の作成
		sprintf_s(HttpCmd, "GET %s HTTP/1.1\nHost: %s\n\n", uri, domain);
#ifdef _DEBUG
		//DrawFormatString(0, 60, 0xffffff, "HttpCmd:\n%s", HttpCmd);
#endif // _DEBUG

		// データ送信(http命令を送る)
		NetWorkSend(m_netH, HttpCmd, static_cast<int>(strlen(HttpCmd)));

		// データがくるのを待つ
		while (!ProcessMessage())
		{
			// 取得していない受信データ量を得る
			m_dataLength = GetNetWorkDataLength(m_netH);

			// 取得してない受信データ量が-1じゃない場合はループを抜ける
			if (m_dataLength != -1)
			{
				break;
			}
		}

		// データ受信
		NetWorkRecv(m_netH, m_strBuf, kDataSize);    // データをバッファに取得

		// 接続を断つ
		CloseNetWork(m_netH);
	}

	return m_strBuf;
}