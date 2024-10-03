#pragma once
#include "DxLib.h"

class MoveDirectionVec
{
public:
	MoveDirectionVec();
	~MoveDirectionVec();

	void Update();

	/// <summary>
	/// ˆÚ“®•ûŒü‚ğİ’è‚·‚é
	/// </summary>
	void SetDirectionVec();

	/// <summary>
	/// ˆÚ“®•ûŒü‚Ìæ“¾
	/// </summary>
	/// <returns>ˆÚ“®•ûŒü</returns>
	VECTOR GetDirectionVec()const { return m_directionVec; }

private:
	VECTOR m_directionVec;	// •ûŒüƒxƒNƒgƒ‹
};

