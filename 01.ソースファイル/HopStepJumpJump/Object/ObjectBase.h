#pragma once
#include "DxLib.h"
#include "../Util/CharacterData.h"

#include <array>
#include <memory>

class Model;
class ObjectManager;
class Time;
class Player;
class Circle;
class ToonShader;
class ShadowMapShader;

namespace ColInfo
{
	// 最大当たり判定ポリゴン数
	constexpr int kMaxColHitPolyNum = 2000;
	// 壁押し出し処理の最大試行回数
	constexpr int kMaxColHitTryNum = 16;
	// 壁押し出し時にスライドさせる距離
	constexpr float kColHitSlideLength = 1.0f;
}

class ObjectBase
{
public:	// 列挙体

	// 物体の分類
	enum class ColType
	{
		None,		// 無し
		Player,		// プレイヤー
		Enemy,		// エネミー
		Field,		// フィールド
		Item,		// アイテム
	};

public:	// 関数
	ObjectBase();

	virtual ~ObjectBase();

	virtual void Init();
	virtual void Update();

	virtual void Draw(std::shared_ptr<ToonShader> pToonShader);

	/// <summary>
	/// 影の描画
	/// </summary>
	/// <param name="pShadoeMapShader"></param>
	virtual void ShadowMapDraw(std::shared_ptr<ShadowMapShader> pShadowMapShader);

	/// <summary>
	/// 2Dの描画
	/// </summary>
	virtual void Draw2D();

	/// <summary>
	/// ステージクリア時
	/// </summary>
	virtual void StageClear();

	/// <summary>
	///	ゲーム終了時
	/// </summary>
	virtual void GameEnd();

	/// <summary>
	/// 攻撃が当たった時
	/// </summary>
	virtual void OnAttack();

	/// <summary>
	/// メインポインタの設定
	/// </summary>
	/// <param name="objectManager"></param>
	void SetMain(ObjectManager* objectManager) { m_pObjectManager = objectManager; }

	/// <summary>
	/// モデルハンドルの設定
	/// </summary>
	/// <param name="handle"></param>
	void SetModelHandle(int handle) { m_modelH = handle; }

	/// <summary>
	/// 存在が消えてからのカウント
	/// (存在が消えてからも倒れる演出で少しの間だけモデルを描画させるため)
	/// </summary>
	/// <returns></returns>
	bool IsExistCount();

	/// <summary>
	/// 終了処理
	/// </summary>
	void End();

	/// <summary>
	/// モデルクラスの取得
	/// </summary>
	/// <returns>モデルクラス</returns>
	const std::shared_ptr<Model> GetModel() const { return m_pModel;}

	/// <summary>
	/// キャラクター情報の取得
	/// </summary>
	/// <returns>キャラクター情報</returns>
	const CharacterData::CharacterInfo GetInfo() const { return m_info; }

	/// <summary>
	/// 自信の当たり判定の情報
	/// </summary>
	/// <returns>当たり判定情報</returns>
	virtual ColType GetColType() const { return ColType::None; }


	bool IsGameEnd()const { return m_isGameEnd; }

	bool IsDamage() const { return m_isDamage; }


	/// <summary>
	/// 移動アップデート
	/// </summary>
	/// <param name="pField"></param>
	void MoveCollFieldUpdate(ObjectBase* pField);

protected:	// 変数
	std::shared_ptr<Model> m_pModel;	// モデルクラス
	CharacterData::CharacterInfo m_info;	// キャラクター情報	
	float m_objSize;	// オブジェクトサイズ
	float m_angle;		// キャラクターの向き
	float m_moveSpeed;	// 移動スピード
	int m_modelH;		// モデルハンドル
	bool m_isDamage;	// 攻撃を受けたか
	bool m_isGameEnd;	// ゲーム終了したかどうか

	ObjectManager* m_pObjectManager = nullptr;

private:	// 関数
	// チェックしたポリゴンが壁ポリゴンか床ポリゴン化を判断し保存する
	void CheckWallAndFloor();
	// 壁ポリゴンとの当たり判定をチェックし、移動させる
	void FixPosWithWall();
	void FixPosWithWallInternal();
	// 床ポリゴンとの当たり判定をチェックし、移動させる
	void FixPosWithFloor();

private:	// 変数
	bool m_isMove;	// 移動したかどうか
	bool m_isHit;	// ポリゴンに当たったかどうか
	int m_wallNum;	// 壁と判断されたポリゴン数
	int m_floorNum;	// 床と判断されたポリゴン数

	MV1_COLL_RESULT_POLY_DIM m_hitDim;		// 当たり判定結果構造体
	MV1_COLL_RESULT_POLY* m_pWallPoly[ColInfo::kMaxColHitPolyNum];	// 壁ポリゴンと判断されたポリゴンのアドレスを保存しておくためのポインタ配列
	MV1_COLL_RESULT_POLY* m_pFloorPoly[ColInfo::kMaxColHitPolyNum];	// 壁ポリゴンと判断されたポリゴンのアドレスを保存しておくためのポインタ配列
	MV1_COLL_RESULT_POLY* m_pPoly;	// ポリゴンの構造体にアクセスするために使用するポインタ
	HITRESULT_LINE m_lineRes;					// 線分とポリゴンとの当たり判定の結果を代入する構造体

	VECTOR m_oldPos;		// 移動前の座標
	VECTOR m_nextPos;		// 移動後の座標

	// 存在が消えてからの時間
	std::shared_ptr<Time> m_pExistCountTime;
};

