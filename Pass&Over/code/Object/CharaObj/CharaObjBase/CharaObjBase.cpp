#include "CharaObjBase.h"

using namespace math;

CharaObjBase::CharaObjBase(std::string tag)
    :ObjBase(tag)
    , isMove(false)
    , moveSpeed(0.0f)
    , nowRotate(false)
    , rotRad(0.0f)
{


}

CharaObjBase::~CharaObjBase()
{
    //処理なし
}

void CharaObjBase::RotateYAxis(const VECTOR dir, float velocity)
{
    //現方向が目標方向でなければ回転中にする
    if (objDir != dir)
    {
        aimDir = dir;
        nowRotate = true;
        CalcRotDir(dir, velocity);
    }

    //目標方向まで回転処理
    if (VDot(objDir, aimDir) < 1.0f)
    {
        objDir = VTransform(objDir, rotYMat);
        rotRad += velocity;
    }
    else
    {
        objDir = aimDir;
        nowRotate = false;
    }
    rotateMat = MMult(MGetScale(objScale), rotYMat);
}

float CharaObjBase::CalcRotDir(const VECTOR dir, float velocity)
{
    //ラジアン角に変換
    rotRad = math::DegToRad(velocity);
    rotYMat = MGetRotY(rotRad);

    //回転方向を算出
    if (VCross(objDir, aimDir).y < 0)
    {
        rotYMat = MTranspose(rotYMat);
    }
    return velocity;
}

