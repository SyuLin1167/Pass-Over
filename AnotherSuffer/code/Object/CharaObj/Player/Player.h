#pragma once
#include"../CharaObjBase/CharaObjBase.h"

/// <summary>
/// Player�N���X
/// </summary>
class Player final:public CharaObjBase
{
public:
    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    Player();

    /// <summary>
    /// �f�X�g���N�^
    /// </summary>
    ~Player();

private:
    /// <summary>
    /// �X�V����
    /// </summary>
    /// <param name="deltaTime">�f���^�^�C��</param>
    void Update(const float deltaTime) override;

    /// <summary>
    /// �L�������쏈��
    /// </summary>
    /// <param name="deltaTime">:�f���^�^�C��</param>
    void MoveChara(const float deltaTime)override;

    /// <summary>
    /// �L�[���͂ɂ��ړ�����
    /// </summary>
    /// <param name="keyName">:�L�[��</param>
    /// <param name="dir">:�ړ�����</param>
    /// <param name="deltaTime">:�f���^�^�C��</param>
    void MoveByKey(const int keyName, const VECTOR dir, const float deltaTime);

    /// <summary>
    /// �`�揈��
    /// </summary>
    void Draw() override;

    const float RUN_SPEED = 10.0f;
    float a = 0;
    int texHandle;
    class Capsule* capsule;
};
