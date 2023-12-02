#include<DxLib.h>
#include<iostream>

#include "Sphere.h"

Capsule::Capsule(const VECTOR& startPos, const VECTOR& endPos, float rad)
    :localStart(startPos)
    , localEnd(endPos)
    , worldStart(startPos)
    , worldEnd(endPos)
    , worldCenter(VGet(0, 0, 0))
    , radius(rad)
{
    //処理なし
}

Capsule::~Capsule()
{
    //処理なし
}

void Capsule::Update(const VECTOR& pos)
{
    //座標移動
    worldStart = VAdd(localStart, pos);
    worldEnd = VAdd(localEnd, pos);
    worldCenter = VAdd(worldStart, VScale(VSub(worldEnd, worldStart), 0.5f));
}

bool Capsule::OnCollisionWithMesh(const int modelHandle, MV1_COLL_RESULT_POLY_DIM& colInfo)
{
    //当たり判定情報から判定結果を返す
    colInfo = MV1CollCheck_Capsule(modelHandle, -1, worldStart, worldEnd, radius);
    MV1SetupCollInfo(modelHandle);
    if (colInfo.HitNum == 0)
    {
        return false;
    }
    return true;
}

VECTOR Capsule::CalcPushBackFromMesh(const MV1_COLL_RESULT_POLY_DIM& colInfo, bool shouldVecY)
{

    VECTOR pushBack = VGet(0, 0, 0);
    VECTOR distance = VGet(0, 0, 0);

    // 衝突ポリゴン数分押し戻し量を計算する
    for (int i = 0; i < colInfo.HitNum; ++i)
    {
        if (HitCheck_Capsule_Triangle(worldStart, worldEnd,
            radius, colInfo.Dim[i].Position[0], colInfo.Dim[i].Position[1], colInfo.Dim[i].Position[2]))
        {
            VECTOR normalVec = VNorm(colInfo.Dim[i].Normal);
            distance = VSub(worldCenter, colInfo.Dim[i].Position[0]);

            float dot = VDot(normalVec, distance);
            float len = radius - VSize(VScale(normalVec, dot));

            pushBack = VAdd(pushBack, VScale(normalVec, len));
        }
    }


    return VScale(VSub(pushBack, worldCenter), 0.005f);                                      // 押し戻し量を返却
}

void Capsule::DrawDebug()
{
    DrawCapsule3D(worldStart, worldEnd, radius, 8, GetColor(255, 255, 255), GetColor(255, 50, 255), false);
}




