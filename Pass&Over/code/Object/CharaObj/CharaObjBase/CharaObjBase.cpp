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
    if (objDir != aimDir)
    {
        //角速度をラジアン角にして、回転方向を算出
        rotRad = DX_PI_F * velocity / PI_RAD;
        if (VCross(objDir, aimDir).y < 0)
        {
            rotRad *= -1;
        }

        //回転中にする
        nowRotate = true;
    }

    //回転処理
    if (nowRotate)
    {
        MATRIX objMat = MV1GetMatrix(objHandle);
        MATRIX rotMat = MGetRotY(rotRad);
        objMat = MMult(objMat, rotMat);
        MV1SetRotationMatrix(objHandle, objMat);
    }
}

float CharaObjBase::CaldRotDir()
{
    return 0.0f;
}

