#include"../../../KeyStatus/KeyStatus.h"
#include"../../ObjManager/ObjManager.h"
#include"../../../Collision/Sphere/Sphere.h"
#include "Player.h"

Player::Player()
    :CharaObjBase(ObjTag.PLAYER)
{
    //モデル読み込み
    texHandle = LoadGraph("../assets/model/Chara/Player/PlayerFace.png");
    objHandle = model->GetHandle(modelData.GetString());
    objDir = VGet(0, 0, -1);
    MV1SetMatrix(objHandle, MMult(rotateMat, MGetTranslate(objPos)));

    motion->StartMotion(objHandle,
        motion->GetHandle(GetFilePass(motionData[jsondata::objKey.nomal.c_str()])));

    //移動速度は走る速度
    moveSpeed = RUN_SPEED;

    capsule = new Capsule(VAdd(objPos, VGet(0, 5, 0)), VAdd(objPos, VGet(0, 25, 0)), 5.0f);
}

Player::~Player()
{
    //処理なし
}

void Player::Update(const float deltaTime)
{
    a += deltaTime;
    //アニメーション時間再生
    motion->AddMotionTime(deltaTime);

    //キャラ移動
    MoveChara(deltaTime);

    //停止中
    if(!isMove)
    {
        //通常時アニメーション再生
        motion->StartMotion(objHandle,
            motion->GetHandle(GetFilePass(motionData[jsondata::objKey.nomal.c_str()])));
    }

    //Pキーが押されたら
    if (KeyStatus::KeyStateDecision(KEY_INPUT_P, ONINPUT))
    {
        //死亡にする
        isAlive = false;

        //サウンド再生
        sound->StartSound(sound->GetHandle(GetFilePass(soundData[jsondata::objKey.walk.c_str()])));
    }

    colInfo = {};
    if (capsule->OnCollisionWithMesh(ObjManager::GetObj(ObjTag.STAGE)[0]->GetObjHandle(), colInfo))
    {
        a = 0;
        VECTOR pushBack = capsule->CalcPushBackFromMesh(colInfo);
        //objLocalPos = VAdd(objLocalPos, pushBack);
        MV1CollResultPolyDimTerminate(colInfo);
        objLocalPos.y = 0;
    }
        //オブジェクトの座標算出
        CalcObjPos();

        capsule->Update(objPos);

        //行列でモデルの動作
        MV1SetMatrix(objHandle, MMult(rotateMat, MGetTranslate(objPos)));


        //停止中にする
    isMove = false;
}

void Player::MoveChara(const float deltaTime)
{
    //カメラの向きを自身の移動方向とする
    std::shared_ptr<ObjBase> camera = ObjManager::GetObj(ObjTag.CAMERA)[0];
    VECTOR aimDir = camera->GetObjDir();
    VECTOR rightDir = VCross(VGet(0, -1, 0), aimDir);
    aimDir.y = 0;
    aimDir = VNorm(aimDir);

    //キー入力による上下左右移動
    moveVel = VGet(0, 0, 0);
    MoveByKey(KEY_INPUT_W, aimDir, deltaTime);
    MoveByKey(KEY_INPUT_S, VScale(aimDir, -1), deltaTime);
    MoveByKey(KEY_INPUT_A, rightDir, deltaTime);
    MoveByKey(KEY_INPUT_D, VScale(rightDir, -1), deltaTime);


    //座標・方向の算出
    objLocalPos = VAdd(objLocalPos, VScale(moveVel, moveSpeed * deltaTime));
    rotateMat = MMult(MGetScale(objScale), MGetRotVec2(objDir, aimDir));
}

void Player::MoveByKey(const int keyName, const VECTOR dir, const float deltaTime)
{
    //キーが入力されていたら移動時の処理実行
    if (KeyStatus::KeyStateDecision(keyName, ONINPUT | NOWONINPUT))
    {
        moveVel = VNorm(VAdd(moveVel, dir));

        //移動アニメーション再生
        motion->StartMotion(objHandle,
            motion->GetHandle(GetFilePass(motionData[jsondata::objKey.walk.c_str()])));

        //動作中にする
        isMove = true;
    }
}

void Player::Draw()
{
    // テクスチャで使用するグラフィックハンドルを変更する
    int texIndex = MV1GetMaterialDifMapTexture(objHandle, 0);
    MV1SetTextureGraphHandle(objHandle, texIndex, texHandle, FALSE);

    //モデル描画
    MV1DrawModel(objHandle);

    // 当たったポリゴンの数を描画
    DrawFormatString(0, 150, GetColor(255, 255, 255), "Hit Poly Num   %d", colInfo.HitNum);

    // 当たったポリゴンの数だけ繰り返し
    for (int i = 0; i < colInfo.HitNum; i++)
    {
        // 当たったポリゴンを描画
        DrawTriangle3D(
            colInfo.Dim[i].Position[0],
            colInfo.Dim[i].Position[1],
            colInfo.Dim[i].Position[2], GetColor(0, 255, 255), TRUE);
    }

    DrawFormatString(0, 20, GetColor(255, 255, 255), "%f", a);
    DrawLine3D(objPos, VAdd(objPos, VScale(objDir,3)), GetColor(255, 0, 0));

    capsule->DrawDebug();
}