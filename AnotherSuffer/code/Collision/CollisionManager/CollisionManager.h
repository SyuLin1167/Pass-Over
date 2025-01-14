#pragma once
#include<../Rapidjson/istreamwrapper.h>
#include<../Rapidjson/document.h>
#include<fstream>
#include<vector>
#include<unordered_map>

#include"../../Object/ObjBase/ObjBase.h"
#include"../CollisionBase/CollisionBase.h"

/// <summary>
/// 当たり判定の管理
/// </summary>
class CollisionManager final
{
public:
    /// <summary>
    /// コリジョンマネージャー初期化
    /// </summary>
    static void InitColManager();

    /// <summary>
    /// 当たり判定追加
    /// </summary>
    /// <param name="obj">:オブジェクト</param>
    /// <param name="col">:当たり判定</param>
    static void AddCol(ObjBase* obj, CollisionBase* col);

    /// <summary>
    /// 当たり判定取得
    /// </summary>
    /// <param name="obj">:オブジェクト</param>
    /// <returns>オブジェクトの当たり判定</returns>
    static const std::vector<CollisionBase*> GetCol(ObjBase* obj);

    /// <summary>
    /// 当たり判定組み合わせ判別
    /// </summary>
    static void CheckCollisionPair();

    /// <summary>
    /// 当たり判定削除
    /// </summary>
    static void DeleteCollision();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~CollisionManager();
private:
    /// <summary>
    /// コンストラクタ(シングルトン)
    /// </summary>
    CollisionManager();

    /// <summary>
    /// 当たり判定
    /// </summary>
    /// <param name="mainObj">:主オブジェクトタグ</param>
    /// <param name="pairObj">:ペアオブジェクトタグ</param>
    static void OnCollisionEnter(std::string mainObjTag, std::string pairObjTag);

    static std::unique_ptr<CollisionManager> singleton;     //自身の実体
    rapidjson::Document doc;                                //ドキュメント
    std::unordered_map<ObjBase*, std::vector<CollisionBase*>> colData;   //当たり判定データ
};

