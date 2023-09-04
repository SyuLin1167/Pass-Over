#pragma once


/*Titleクラス*/
class Title: public SceneBase
{
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    Title();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~Title();

    /// <summary>
    /// シーン更新処理
    /// </summary>
    /// <param name="deltaTime">:フレームレート</param>
    /// <returns>次フレームのシーン</returns>
    SceneBase* SceneUpdate(const float& deltaTime)override;

    /// <summary>
    /// シーン描画処理
    /// </summary>
    void SceneDraw()override;
};

