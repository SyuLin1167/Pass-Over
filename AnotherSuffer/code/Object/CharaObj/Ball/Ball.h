#pragma once
#include"../CharaObjBase/CharaObjBase.h"

/// <summary>
/// ボール
/// </summary>
class Ball final:public CharaObjBase
{
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    Ball();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~Ball();

private:
    /// <summary>
    /// 更新
    /// </summary>
    /// <param name="deltaTime">デルタタイム</param>
    void Update(const float deltaTime) override;

    /// <summary>
    /// キャラ動作
    /// </summary>
    /// <param name="deltaTime">:デルタタイム</param>
    void MoveChara(const float deltaTime)override;

    /// <summary>
    /// キー入力による移動
    /// </summary>
    /// <param name="keyName">:キー名</param>
    /// <param name="dir">:移動方向</param>
    /// <param name="deltaTime">:デルタタイム</param>
    void MoveByKey(const int keyName, const VECTOR dir, const float deltaTime);

    /// <summary>
    /// 当たり判定
    /// </summary>
    /// <param name="colObj">:判定するオブジェクト</param>
    void OnCollisionEnter(class ObjBase* colObj) override;

    /// <summary>
    /// 描画
    /// </summary>
    void Draw() override;

    MATRIX RotateVel();
    bool canMove;
    float a = 0;
    class Capsule* capsule;
    VECTOR moveVel;
};

