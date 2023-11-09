#include "CharaObjBase.h"

CharaObjBase::CharaObjBase(std::string tag)
    :ObjBase(tag)
    , moveSpeed(0.0f)
{


}

CharaObjBase::~CharaObjBase()
{
    //処理なし
}

void CharaObjBase::RotatePitch(const VECTOR& aimDir, float velocity)
{
    //角速度をラジアン角にして、回転方向を算出
    rotRad = math::DegToRad(velocity);
    if (VCross(objDir, aimDir).y < 0)
    {
        rotRad *= -1;
    }

    //回転中にする
    nowRotate = true;
    
    //回転処理
    if (nowRotate)
    {
        // モデルに向いてほしい方向に回転.
        MATRIX rotYMat = MGetRotY(rotRad);
        MV1SetRotationMatrix(objHandle, rotYMat);
    }
}

float CharaObjBase::CaldRotDir()
{
    return 0.0f;
}

