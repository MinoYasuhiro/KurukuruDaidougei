#pragma once
#include "sound/SoundSource.h"
#include "Umbrella.h"

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
	//アニメーションの再生。
	//void PlayAnimation();
	//アニメーションの再生2。
	void PlayAnimation2();
	//走る音の再生。
	//void PlayRunSound();
	////座標を取得する。
	Vector3 GetPosition() const
	{
		return m_position;
	}
	void Reset();   // ★ 追加
	//ココからメンバ変数
	//描画
	ModelRender m_modelRender;
	ModelRender m_NewModelRender;

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

	////アニメーションの再生。
	//enum EnAnimationClip {	//アニメーション
	//	enAnimationClip_Idle,
	//	enAnimationClip_Walk,
	//	enAnimationClip_Jump,
	//	enAnimationClip_Run,
	//	enAnimationClip_Num,

	//};
	//AnimationClip m_animationClips[enAnimationClip_Num];


	enum EnPlayerAnimationState {	//アニメーションの状態
		enPlayerAnimationState_Idle,
		enPlayerAnimationState_Run,
		enPlayerAnimationState_Rotation,
		enPlayerAnimationState_success,
		enPlayerAnimationState_Num,
	};
	AnimationClip m_playerAnimationState[enPlayerAnimationState_Num];


	//ステート管理用の変数。
	int m_playerState = 0;


	//効果音。
	SoundSource* m_jumpSound = nullptr;	//ジャンプの音
	SoundSource* m_runSound = nullptr;	//走る音

private:
	Umbrella* m_umbrella = nullptr;

	// ★スティック回転用
	Vector2 m_prevStick = Vector2(0, 0);

	float CalcStickRotationSpeed();

	int number = 1;

	int m_state = 0;


	int m_prevNumber = 1;
	Vector3 m_startPos = { 0.0f, 0.0f, 0.0f };
};