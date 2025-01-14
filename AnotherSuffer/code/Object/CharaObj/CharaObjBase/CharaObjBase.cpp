#include<DxLib.h>

#include"../../ObjBase/ObjBase.h"
#include"../../../Asset/AssetManager/AssetManager.h"
#include"../../Math/Math.h"
#include "CharaObjBase.h"

static constexpr float SIMILAR_ANGLE = 0.999f;     //相似角
const float CharaObjBase::ROTATE_SPEED = 3.0f;     //回転速度

using namespace math;

CharaObjBase::CharaObjBase(std::string tag)
    :ObjBase(tag)
    , isMove(false)
    , moveSpeed()
    , nowRotate(false)
    , modelData(GetAssetPathData(AssetManager::ModelInstance()))
    , soundData(GetAssetPathData(AssetManager::SoundInstance()))
    , motionData(GetAssetPathData(AssetManager::MotionInstance()))
{
    XAxisData.reset(new AxisData(xAxis, this));
    YAxisData.reset(new AxisData(yAxis, this, DX_PI_F / 2.0f));
    ZAxisData.reset(new AxisData(zAxis, this));
}

CharaObjBase::~CharaObjBase()
{
    //処理なし
}

rapidjson::Value& CharaObjBase::GetAssetPathData(AssetBase* asset)
{
    //任意のアセットデータにオブジェクトがいたらパスデータ初期化
    auto& jsonData = asset->GetJsonData();
    if (jsonData.HasMember(objTag.c_str()))
    {
        return jsonData[objTag.c_str()];
    }

    //なかったら空の値を返す
    rapidjson::Value null;
    null.SetObject();
    return null;
}

CharaObjBase::AxisData::AxisData(const std::string& axis, CharaObjBase* obj, const float rad)
    :object(obj)
    , axisType(axis)
    , aimDir(VGet(0, 0, -1))
    , rotRad(rad)
    , rotVel()
    , rotMat(MGetIdent())
{
    if (axisType == obj->xAxis)
    {
        rotMat = MGetRotX(rotVel);
    }
    else if (axisType == obj->yAxis)
    {
        rotMat = MGetRotY(rotVel);
    }
    else if (axisType == obj->zAxis)
    {
        rotMat = MGetRotZ(rotVel);
    }
}

void CharaObjBase::AxisData::Rotate(const VECTOR& dir, const float& velocity)
{
    if (VSize(dir) > 0)
    {
        aimDir = dir;
        //回転方向算出して回転量分回転
        rotVel += CalcRotDir(velocity);
        if (rotVel > DX_PI_F * 2.0f)
        {
            rotVel = 0;
        }

        object->objDir = VTransform(object->objDir, rotMat);
        if (axisType == object->xAxis)
        {
            rotMat = MGetRotX(rotVel);
        }
        else if (axisType == object->yAxis)
        {
            rotMat = MGetRotY(rotVel);
        }
        else if (axisType == object->zAxis)
        {
            rotMat = MGetRotZ(rotVel);
        }
    }
}

void CharaObjBase::AxisData::RotateToAim(const VECTOR& dir,const float& velocity)
{
    if (VSize(dir) > 0)
    {
        //現方向が目標方向でなければ回転中にする
        aimDir = dir;
        if (VDot(object->objDir, aimDir) < SIMILAR_ANGLE)
        {
            object->nowRotate = true;
        }

        //回転中なら
        if (object->nowRotate)
        {
            //目標方向付近まで回転
            if (VDot(object->objDir, aimDir) < SIMILAR_ANGLE)
            {
                //回転方向算出して回転量分回転
                rotVel += CalcRotDir(velocity);

                object->objDir = VTransform(object->objDir, rotMat);
                if (axisType == object->xAxis)
                {
                    rotMat = MGetRotX(rotVel);
                }
                else if (axisType == object->yAxis)
                {
                    rotMat = MGetRotY(rotVel);
                }
                else if (axisType == object->zAxis)
                {
                    rotMat = MGetRotZ(rotVel);
                }
            }
            else
            {
                //目標方向にして回転停止
                rotMat = MGetRotVec2(object->objDir, aimDir);
                object->objDir = aimDir;
                object->nowRotate = false;
            }
        }
    }
}

void CharaObjBase::AxisData::RotateToAim(const VECTOR& dir)
{
    //目標方向に回転
    rotMat = MMult(MGetIdent(), MGetRotVec2(aimDir, dir));
    object->objDir = dir;
}

float CharaObjBase::AxisData::CalcRotDir(const float& velocity)
{
    //ラジアン角にして回転行列へ変換
    rotRad = math::DegToRad(velocity);
    if (axisType == object->xAxis)
    {
        rotMat = MGetRotX(rotRad);
    }
    else if (axisType == object->yAxis)
    {
        rotMat = MGetRotY(rotRad);
    }
    else if(axisType== object->zAxis)
    {
        rotMat = MGetRotZ(rotRad);
    }

    //回転方向を算出
    if (VCross(object->objDir, aimDir).y < 0)
    {
        rotMat = MTranspose(rotMat);
        return -rotRad;
    }
    return rotRad;
}
