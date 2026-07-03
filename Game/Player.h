#pragma once
#include "sound/SoundSource.h"
#include "Umbrella.h"
#include "Item.h"

class Arrow;
class Circle;
class Player : public IGameObject
{
public:
    // --- コンストラクタ・デストラクタ ---
    Player();
    ~Player();

    // --- オーバーライド ---
    bool Start() override;
    void Update() override;
    void Render(RenderContext& rc) override;

    // --- アクション ---
    void Move();            // 移動処理
    void Rotation();        // 回転処理
    void ManageState();     // ステート管理
    void PlayerAction();    // プレイヤーのアクション
    void PlayAnimation2();  // アニメーションの再生
    void SoundPlay();       // サウンドの再生
    void SpinCount();       // 傘回しの回転数保持
    void Reset();           // リセット処理
    void EndUmbrellaSpin(); // 傘回し終了処理
	void LimitMoveArea();     // 移動範囲制限

	//--- Setter ---
    void SetSpinCountSuccess(int count)
    { 
        m_spinCountSuccess = count;
    }

    // --- Getter ---
    Vector3 GetPosition() const
    {
        return m_position;
    }

    Umbrella* GetUmbrella() const
    {
        return m_umbrella;
    }

    // --- アニメーション状態 ---
    enum EnPlayerAnimationState
    {
        enPlayerAnimationState_Idle,
        enPlayerAnimationState_Run,
        enPlayerAnimationState_Rotation,
        enPlayerAnimationState_Success,
        enPlayerAnimationState_Normal,
        enPlayerAnimationState_QTEsippai,
        enPlayerAnimationState_QTEseikou,
        enPlayerAnimationState_GameClear1,
        enPlayerAnimationState_GameClear2,
        enPlayerAnimationState_GameOver,
        enPlayerAnimationState_GameOverRun,
        enPlayerAnimationState_Num,
    };

    // --- プレイヤーステート ---
    enum EnPlayerState
    {
        enPlayerState_Idle = 0,   // 待機
        enPlayerState_Fail = 1,   // 傘回し失敗
        enPlayerState_Run = 2,   // 移動
        enPlayerState_Spinning = 3,   // 傘回し中
        enPlayerState_Success = 4,   // 傘回し成功
        enPlayerState_QTEFail = 5,   // QTE失敗
        enPlayerState_QTESuccess = 6,   // QTE成功
        enPlayerState_GameClear1 = 7,   // ゲームクリア1
        enPlayerState_GameClear2 = 8,   // ゲームクリア2
        enPlayerState_GameOver = 9,   // ゲームオーバー
        enPlayerState_GameOverRun = 10,  // ゲームオーバー後の走り
        enPlayerState_StartWait = 11, //ゲーム開始待機
        enPlayerState_Stop = 30,  // 完全停止
    };

    // --- モデル描画 ---
    ModelRender m_NewModelRender;

    // --- ゲーム管理 ---
    Game* m_game = nullptr;

    // --- トランスフォーム ---
    Vector3    m_position;   // プレイヤーの座標
    Vector3    m_moveSpeed;  // 移動速度
    Quaternion m_rotation;   // プレイヤーの回転

    // --- キャラクターコントローラー ---
    CharacterController m_characterController;

    // --- アニメーション ---
    AnimationClip m_playerAnimationState[enPlayerAnimationState_Num];

    // --- ステート ---
    int m_playerState = enPlayerState_Idle;

    // --- 状態フラグ ---
    bool m_itemOnUmbrella = false;  // アイテムが傘の上にあるか
    bool m_resetGame = false;  // ゲームリセットフラグ
    bool m_isRunSEPlaying = false;  // 走りSE再生中か
    bool m_isSpinSEPlaying = false;  // 傘回しSE再生中か

    // --- ゲームプレイカウンター ---
    int m_playerError = 0;  // ミス回数

private:
    // --- ゲームパラメータ定数 ---
    static const int   CLEAR_COUNT_TO_WIN = 5;         // クリアに必要な成功回数
    static const int   ERROR_COUNT_TO_GAME_OVER = 3;   // ゲームオーバーになるミス数
    static const int   SPIN_TIME_LIMIT_SEC = 3;        // 傘回しの制限時間（秒）

    static const float SPIN_TIME_LIMIT;                // 傘回しの制限時間（float版）
    static const float GAME_START_DELAY;               // ゲーム開始までの待機時間
    static const float GAME_OVER_RUN_TIME;             // ゲームオーバー後の走り時間

    // --- 関連オブジェクト ---
    Umbrella* m_umbrella = nullptr;
    Arrow* m_arrow = nullptr;
    Circle* m_circle = nullptr;

    // --- 傘回し関連 ---
    float m_spinSpeed = 0.0f;
    float m_spinTimer = 0.0f;
    int   m_spinCount = 0;    // 傘回しのカチカチ回数
    int   m_spinCountSuccess = 0;  //傘回しに必要な回転数。

    // --- スティック入力（傘回し計算用）---
    Vector2 m_prevStick = Vector2(0, 0);  // CalcStickRotationSpeed用
    Vector2 m_prevStick2 = Vector2(0, 0);  // SpinCount用
    float   m_inputCooldown = 0.0f;           // 連続カウント防止用

    // --- ゲーム進行 ---
    int   m_playerClear = 0;
    float m_gameStartTimer = 0.0f;
    bool  m_canPlayerMove = false;
    float m_gameOverRunTimer = 0.0f;

    // --- ステート管理補助 ---
    int        m_prevPlayerState = -1;
    Quaternion m_finishRot;              // ゲームオーバー走り時の向き

    // --- その他 ---
    int     number = 1;
    int     m_prevNumber = 1;
    Vector3 m_startPos = { 0.0f, 0.0f, 0.0f };

    // --- UI ---
    FontRender m_font;

    // --- 内部メソッド ---
    float CalcStickRotationSpeed();  // スティックの回転速度を計算
};