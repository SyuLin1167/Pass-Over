#include "Stage.h"

Stage::Stage()
    :ObjBase(ObjTag.STAGE)
{
    objHandle = model->GetHandle(model->GetJsonData()[objTag.c_str()].GetString());
    objLocalPos = VGet(30, 0, 20);
}

Stage::~Stage()
{
    //処理なし
}

void Stage::Update(const float deltaTime)
{
    CalcObjPos();

    //行列でモデルの動作
    MV1SetMatrix(objHandle, MMult(MGetScale(objScale), MGetTranslate(objPos)));
}

void Stage::Draw()
{
    MV1DrawModel(objHandle);
}