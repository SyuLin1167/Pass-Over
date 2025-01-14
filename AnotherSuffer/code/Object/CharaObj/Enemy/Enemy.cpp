#include<math.h>
#include<stack>

#include"../../ObjManager/ObjManager.h"
#include"../../../Asset/AssetManager/AssetManager.h"
#include"../../../Collision/CollisionManager/CollisionManager.h"
#include"../../../Object/StageObj/StageManager/StageManager.h"
#include "Enemy.h"
#include"../../../Collision/Capsule/Capsule.h"
#include"../../../Collision/Line/Line.h"
#include"../Astar/Astar.h"

static constexpr float RESET_TIME = 2.0f;   //リセット時間
static constexpr float MAX_DISTANCE = 1000.0f;  //最大距離
static constexpr float CLEAR_DISTANCE = 0.0f;   //初期距離
static constexpr float CLIP_BOX_SIZE = 150.0f;     //切り抜きボックスサイズ
static constexpr float MOVE_SPEED = 70.0f;
static constexpr float CAPSULE_RAD = 8.0f;

Enemy::Enemy()
    :CharaObjBase(ObjTag.ENEMY)
    , timer()
    , clipBoxScale()
    , clipBoxPos1()
    , clipBoxPos2()
    , isScream(false)
{
    //モデル読み込み
    objHandle = MV1DuplicateModel(AssetManager::ModelInstance()->GetHandle(modelData.GetString()));
    objDir = VGet(0, 0, -1);
    objScale = VGet(0.15f, 0.15f, 0.15f);
    auto stage=StageManager::GetStageData();
    objLocalPos = stage[15][15].pos;
    CalcObjPos();
    MV1SetMatrix(objHandle, MMult(MMult(MGetScale(objScale), YAxisData->GetRotateMat()), MGetTranslate(objPos)));

    AssetManager::MotionInstance()->StartMotion(this,
        AssetManager::MotionInstance()->GetHandle(
            AssetManager::GetFilePass(motionData[jsondata::objKey.walk.c_str()])));

    //当たり判定はカプセル型
    capsule = new Capsule(VAdd(objWorldPos, VGet(0, 6, 0)), VAdd(objWorldPos, VGet(0, 30, 0)), CAPSULE_RAD);
    capsule->Update(objPos);
    CollisionManager::AddCol(this, capsule);
    line = new Line(objWorldPos, objWorldPos);
    CollisionManager::AddCol(this, line);

    //経路探索設定
    astar.reset(new Astar);
    player = ObjManager::GetObj(ObjTag.PLAYER,0);

    ResetNode(objPos, &start);
    ResetNode(player->GetObjPos(), &goal);
    path = astar->Algorithm(start, goal);

}

Enemy::~Enemy()
{
    path.clear();
}

void Enemy::Update(const float deltaTime)
{
    //残り障壁数に応じて加速
    moveSpeed = MOVE_SPEED - (StageManager::GetBarricadeNum() * 1.5f);

    //リセット時間ごとに経路の再構成を行う
    if (!isScream)
    {
        timer += deltaTime;
        if (timer >= RESET_TIME)
        {
            path.clear();
            ResetNode(objPos, &start);
            ResetNode(player->GetObjPos(), &goal);
            path = astar->Algorithm(start, goal);
            timer = 0;
        }


        if (isMove)
        {
            AssetManager::MotionInstance()->AddMotionTime(this, deltaTime);
            MoveChara(deltaTime);
        }

        //通常は動く
        isMove = true;
        ViewClipBox();
    }
    else
    {
        AssetManager::MotionInstance()->StartMotion(this,
            AssetManager::MotionInstance()->GetHandle(
                AssetManager::GetFilePass(motionData[jsondata::objKey.scream.c_str()])));

        AssetManager::MotionInstance()->AddMotionTime(this, deltaTime);

        if (!AssetManager::MotionInstance()->IsPlaying(AssetManager::MotionInstance()->GetHandle(
            AssetManager::GetFilePass(motionData[jsondata::objKey.scream.c_str()]))))
        {
            isAlive = false;
        }
    }

    //座標更新
    CalcObjPos();

    //行列でモデルの動作
    MV1SetMatrix(objHandle, MMult(MMult(MGetScale(objScale), YAxisData->GetRotateMat()), MGetTranslate(objPos)));

}

void Enemy::MoveChara(const float deltaTime)
{
    //経路が空じゃなかったら
    if (!path.empty())
    {
        //経路通りに進む
        auto stage = StageManager::GetStageData();
        for (auto& point : path)
        {
            float dis = abs(VSize(VSub(stage[point.first][point.second].pos, objPos)));
            if (dis > 8.0f)
            {
                //目標地点に移動
                VECTOR vec = VNorm(VSub(stage[point.first][point.second].pos, objPos));
                vec.y = 0;
                objLocalPos = VAdd(objLocalPos, VScale(vec, moveSpeed * deltaTime));
                YAxisData->RotateToAim(vec);
                break;
            }
            else
            {
                //目標地点に一定距離近づいたら、次の地点に移動
                path.erase(path.begin());
            }
        }
    }
    else
    {
        VECTOR vec = VNorm(VSub(player->GetObjPos(), objPos));
        objLocalPos = VAdd(objLocalPos, VScale(vec, moveSpeed * deltaTime));
    }
}

void Enemy::ViewClipBox()
{
    //視野にクリップボックスがあったら動かない
    clipBoxScale = VScale(objScale, CLIP_BOX_SIZE);
    clipBoxPos1 = VSub(objPos, clipBoxScale);
    clipBoxPos2 = VAdd(objPos, VAdd(clipBoxScale, VGet(0, clipBoxScale.y, 0)));
    if (!CheckCameraViewClip_Box(clipBoxPos1, clipBoxPos2))
    {
        isMove = false;
    }
}

void Enemy::OnCollisionEnter(ObjBase* colObj)
{
    //当たり判定処理
    for (auto& obj : CollisionManager::GetCol(colObj))
    {
        if (obj->GetColTag() == ColTag.MODEL)
        {
            if (capsule->OnCollisionWithMesh(obj->GetColModel()))
            {
                objLocalPos = VAdd(objLocalPos, capsule->CalcPushBackFromMesh());

                MV1CollResultPolyDimTerminate(capsule->GetColInfoDim());
            }
            if (line->OnCollisionWithMesh(obj->GetColModel()))
            {
                isMove = true;
            }
            continue;
        }
        if (obj->GetColTag() == ColTag.CAPSULE)
        {
            if (capsule->OnCollisionWithCapsule(obj->GetWorldStartPos(), obj->GetWorldEndPos(), obj->GetRadius() * 2.5f) || !player->IsVisible())
            {
                isScream = true;
                YAxisData->RotateToAim(VNorm(VSub(player->GetObjPos(), objPos)));
            }
        }
    }

    //座標更新
    CalcObjPos();
    capsule->Update(objPos);
    line->Update(objPos, player->GetObjPos());

    //行列でモデルの動作
    MV1SetMatrix(objHandle, MMult(MMult(MGetScale(objScale), YAxisData->GetRotateMat()), MGetTranslate(objPos)));
}

void Enemy::ResetNode(VECTOR pos, std::pair<int, int>* node)
{
    //距離初期化
    float nowDistance = MAX_DISTANCE;
    float distance = CLEAR_DISTANCE;

    //横軸探索
    for (auto& rowData : StageManager::GetStageData())
    {
        //現在の距離より近い距離にあるならノード書き換え
        distance = abs(rowData.second[0].pos.z - pos.x);
        if (nowDistance > distance)
        {
            nowDistance = distance;
            node->first = rowData.first;
        }
        else
        {
            //横軸で最短ノードが決まったら縦軸探索
            nowDistance = MAX_DISTANCE;
            for (auto& colData : rowData.second)
            {
                //現在の距離より近い距離にあるならノード書き換え
                distance = abs(colData.second.pos.x - pos.z);
                if (nowDistance > distance)
                {
                    nowDistance = distance;
                    node->second = colData.first;
                }
                else
                {
                    //縦軸で最短ノードが決まったら探索終了
                    break;
                }
            }
            break;
        }
    }
}

void Enemy::Draw()
{
    //モデル描画
    MV1DrawModel(objHandle);

#ifdef _DEBUG
    if (path.empty()) {
        DrawFormatString(0, 50, GetColor(255, 255, 255), "ゴールに到達できませんでした。");
    }
    else {
        DrawFormatString(0, 100, GetColor(255, 255, 255), "最短経路: ");
        for (const auto& point : path) {
            DrawFormatString(point.second * 50, point.first * 50 + 200, GetColor(255, 255, 255), "(%d,%d)", point.first, point.second);
        }
    }

    //当たり判定描画
    capsule->DrawCapsule();

    DrawFormatString(800, 0, GetColor(255, 255, 255), "敵座標%f,%f\n移動速度%f", capsule->GetWorldStartPos().x, capsule->GetWorldStartPos().z,moveSpeed);
#endif // _DEBUG
}