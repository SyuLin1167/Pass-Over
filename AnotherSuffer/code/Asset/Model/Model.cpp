#include<DxLib.h>
#include<future>

#include "Model.h"

Model::Model()
    :AssetBase()
{
    //jsonファイル読み込み
    jsonFile = "../json/ModelData.json";
    LoadJsonFile(jsonFile);

    //オブジェクト分ハンドル追加
    for (rapidjson::Value::ConstMemberIterator objType = GetJsonData().MemberBegin();
        objType != GetJsonData().MemberEnd(); objType++)
    {
        AddHandle(objType->value.GetString());
    }
}

Model::~Model()
{
    DeleteHandle();
}

void Model::AddHandle(const std::string fileName)
{
    //仮ハンドル初期化
    holdHandle = -1;
    dupHandle = -1;

    //ファイルが見つからなかったらハンドルを複製して追加
    auto iter = handle.find(fileName);
    if (iter == handle.end())
    {
            holdHandle = MV1LoadModel(fileName.c_str());
            dupHandle = MV1DuplicateModel(holdHandle);
            handle.emplace(fileName, dupHandle);
    }
}

void Model::DeleteHandle()
{
    //ハンドルとデータ解放
    for (auto& iter : handle)
    {
        MV1DeleteModel(iter.second);
    }
    handle.clear();
}