#include<DxLib.h>
#include "Sphere.h"

Sphere::Sphere(const VECTOR& centerPos, float rad)
    :localPos(centerPos)
    , worldPos(centerPos)
    , radius(rad)
{
    //処理なし
}

Sphere::~Sphere()
{
    //処理なし
}

void Sphere::Update(const VECTOR& pos)
{
    //座標移動
    worldPos = VAdd(localPos, pos);
}

bool Sphere::OnCollisionWithMesh(const int modelHandle, MV1_COLL_RESULT_POLY_DIM& colInfo)
{
    //当たり判定情報から判定結果を返す
    colInfo = MV1CollCheck_Sphere(modelHandle, -1, worldPos, radius);
    if (colInfo.HitNum == 0)
    {
        return false;
    }
    return true;
}

VECTOR Sphere::CalcPushBackFromMesh(const MV1_COLL_RESULT_POLY_DIM& colInfo)
{
    VECTOR SphereCenter = worldPos;

    for (int i = 0; i < colInfo.HitNum; i++)
    {
        if (HitCheck_Sphere_Triangle(worldStart, worldEnd,
            radius, colInfo.Dim[i].Position[0],colInfo.Dim[i].Position[1],colInfo.Dim[i].Position[2]))
        {
            VECTOR nomalVec = VNorm(colInfo.Dim[i].Normal);
            VECTOR distanceVec = VSub(SphereCenter, colInfo.Dim[i].HitPosition);
            float dot = VDot(nomalVec, distanceVec);
            float len = VSize(VScale(nomalVec, dot));

            len = radius - len;
            SphereCenter = VAdd(SphereCenter, VScale(nomalVec, len));
        }
    }
    return SphereCenter;
}

void Sphere::DrawDebug()
{
    DrawSphere3D(worldStart, worldEnd, radius, 8, GetColor(255, 255, 255), GetColor(255, 50, 255), false);
}




