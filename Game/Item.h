#pragma once
#include "BallState.h"

class Game;
class Player;
class Circle;
class Item :public IGameObject
{
public:
	Item();
	~Item();
	bool Start()override;
	void Update()override;
	void Render(RenderContext& renderContext)override;

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
	void DropPrepare();

	void StartFallTest();
	void StartParabolaTest();
	void PrepareParabola();

	//現在の速度から予測される着地点を取得
	Vector3 GetPlannedLandingPosition()const;

	//現在の状態での着地点を取得
	Vector3 GetLandingPosition()const;

	//飛行中かどうかを取得
	bool IsFlying()const { return m_isFlying; }

private:

	Game* m_game = nullptr;
	Player* m_player = nullptr;
	Circle* m_circle = nullptr;

	BallState m_state = BallState::Idle;	//アイテムの状態
	ModelRender m_modelRender;				//ボールモデル
	//ModelRender m_eggRender;				//卵モデル
	//ModelRender m_eggCrackedRender;		//割れた卵モデル
	Vector3 m_position = Vector3::Zero;		//現在位置
	Vector3 m_moveSpeed = Vector3::Zero;	//現在の移動速度
	Vector3 m_gravity = { 0.0f,-0.5f,0.0f };//重力加速度
	Vector3 m_plannedVelocity;				//発射予定の初速
	bool m_isFlying = false;	//球が飛んでいるかどうか
	bool m_isCracked = false;	//割れ状態かどうか
	bool m_wasOnUmbrella = false;
	bool m_hasPlayedLandSE = false;
	float m_onUmbrellaTimer = 0.0f;
	float m_onUmbrellaLimitTimer = 2.0f;
};

