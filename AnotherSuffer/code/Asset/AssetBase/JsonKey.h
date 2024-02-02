#pragma once
#include <string>

namespace jsondata
{
    /// <summary>
    /// jsonオブジェクト用のキー構造体
    /// </summary>
    static struct JsonObjKey
    {
        std::string nomal = "nomal";
        std::string walk = "walk";
        std::string look = "look";
        std::string hide = "hide";
        std::string wall="wall";
        std::string barricade="barricade";
        std::string aisle="aisle";
    }objKey;

    /// <summary>
    /// jsonデータ用のキー構造体
    /// </summary>
    static struct JsonDataKey
    {
        std::string pass = "pass";
        std::string loop = "loop";
        std::string speed = "speed";
        std::string type = "type";
        std::string volume = "volume";
    }dataKey;
}