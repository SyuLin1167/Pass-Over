#include<DxLib.h>

#include"../../Object/ObjBase/ObjBase.h"
#include"../Line/Line.h"
#include "Capsule.h"

Capsule::Capsule(const VECTOR& startPos, const VECTOR& endPos,const float& rad)
    :CollisionBase(ColTag.CAPSULE)
{
    localStart = startPos;
    localEnd = endPos;
    worldStart = startPos;
    worldEnd = endPos;
    radius = rad;
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

bool Capsule::OnCollisionWithMesh(const int& modelHandle)
{
    //当たり判定情報から判定結果を返す
    colInfoDim = MV1CollCheck_Capsule(modelHandle, -1, worldStart, worldEnd, radius);
    if (colInfoDim.HitNum == 0)
    {
        return false;
    }
    return true;
}

bool Capsule::OnCollisionWithCapsule(const VECTOR& pos, const VECTOR& pos2, const float& rad)
{
    return HitCheck_Capsule_Capsule(worldStart, worldEnd, radius, pos, pos2, rad);
}

VECTOR Capsule::CalcPushBackFromMesh()
{
    //押し戻し量初期化
    VECTOR pushBack = worldCenter;

    // 衝突ポリゴン数分押し戻し量を計算する
    for (int i = 0; i < colInfoDim.HitNum; ++i)
    {
        //押し戻し後の座標がまだめり込んでいたら押し戻し量計算
        if (HitCheck_Capsule_Triangle(
            VAdd(worldStart, VSub(pushBack, worldCenter)), VAdd(worldEnd, VSub(pushBack, worldCenter)),
            radius, colInfoDim.Dim[i].Position[0], colInfoDim.Dim[i].Position[1], colInfoDim.Dim[i].Position[2]))
        {
            //2辺から法線ベクトル算出
            VECTOR poligonVec1 = VSub(colInfoDim.Dim[i].Position[1], colInfoDim.Dim[i].Position[0]);
            VECTOR poligonVec2 = VSub(colInfoDim.Dim[i].Position[2], colInfoDim.Dim[i].Position[0]);
            VECTOR normalVec = VNorm(VCross(poligonVec1, poligonVec2));

            //めり込み量算出
            VECTOR distance = VSub(pushBack, colInfoDim.Dim[i].Position[0]);
            float dot = VDot(normalVec, distance);
            float cavedLen = radius - VSize(VScale(normalVec, dot));

            //めり込んだ分押し戻し量として加算
            pushBack = VAdd(pushBack, VScale(normalVec, cavedLen));
        }
    }

    // 押し戻し量を返却
    return VSub(pushBack, worldCenter);
}

void Capsule::DrawCapsule()
{
    DrawCapsule3D(worldStart, worldEnd, radius, 8, GetColor(255, 255, 255), GetColor(255, 50, 255), false);
}