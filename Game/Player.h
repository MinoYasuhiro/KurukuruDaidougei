#pragma once
#include "sound/SoundSource.h"
#include "Umbrella.h"
#include "Item.h"

class Player : public IGameObject
{
public:
	Player();
	~Player();
	bool Start() override;
	//更新処理
	void Update() override;
	//描画処理
	void Render(RenderContext& rc) override;
	//移動処理
	void Move();
	//回転処理。
	void Rotation();
	//ステート管理。
	void ManageState();
	//プレイヤーのアクション
	void PlayerAction();
	//アニメーションの再生2。
	void PlayAnimation2();
	//サウンドの再生。
	void SoundPlay();
	//傘回しの回転数保持
	void SpinCount();
	////座標を取得する。
	Vector3 GetPosition() const
	{
		return m_position;
	}
	void Reset();   // ★ 追加
	void EndUmbrellaSpin();

	//ココからメンバ変数
	//描画
	ModelRender m_modelRender;
	ModelRender m_NewModelRender;

	Game* m_game = nullptr;

	//プレイヤーの位置や回転、当たり判定。
	Vector3 m_position;			//プレイヤーの座標。
	Vector3 m_playerPosition;	//プレイヤーの初期位置
	Vector3 m_respawnPosition; //プレイヤーの復活ポジション
	Vector3 m_moveSpeed;		//移動速度。 
	Quaternion m_rot;			//障害物に当たった時の回転の初期化用。
	Quaternion m_rotation;		//プレイヤーのスティックの回転。
	CharacterController m_characterController;	//キャラクターコントローラー

	bool m_wasMoving = false;
	bool m_isRunSEPlaying = false;
	bool m_isSpinSEPlaying = false;

	enum EnPlayerAnimationState {	//アニメーションの状態
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
	AnimationClip m_playerAnimationState[enPlayerAnimationState_Num];


	//ステート管理用の変数。
	int m_playerState = 0;


	//効果音。
	SoundSource* m_jumpSound = nullptr;	//ジャンプの音
	SoundSource* m_runSound = nullptr;	//走る音

	bool m_itemOnUmbrella = false;	//アイテムが傘の上にあるかどうか

	bool m_resetGame = false;	//ゲームリセットフラグ
private:
	Umbrella* m_umbrella = nullptr;

	// ★スティック回転用
	Vector2 m_prevStick = Vector2(0, 0);

	float CalcStickRotationSpeed();

	int number = 1;
	int m_state = 0;
	int m_prevPlayerState = -1;
	Quaternion m_finishRot;
	int playerSpeed = 10;

	int m_prevNumber = 1;

	Vector3 m_startPos = { 0.0f, 0.0f, 0.0f };

	Vector2 m_prevStick2 = Vector2(0, 0);
	float   m_inputCooldown = 0.0f;
	int     m_spinCount = 0;   // カチカチ回数

	float m_spinSpeed = 0.0f;

	FontRender m_font;

	Item* m_item = nullptr;
	
	float m_gameStartTimer = 0.0f;
	bool m_canPlayerMove = false;

	float m_timer = 0.0f;
	
};