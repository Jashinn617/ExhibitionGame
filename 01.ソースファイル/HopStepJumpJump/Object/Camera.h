#pragma once
#include "DxLib.h"
#include "ObjectBase.h"

#include <memory>
#include <vector>

class Circle;
class Player;
class Time;

class Camera
{
public:
	Camera();
	~Camera();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="playerPos">プレイヤーの座標の取得</param>
	void Update(VECTOR playerPos);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// カメラの座標のリセット
	/// </summary>
	void ResetCamera();

	/// <summary>
	/// 当たり判定の更新
	/// </summary>
	/// <param name="Field"></param>
	void ColUpdate(ObjectBase* Field);

	/// <summary>
	/// ステージクリア時のカメラの更新
	/// </summary>
	void StageClearUpdate();

	/// <summary>
	/// ステージをクリア時
	/// </summary>
	/// <param name="angle"></param>
	/// <param name="targetPos"></param>
	void StageClear(float angle, VECTOR targetPos);

	/// <summary>
	/// カメラのX角度の取得
	/// </summary>
	/// <returns></returns>
	float GetCameraAngleX() { return m_angleH; }
	
	/// <summary>
	/// 当たり判定のサークル取得
	/// </summary>
	/// <returns>カメラの当たり判定のサークルポインタ</returns>
	const std::shared_ptr<Circle> GetCircle() const { return m_pCircle; }

	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>カメラ座標</returns>
	const VECTOR& GetPos() const { return m_nextPos; }

	/// <summary>
	/// カメラ座標の設定
	/// </summary>
	/// <param name="pos"></param>
	void SetPos(VECTOR pos) { m_nextPos = pos; }

	/// <summary>
	/// 前フレームのカメラの座標の取得
	/// </summary>
	/// <returns></returns>
	const VECTOR& GetPrevPos()const { return m_prevPos; }


private:	// 関数
	/// <summary>
	/// 角度の更新
	/// </summary>
	void UpdateAngle();

	/// <summary>
	/// 通常時の更新
	/// </summary>
	/// <param name="targetPos"></param>
	void NormalUpdate(VECTOR targetPos);

	/// <summary>
	/// 座標の更新
	/// </summary>
	void UpdatePos();

	/// <summary>
	/// 当たり判定を考慮した座標更新
	/// </summary>
	void FixPos();

	/// <summary>
	/// 押し出し処理
	/// </summary>
	void FixPosInternal();


private:	// 変数
	float m_angleH;	// 水平角度
	float m_angleV;	// 垂直角度
	float m_cameraToTargetLenght;	// カメラからターゲットまでの距離
	float m_clearCameraToTargetLength; // ステージクリア時のカメラからターゲットまでの距離
	float m_stageClearEasingTime;		// ゲームクリア時のイージングにかかる時間
	float m_stageClearTargetEasingTime;	// ゲームクリア時のターゲットの移動時のイージングにかかる時間
	bool m_isStageClear;	// ステージをクリアしたか

	VECTOR m_pos;		// 座標
	VECTOR m_nextPos;	// 次の座標
	VECTOR m_prevPos;	// 前の座標
	VECTOR m_targetPos;	// ターゲットの座標
	VECTOR m_leftVec;	// カメラからターゲットまでのベクトルから見て左方向のベクトル
	VECTOR m_stageClearTargetStartPos;	// ステージクリア時のターゲットの初期座標
	VECTOR m_stageClearTargetEndPos;	// ステージクリア時のターゲットの最終座標

	MATRIX m_rotX;	// 回転用行列X
	MATRIX m_rotY;	// 回転用行列Y

	std::shared_ptr<Circle> m_pCircle;	// 当たり判定のポインタ
	std::shared_ptr<Time> m_pClearTargetStartMoveTime;	// ステージクリア時のターゲットが移動するまでの移動時間


	/*当たり判定用の情報*/
	bool m_isMove;		// 移動したかどうか
	bool m_isHit;		// ポリゴンに当たったか
	
	MV1_COLL_RESULT_POLY_DIM m_hitDim{};								// 当たり判定の結果を入れる構造体
	MV1_COLL_RESULT_POLY* m_pPolyIndex[ColInfo::kMaxColHitPolyNum]{};	// 壁ポリゴンと判断されたポリゴンの構造体のアドレスを保存するためのポインタ配列
	MV1_COLL_RESULT_POLY* m_pPoly;									// ポリゴンの構造体にアクセス数るために使用するポインタ
	HITRESULT_LINE m_lineRes{};										// 線分とポリゴンとの当たり判定の結果を代入する構造体
};

