#pragma once
#include "BallState.h"

class Game;
class Player;
class Circle;


enum ItemType
{
	ball,
	egg,
	skeleton,
	box,
	teaBowl,
	penguin,
	phone,
};

enum ItemCategory
{
	Normal,
	QTE,
};

class Item :public IGameObject
{
public:
	Item();
	~Item();

	//コピー禁止
	Item(const Item&) = delete;
	Item& operator=(const Item&) = delete;

	bool Start()override;
	void Update()override;
	void Render(RenderContext& renderContext)override;

	//QTEアイテムかどうか
	bool IsQTEItem()const { return m_category == ItemCategory::QTE; }
	//有効かどうか
	bool IsActive()const { return m_isActive; }

	//初期化
	void Init(ItemType type);
	void SetState(BallState newState);	//状態切り替え
	void Move();			//アイテムを飛ばす
	void RandomSpawn();		//アイテムの初期位置をランダム設定
	void ParabolicMotion();	//放物運動による移動処理
	void RequestFire();		//プレイヤーが成功したら呼ぶ
	void OnBallLanded();	//着地時に呼ばれるイベント
	void OnUmbrella();		//傘に乗った瞬間の処理
	void StartQTE();		//QTE開始
	void SpinningFailed();	//傘回し失敗時
	void FailFallMotion();	//失敗時の落下モーション
	void DropPrepare();		//傘から落とす準備
	void PrepareParabola();	//放物準備
	void SetActive(bool isActive);	//有効フラグ

	//現在の速度から予測される着地点を取得
	Vector3 GetPlannedLandingPosition()const;

	//現在の状態での着地点を取得
	Vector3 GetLandingPosition()const;

	//飛行中かどうかを取得
	bool IsFlying()const { return m_isFlying; }

	//状態取得
	BallState GetState()const { return m_state; }

private:

	//ペンギンのアニメーション
	enum EnItemAnimation
	{
		enPenguinAnimation_Run,
		enPenguinAnimation_Num,
	};

	AnimationClip m_penguinAnimationState[enPenguinAnimation_Num];

	Game* m_game = nullptr;
	Player* m_player = nullptr;
	Circle* m_circle = nullptr;

	ItemType m_type = ItemType::ball;
	ItemCategory m_category;
	BallState m_state = BallState::Idle;			//アイテムの状態
	ModelRender* m_modelRender = nullptr;				//アイテムモデル
	ModelRender* m_failureModelRender = nullptr;		//アイテム差分モデル
	Vector3 m_position = Vector3::Zero;				//現在位置
	Vector3 m_moveSpeed = Vector3::Zero;			//現在の移動速度
	Vector3 m_gravity = { 0.0f,-0.5f,0.0f };		//重力加速度
	Vector3 m_plannedVelocity;						//発射予定の初速
	bool m_isFlying = false;						//球が飛んでいるかどうか
	bool m_isCracked = false;						//割れ状態かどうか
	bool m_wasOnUmbrella = false;					//傘に一度乗ったか
	bool m_hasPlayedLandSE = false;					//SE再生済みフラグ
	bool m_isProcessed = false;						//成功・失敗処理をすでに実行済みかどうか
	bool m_hasFailureModel = false;					//割れた状態用のモデルを持っているかどうか
	bool m_isModelInited = false;					//通常モデルの初期化が完了しているかどうか
	bool m_isFailureModelInited = false;			//割れモデルの初期化が完了しているかどうか
	bool m_isDead = false;							//このアイテムが不要(削除対象)かどうか
	bool m_isActive = true;							//更新・描画を行うかどうか(非アクティブなら停止)
	float m_onUmbrellaTimer = 0.0f;					//傘の上にいる時間
	float m_onUmbrellaLimitTimer = 2.0f;			//制限時間
	float m_rotY = 0.0f;							//アイテム自体の回転角度(Y軸)
	float m_shakeTimer = 0.0f;						//揺れを計算するためのタイマー
};

