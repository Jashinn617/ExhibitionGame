#include "ObjectBase.h"

#include "CharacterBase.h"
#include "ObjectManager.h"
#include "Model.h"
#include "Circle.h"

#include "../Util/Time.h"

#include"../Shader/ToonShader.h"
#include"../Shader/ShadowMapShader.h"

namespace
{
	constexpr int kMaxColHitPolyNum = 2000;	// 最大当たり判定ポリゴン数
	constexpr int kMaxColHitTryNum = 16;	// 壁押し出し処理の最大試行回数
	constexpr float kColHitSlideLength = 1.0f;		// 壁押し出し時にスライドさせる距離
	constexpr float kMove = 0.01f;			// 移動したかを判断するための変数
	constexpr float kWallPolyBorder = 0.4f;	// 壁ポリゴンか床ポリゴン化を判断するための変数
	constexpr float kWallPolyHeight = 20.0f;	// 壁ポリゴンと判断するための高さ変数
	constexpr int kExistTimeCount = 10;		// 死んでからの時間
	constexpr float kHeadHeight = 40.0f;	// 頭の高さ
}


ObjectBase::ObjectBase():
	m_objSize(0.0f),
	m_angle(0.0f),
	m_moveSpeed(0.0f),
	m_modelH(-1),
	m_isDamage(false),
	m_isMove(false),
	m_isHit(false),
	m_wallNum(0),
	m_floorNum(0),
	m_hitDim{},
	m_pWallPoly{},
	m_pFloorPoly{},
	m_pPoly(nullptr),
	m_lineRes{},
	m_oldPos{0,0,0},
	m_nextPos{0,0,0}
{
	m_pExistCountTime = std::make_shared<Time>(kExistTimeCount);

	m_info.pos = VGet(0.0f, 0.0f, 0.0f);
	m_info.vec = VGet(0.0f, 0.0f, 0.0f);
	m_info.rot = VGet(0.0f, 0.0f, 0.0f);
	m_info.isExist = true;
}

ObjectBase::~ObjectBase()
{
	/*処理無し*/
}

void ObjectBase::Init()
{
	/*処理無し*/
}

void ObjectBase::Update()
{
	/*処理無し*/
}

void ObjectBase::Draw(std::shared_ptr<ToonShader> pToonShader)
{
	/*処理無し*/
}

void ObjectBase::ShadowMapDraw(std::shared_ptr<ShadowMapShader> pShadowMapShader)
{
	// モデルがない場合は描画をしない
	if (m_pModel == nullptr) return;

	for (int i = 0; i < MV1GetTriangleListNum(m_pModel->GetModelHandle()); i++)
	{
		int shaderType = MV1GetTriangleListVertexType(m_pModel->GetModelHandle(), i);
		pShadowMapShader->SetShader(shaderType);

		MV1DrawTriangleList(m_pModel->GetModelHandle(), i);
	}
}

void ObjectBase::Draw2D()
{
	/*処理無し*/
}

void ObjectBase::StageClear()
{
	/*処理無し*/
}

void ObjectBase::GameEnd()
{
	/*処理無し*/
}

void ObjectBase::OnAttack()
{
	/*処理無し*/
}

bool ObjectBase::IsExistCount()
{
	// 規定時間が過ぎたら存在を消す
	if (m_pExistCountTime->Update())
	{
		return true;
	}
	return false;
}

void ObjectBase::End()
{
	*m_pWallPoly = nullptr;
	*m_pFloorPoly = nullptr;
	m_pPoly = nullptr;
}

void ObjectBase::MoveCollFieldUpdate(ObjectBase* pField)
{
	// 移動前の座標を保存しておく
	m_oldPos = m_info.pos;
	// 移動後の座標を計算する
	m_nextPos = VAdd(m_oldPos, GetInfo().vec);

	// プレイヤーの周囲にあるステージポリゴンを取得する
	// 移動距離も考慮してから検出する
	m_hitDim = MV1CollCheck_Sphere(pField->GetModel()->GetModelHandle(), -1, GetInfo().pos,
		dynamic_cast<CharacterBase*>(this)->GetCircle()->GetRadius() + VSize(GetInfo().vec));

	// XかZにkMove(0.01f)以上移動した場合は移動した事にする
	if (fabs(dynamic_cast<CharacterBase*>(this)->GetInfo().vec.x) > kMove || fabs(GetInfo().vec.z) > kMove)
	{
		m_isMove = true;
	}
	else
	{
		m_isMove = false;
	}

	// 壁と床の当たり判定を行う
	CheckWallAndFloor();
	// 壁との当たり判定から位置修正処理をする
	FixPosWithWall();
	// 壁との当たり判定から位置修正処理をする
	FixPosWithFloor();

	// 新しい座標を保存する
	m_info.pos = m_nextPos;

	// 検出したプレイヤーの周囲のポリゴン情報を開放する(後始末)
	MV1CollResultPolyDimTerminate(m_hitDim);
}

void ObjectBase::CheckWallAndFloor()
{
	// 壁ポリゴンと床ポリゴンの数を初期化する
	m_wallNum = 0;
	m_floorNum = 0;

	// 検出されたポリゴンの数だけ繰り返す
	for (int i = 0; i < m_hitDim.HitNum; i++)
	{
		// ポリゴンの法線のY成分がkWallPolyBorderに達しているか
		// どうかで壁ポリゴンか床ポリゴンかを判断する
		if (m_hitDim.Dim[i].Normal.y < kWallPolyBorder && m_hitDim.Dim[i].Normal.y > -kWallPolyBorder)
		{
			// 壁ポリゴンと判断された場合でも、
			// プレイヤーのY座標よりも高いポリゴンのみと当たり判定を行う
			if (m_hitDim.Dim[i].Position[0].y > GetInfo().pos.y + kWallPolyHeight ||
				m_hitDim.Dim[i].Position[1].y > GetInfo().pos.y + kWallPolyHeight ||
				m_hitDim.Dim[i].Position[2].y > GetInfo().pos.y + kWallPolyHeight)
			{
				// ポリゴンの数が限界数に達していなかった場合はポリゴンを配列に追加する
				if (m_wallNum < ColInfo::kMaxColHitPolyNum)
				{
					// ポリゴンの構造体のアドレスを壁ポリゴンポインタ配列に保存する
					m_pWallPoly[m_wallNum] = &m_hitDim.Dim[i];

					// 壁ポリゴンの数を足す
					m_wallNum++;
				}
			}
		}
		else
		{
			// ポリゴンの数が限界数に達していなかった場合はポリゴンを配列に追加する
			if (m_floorNum < ColInfo::kMaxColHitPolyNum)
			{
				// ポリゴンの構造体のアドレスを壁ポリゴンポインタ配列に保存する
				m_pFloorPoly[m_floorNum] = &m_hitDim.Dim[i];

				// 床ポリゴンの数を足す
				m_floorNum++;
			}
		}
	}

}

void ObjectBase::FixPosWithWall()
{
	// 壁ポリゴンがなかった場合は何もしない
	if (m_wallNum == 0)return;

	/*壁ポリゴンとの当たり判定の処理*/
	// 壁に当たったかどうかのフラグをfalseにしておく
	m_isHit = false;
	
	/*移動したかどうかで処理を分岐する*/
	// 移動していた場合
	if (m_isMove)
	{
		// 壁ポリゴンの数だけ繰り返す
		for (int i = 0; i < m_wallNum; i++)
		{
			// i番目の壁のポリゴンのアドレスを壁ポリゴンポインタ配列から取得する
			m_pPoly = m_pWallPoly[i];

			// ポリゴンとプレイヤーが当たっていなかったら次のカウントに行く
			if (!HitCheck_Capsule_Triangle(m_nextPos,
				VAdd(m_nextPos, VGet(0.0f, dynamic_cast<CharacterBase*>(this)->GetCircle()->GetRadius(), 0.0f)),
				dynamic_cast<CharacterBase*>(this)->GetCircle()->GetRadius(),
				m_pPoly->Position[0], m_pPoly->Position[1], m_pPoly->Position[2])) continue;

			// 当たっていたら当たっているフラグを立てる
			m_isHit = true;

			/*壁を考慮した移動量を外積を使って出す*/
			
			// スライドさせる距離
			VECTOR SlideVec;

			// 進行方向ベクトルと壁ポリゴンの法線ベクトルに垂直なベクトルを出す
			SlideVec = VCross(dynamic_cast<CharacterBase*>(this)->GetInfo().vec, m_pPoly->Normal);

			// ↑で出したベクトルと壁ポリゴンの法線ベクトルに垂直なベクトルを出し、
			// 元の移動成分から壁方向の移動成分を抜いたベクトルを出す
			SlideVec = VCross(m_pPoly->Normal, SlideVec);

			// 移動前の座標にSlideVecを足したものを次の座標にする
			m_nextPos = VAdd(m_oldPos, SlideVec);

			/*新しい移動座標で壁ポリゴンと当たっていないかを判定する*/

			// 壁と当たっているかどうかのフラグを作っておく
			bool isHitWallPoly = false;
			for (int j = 0; j < m_wallNum; j++)
			{
				// j番目の壁のポリゴンのアドレスを壁ポリゴンポインタ配列から取得する
				m_pPoly = m_pWallPoly[j];

				// ポリゴンとプレイヤーが当たっていたらループから抜ける
				if (HitCheck_Capsule_Triangle(m_nextPos,
					VAdd(m_nextPos, VGet(0.0f, dynamic_cast<CharacterBase*>(this)->GetCircle()->GetRadius(), 0.0f)),
					dynamic_cast<CharacterBase*>(this)->GetCircle()->GetRadius(),
					m_pPoly->Position[0], m_pPoly->Position[1], m_pPoly->Position[2]))
				{
					// 当たっているフラグを立てる
					isHitWallPoly = true;
					break;
				}
			}

			// 全てのポリゴンと当たっていなかったらループを終わる
			if (!isHitWallPoly)
			{
				// 当たっているフラグをfalseにする
				m_isHit = false;
				break;
			}
		}
	}
	else
	{
		/*移動していない場合の処理*/

		// 壁ポリゴンの数だけ繰り返す
		for (int i = 0; i < m_wallNum; i++)
		{
			// i番目の壁のポリゴンのアドレスを壁ポリゴンポインタ配列から取得する
			m_pPoly = m_pWallPoly[i];

			// ポリゴンとプレイヤーが当たっていなかったら次のカウントに行く
			if (HitCheck_Capsule_Triangle(m_nextPos,
				VAdd(m_nextPos, VGet(0.0f, dynamic_cast<CharacterBase*>(this)->GetCircle()->GetRadius(), 0.0f)),
				dynamic_cast<CharacterBase*>(this)->GetCircle()->GetRadius(),
				m_pPoly->Position[0], m_pPoly->Position[1], m_pPoly->Position[2]))
			{
				// 当たっているフラグを立てる
				m_isHit = true;
				break;
			}
		}
	}

	// 壁に当たっていたら壁から押し出す処理をする
	if (m_isHit)
	{
		FixPosWithWallInternal();
	}
}

void ObjectBase::FixPosWithWallInternal()
{
	// 壁からの押し出し処理を限界まで繰り返す
	for (int i = 0; i < ColInfo::kMaxColHitTryNum; i++)
	{
		/*当たる可能性のある壁ポリゴンを全て見る*/

		// 壁と当たっているかどうか
		bool isHitWall = false;

		// 壁ポリゴンの数だけ繰り返す
		for (int j = 0; j < m_wallNum; j++)
		{
			// j番目の壁のポリゴンのアドレスを壁ポリゴンポインタ配列から取得する
			m_pPoly = m_pWallPoly[j];

			// キャラクターと当たっているかどうかを判定する
			// 当たっていなかったら次のカウントに行く
			if (!HitCheck_Capsule_Triangle(m_nextPos,
				VAdd(m_nextPos, VGet(0.0f, dynamic_cast<CharacterBase*>(this)->GetCircle()->GetRadius(), 0.0f)),
				dynamic_cast<CharacterBase*>(this)->GetCircle()->GetRadius(),
				m_pPoly->Position[0], m_pPoly->Position[1], m_pPoly->Position[2])) continue;

			// 当たっていた場合は規定距離文プレイヤーを壁の法線方向に移動させる
			m_nextPos = VAdd(m_nextPos, VScale(m_pPoly->Normal, kColHitSlideLength));

			// 移動したうえで壁ポリゴンと接触しているかどうかを判定する
			for (int k = 0; k < m_wallNum; k++)
			{
				// 当たっていたらループを抜ける
				m_pPoly = m_pWallPoly[k];
				if (HitCheck_Capsule_Triangle(m_nextPos,
					VAdd(m_nextPos, VGet(0.0f, dynamic_cast<CharacterBase*>(this)->GetCircle()->GetRadius(), 0.0f)),
					dynamic_cast<CharacterBase*>(this)->GetCircle()->GetRadius(),
					m_pPoly->Position[0], m_pPoly->Position[1], m_pPoly->Position[2]))
				{
					isHitWall = true;
					break;
				}
			}

			// 全てのポリゴンと当たっていなかったらループを終わる
			if (!isHitWall) break;

		}
		// ループ終了
		if (!isHitWall) break;
	}
}

void ObjectBase::FixPosWithFloor()
{
	// 床ポリゴンが無い場合は何もしない
	if (m_floorNum == 0) return;

	/*床ポリゴンとの当たり判定*/
	// 当たったかどうかのフラグの初期化
	bool isHitFlag = false;

	// ジャンプ中かつ上昇中の場合
	if (dynamic_cast<CharacterBase*>(this)->IsJump()
		&& dynamic_cast<CharacterBase*>(this)->GetJumpPower() >= 0.0f)
	{
		// 天井に頭をぶつける処理を行う
		// 一番低い天井にぶつけるための判定用変数を初期化する
		float polyMinPosY = 0.0f;

		// 床ポリゴンの数だけ繰り返す
		for (int i = 0; i < m_floorNum; i++)
		{
			// i番目の床ポリゴンのアドレスを床ポリゴンポインタ配列から取得する
			m_pPoly = m_pFloorPoly[i];

			// 足先から頭の高さまでの間でポリゴンと接触しているかどうかを判定する
			m_lineRes = HitCheck_Line_Triangle(m_nextPos, VAdd(m_nextPos, VGet(0.0f, kHeadHeight, 0.0f)),
				m_pPoly->Position[0], m_pPoly->Position[1], m_pPoly->Position[2]);

			// 接触していなかった場合は何もせずに次のカウントに行く
			if (!m_lineRes.HitFlag) continue;

			// 天井ポリゴンが今まで検出されたポリゴンより低い場合は処理を通す
			if (polyMinPosY < m_lineRes.Position.y)
			{
				// 当たったフラグを立てる
				isHitFlag = true;

				// 接触したY座標を保存する
				polyMinPosY = m_lineRes.Position.y;
			}
		}

		// 接触しているポリゴンがあった場合
		if (isHitFlag)
		{
			// 接触した場合はプレイヤーのY座標を接触座標をもとに更新する
			m_nextPos.y = polyMinPosY - kHeadHeight;

			// 地面に当たった時の処理を行う
			dynamic_cast<CharacterBase*>(this)->HitGroundUpdate();
		}
	}
	else
	{
		/*床ポリゴンとの当たり判定*/

		// 一番高い床ポリゴンにぶつけるための判定用変数を初期化する
		float polyMaxPosY = 0.0f;

		// 床ポリゴンに当たったかどうかのフラグを倒しておく
		m_isHit = false;

		// 床ポリゴンの数だけ繰り返す
		for (int i = 0; i < m_floorNum; i++)
		{
			// i番目の床ポリゴンのアドレスを床ポリゴンポインタ配列から取得する
			m_pPoly = m_pFloorPoly[i];

			// 足先から頭の高さまでの間でポリゴンと接触しているかどうかを判定する
			m_lineRes = HitCheck_Line_Triangle(VAdd(m_nextPos, VGet(0.0f, kHeadHeight, 0.0f)),
				m_nextPos,m_pPoly->Position[0], m_pPoly->Position[1], m_pPoly->Position[2]);

			// 当たっていなかったら何もせずに次のカウントに行く
			if (!m_lineRes.HitFlag) continue;

			// 既に当たったポリゴンがあり、かつ今まで検出した
			// 床ポリゴンより低い場合は何もせずに次のカウントに行く
			if (m_isHit && polyMaxPosY > m_lineRes.Position.y) continue;

			// ポリゴンに当たったフラグを立てる
			m_isHit = true;

			// 接触したY座標を保存する
			polyMaxPosY = m_lineRes.Position.y;
		}

		if (m_isHit)
		{
			// 接触したポリゴン出一番高いY座標をプレイヤーのY座標にする
			m_nextPos.y = polyMaxPosY;
			dynamic_cast<CharacterBase*>(this)->HitGroundUpdate();

			// ジャンプ中かつ、ジャンプ力が0よりも小さい(降下中)の場合はジャンプ処理を終了する
			if (dynamic_cast<CharacterBase*>(this)->GetJumpPower() <= 0.0f &&
				dynamic_cast<CharacterBase*>(this)->IsJump())
			{
				dynamic_cast<CharacterBase*>(this)->EndJump();
			}
		}
	}
}
