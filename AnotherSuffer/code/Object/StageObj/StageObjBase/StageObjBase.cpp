
#include "StageObjBase.h"
#include"../../../KeyStatus/KeyStatus.h"
#include"../../ObjManager/ObjManager.h"
#include"../../../Collision/CollisionManager/CollisionManager.h"
#include"../../../Collision/ColModel/ColModel.h"
#include"../../../UI/MiniMap/MiniMap.h"

static constexpr float CLIP_BOX_SIZE = 100.0f;     //切り抜きボックスサイズ
static constexpr float CLIP_DISTANCE = 150.0f;     //切り抜き距離

StageObjBase::StageObjBase(const VECTOR pos)
    :ObjBase(ObjTag.STAGE)
    , graphData(AssetManager::GraphInstance()->GetJsonData()[objTag.c_str()])
    , color()
    , clipBoxScale()
    , clipBoxPos1()
    , clipBoxPos2()
    , texHandle(-1)
    , texIndex()
{
    objHandle = MV1DuplicateModel(AssetManager::ModelInstance()->GetHandle(
    AssetManager::ModelInstance()->GetJsonData()[objTag.c_str()].GetString()));
    MV1SetMeshBackCulling(objHandle, -1, true);
    MV1SetUseZBuffer(objHandle, true);
    objLocalPos = pos;

    objScale = VGet(0.2f, 0.4f, 0.2f);
    MV1SetScale(objHandle, objScale);

    player = nullptr;
#ifdef _DEBUG
    MV1SetOpacityRate(objHandle, 0.3f);
#endif // _DEBUG

    //行列でモデルの動作
    CalcObjPos();
    MV1SetMatrix(objHandle, MMult(MGetScale(objScale), MGetTranslate(objPos)));
}


void StageObjBase::InitCollision()
{
    //当たり判定はモデル型
    colModel.reset(new ColModel(objHandle));
    CollisionManager::AddCol(this, colModel.get());
}


StageObjBase::~StageObjBase()
{
    //処理なし
}

void StageObjBase::Update(const float deltaTime)
{
    //モデル切り抜き
    player = ObjManager::GetObj(ObjTag.PLAYER, 0);
    if (player)
    {
        ViewClipBox();
    }

    //行列でモデルの動作
    CalcObjPos();
    MV1SetMatrix(objHandle, MMult(MGetScale(objScale), MGetTranslate(objPos)));
}

void StageObjBase::ViewClipBox()
{
    //プレイヤーから一定距離離れていたら描画しない
    VECTOR distance = VSub(objPos, player->GetObjPos());
    if (VSize(distance) > CLIP_DISTANCE)
    {
       isVisible = false;
        return;
    }

    //視野にクリップボックスがなかったら描画しない
    clipBoxScale = VScale(objScale, CLIP_BOX_SIZE);
    clipBoxPos1 = VSub(objPos, clipBoxScale);
    clipBoxPos2 = VAdd(objPos, VAdd(clipBoxScale, VGet(0, clipBoxScale.y, 0)));
    if (CheckCameraViewClip_Box(clipBoxPos1, clipBoxPos2))
    {
       isVisible = false;
        return;
    }

    //基本は描画
    isVisible = true;
    MiniMap::AddMapData(objPos, color);
}

void StageObjBase::Draw()
{
    //モデル描画
    MV1SetTextureGraphHandle(objHandle, texIndex, texHandle, true);
    MV1DrawModel(objHandle);

#ifdef _DEBUG
    DrawBox(objPos.z/2 +820, objPos.x/2 + 400, objPos.z/2 +840, objPos.x/2  +420 , color, true);
#endif // _DEBUG
}