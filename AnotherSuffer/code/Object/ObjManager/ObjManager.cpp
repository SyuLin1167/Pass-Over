#include<assert.h>

#include "ObjManager.h"

std::unique_ptr<ObjManager> ObjManager::singleton;

ObjManager::ObjManager()
    :object()
{
    //処理なし
}

ObjManager::~ObjManager()
{
    //処理なし
}

void ObjManager::InitObjManager()
{
    //自身の中身が空だったらインスタンス生成
    if (!singleton)
    {
        singleton.reset(new ObjManager);
    }
}

void ObjManager::AddObj(class ObjBase* newObj)
{
    std::string tag = newObj->GetTag();

    singleton->object[tag].emplace_back(newObj);
}

void ObjManager::UpdateObj(const float deltaTime)
{
    //全タグ分更新をまとめて行う
    for (auto& tag : objTagAll)
    {
        for (int i = 0; i < singleton->object[tag].size(); i++)
        {
            singleton->object[tag][i]->Update(deltaTime);
        }
    }
    OnDeadObj();
}

void ObjManager::DrawObj()
{
    //全タグ分描画をまとめて行う
    for (auto& tag : objTagAll)
    {
        for (int i = 0; i < singleton->object[tag].size(); i++)
        {
            //オブジェクトが生存していたら描画させる
            if (singleton->object[tag][i]->IsVisible())
            {
                singleton->object[tag][i]->Draw();
            }
        }
    }
}

void ObjManager::OnDeadObj()
{
    //全タグ分死亡オブジェクトを探して削除
    for (auto& tag : objTagAll)
    {
        for (auto& dead: singleton->object[tag])
        {
            //死んでいたらオブジェクト削除
            if (dead && !dead->IsAlive())
            {
                DeleteObj(dead);
            }
        }
    }
}

void ObjManager::DeleteObj(std::shared_ptr<ObjBase> unnecObj)
{
    //削除オブジェクトのタグ取得
    std::string tag = unnecObj->GetTag();

    //オブジェクトを検索
    auto findObj = std::find(singleton->object[tag].begin(), singleton->object[tag].end(), unnecObj);

    //見つかったら末尾に移動させて削除
    if (findObj != singleton->object[tag].end())
    {
        singleton->object[tag].erase(findObj);
    }
}

void ObjManager::DeleteAllObj()
{
    //空じゃなかったらオブジェクト削除
    for (auto& tag : objTagAll)
    {
        if (!singleton->object[tag].empty())
        {
            singleton->object[tag].clear();
        }
    }
}

std::vector<std::shared_ptr<ObjBase>> ObjManager::GetObjData(std::string tag)
{
    //データがあればタグのオブジェクトデータを返す
    if (singleton->object[tag].empty())
    {
        return {};
    }
    return singleton->object[tag];
}

ObjBase* ObjManager::GetObj(std::string tag, int num)
{
    //データがあればタグの指定オブジェクトを返す
    if (singleton->object[tag].empty()||singleton->object[tag].size() < num)
    {
        return nullptr;
    }
    return singleton->object[tag][num].get();
}