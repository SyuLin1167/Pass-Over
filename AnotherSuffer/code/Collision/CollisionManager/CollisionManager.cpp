#include "CollisionManager.h"
#include"../../Object/ObjManager/ObjManager.h"

std::unique_ptr<CollisionManager> CollisionManager::singleton;

CollisionManager::CollisionManager()
{
    //ファイル読み込み
    std::ifstream ifs("../json/ColPairData.json");

    //ファイル解析
    if (ifs.good())
    {
        rapidjson::IStreamWrapper isw(ifs);

        doc.ParseStream(isw);
    }
    ifs.close();
}

void CollisionManager::InitColManager()
{
    //自身の中身が空だったらインスタンス生成
    if (!singleton)
    {
        singleton.reset(new CollisionManager);
    }
}

void CollisionManager::AddCol(ObjBase* obj, CollisionBase* col)
{
    //当たり判定追加
    singleton->colData[obj].emplace_back(col);
}

CollisionManager::~CollisionManager()
{
    DeleteCollision();
}

const std::vector<CollisionBase*> CollisionManager::GetCol(ObjBase* obj)
{
    //データがあるなら返す
    auto finddata = singleton->colData.find(obj);
    if (finddata != singleton->colData.end())
    {
        return singleton->colData[obj];
    }

    //なかったら空を返す
    return {};
}

void CollisionManager::CheckCollisionPair()
{
    //オブジェクトの当たり判定組み合わせを確認
    for (rapidjson::Value::ConstMemberIterator mainObj = singleton->doc.MemberBegin();
        mainObj != singleton->doc.MemberEnd(); mainObj++)
    {
        for (rapidjson::Value::ConstMemberIterator pairObj = mainObj->value.MemberBegin();
            pairObj != mainObj->value.MemberEnd(); pairObj++)
        {
            //確認したオブジェクトの組み合わせで当たり判定
            OnCollisionEnter(mainObj->name.GetString(), pairObj->name.GetString());
        }
    }
}

void CollisionManager::DeleteCollision()
{
    //データ削除
    singleton->colData.clear();
}

void CollisionManager::OnCollisionEnter(std::string mainObjTag, std::string pairObjTag)
{
    //組み合わせどおりに当たり判定実行
    for (auto& colMain : ObjManager::GetObjData(mainObjTag))
    {
        if (colMain)
        {
            for (auto& colPair : ObjManager::GetObjData(pairObjTag))
            {
                if (colPair)
                {
                    colMain->OnCollisionEnter(colPair.get());
                    if (!colMain->IsAlive())
                    {
                        auto findCol = singleton->colData.find(colMain.get());
                        if (findCol != singleton->colData.end())
                        {
                            singleton->colData.erase(findCol->first);
                        }
                    }
                }
            }
        }
    }
}
