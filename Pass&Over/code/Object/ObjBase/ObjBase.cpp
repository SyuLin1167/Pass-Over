#include "ObjBase.h"

ObjBase::ObjBase(ObjTag tag)
    :objTag(tag)
    , isAlive(true)
    , isVisible(true)
    , objHandle(-1)
    , objPos(VGet(0, 0, 0))
    , objDir(VGet(1, 0, 0))
{
    //処理なし
}

ObjBase::~ObjBase()
{
    //処理なし
}

ObjTag& ObjBase::GetTag()
{
    return objTag;
}