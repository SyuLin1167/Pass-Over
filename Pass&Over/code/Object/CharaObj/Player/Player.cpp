#include"../../../KeyStatus/KeyStatus.h"
#include "Player.h"

Player::Player()
    :CharaObjBase(ObjTag.PLAYER)
{
    //モデル読み込み
    objHandle = model->GetHandle(modelData.GetString());
    MV1SetPosition(objHandle, objPos);
    MV1SetScale(objHandle, objScale);

    motion->StartMotion(objHandle,
        motion->GetHandle(GetFilePass(motionData[jsondata::objKey.nomal.c_str()])));

    moveSpeed = RUN_SPEED;
}

Player::~Player()
{
    //処理なし
}

void Player::Update(const float deltaTime)
{
    a += deltaTime;
    motion->AddMotionTime(deltaTime);
    if (KeyStatus::KeyStateDecision(KEY_INPUT_F, NOWONINPUT))
    {
        motion->StartMotion(objHandle,
            motion->GetHandle(GetFilePass(motionData[jsondata::objKey.walk.c_str()])));
    }
    if (KeyStatus::KeyStateDecision(KEY_INPUT_P, ONINPUT))
    {
        isAlive = false;
        sound->StartSound(sound->GetHandle(GetFilePass(soundData[jsondata::objKey.walk.c_str()])));
    }

    //VECTOR v = MV1GetFrameAvgVertexLocalPosition(objHandle, 2);
    //MATRIX m1 = MGetTranslate(VGet(v.x, v.y, v.z));
    //MATRIX m2 = MGetTranslate(VGet(-v.x, -v.y, -v.z));
    //MV1SetFrameUserLocalMatrix(objHandle, 2, MMult(MMult(m2, MGetRotY(((2 * DX_PI_F) / 60) * a * 5)), m1));


    MoveChara(deltaTime);
}

void Player::MoveChara(const float deltaTime)
{
    MoveByKey(KEY_INPUT_W, FRONT, deltaTime);
    MoveByKey(KEY_INPUT_S, BACK, deltaTime);
    MoveByKey(KEY_INPUT_A, LEFT, deltaTime);
    MoveByKey(KEY_INPUT_D, RIGHT, deltaTime);

    CalcObjPos();
    //MV1SetPosition(objHandle, objPos);
    if (isMove)
    {
        MV1SetMatrix(objHandle, MMult(rotateMat, MGetTranslate(objPos)));
    }
    isMove = false;
}

void Player::MoveByKey(const int keyName, const VECTOR dir, const float deltaTime)
{
    if (KeyStatus::KeyStateDecision(keyName, ONINPUT | NOWONINPUT))
    {
        objLocalPos = VAdd(objLocalPos, VScale(dir, moveSpeed * deltaTime));
        RotateYAxis(dir, ROTATE_SPEED);
        isMove = true;
        return;
    }
}

void Player::Draw()
{
    DrawFormatString(0, 50, GetColor(255, 255, 255), "player:%f", a);
    MV1DrawModel(objHandle);
}