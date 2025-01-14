#include "TitleCamera.h"
#include"../../../Object/ObjManager/ObjManager.h"
#include"../../Math/Math.h"

static constexpr int PLAYER_HEAD_FRAME = 10;      //プレイヤー頭部フレーム

TitleCamera::TitleCamera()
    :ObjBase(ObjTag.CAMERA)
    ,rotAngle()
    ,rotRad()
{
    objWorldPos = VGet(-5, 0, -5);
    SetCameraNearFar(0.1f, 500.0f) ;
    //仮ライト
    handle = CreatePointLightHandle(objPos,  150.0f, 0.0f, 0.0001f, 0.0005f);
}

TitleCamera::~TitleCamera()
{
    DeleteLightHandle(handle);
}

void TitleCamera::Update(const float deltaTime)
{
    //カメラを無造作に移動
    rotAngle += deltaTime;
    rotRad = math::DegToRad(rotAngle);
    float moveX = cosf(rotAngle) - cosf(rotAngle) * sinf(rotAngle);
    float moveZ= sinf(rotAngle) - sinf(rotAngle) * cosf(rotAngle);
    objLocalPos = VAdd(objWorldPos, VGet(moveX * moveX, cosf(rotAngle) * sinf(rotAngle) + 50, moveZ * moveZ));

    //座標更新
    CalcObjPos();
    SetLightPositionHandle(handle, objPos);
}

void TitleCamera::Draw()
{
    SetCameraPositionAndTarget_UpVecY(objPos,VGet(50,10,20));
}