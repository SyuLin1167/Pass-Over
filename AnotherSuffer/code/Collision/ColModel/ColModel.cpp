#include<DxLib.h>

#include "ColModel.h"

ColModel::ColModel(int& handle)
    :CollisionBase(ColTag.MODEL)
{
    colHandle = handle;
}

ColModel::~ColModel()
{
    //処理なし
}

bool ColModel::OnCollisionWithCapsule(const VECTOR startPos, const VECTOR endPos, const float radius)
{
    //当たり判定情報から判定結果を返す
    colInfoDim = MV1CollCheck_Capsule(colHandle, -1, startPos, endPos, radius);
    if (colInfoDim.HitNum > 0)
    {
        return true;
    }
    return false;
}

