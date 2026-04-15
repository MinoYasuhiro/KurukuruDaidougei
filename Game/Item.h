#pragma once
#include "BallState.h"

class Item:public IGameObject
{
public:
	Item();
	~Item();
	bool Start()override;
	void Update()override;
	void Render(RenderContext&renderContext)override;

	void SetState(BallState newState);	//状態切り替え
	void Move();
	void RandomSpawn();
	void ParabolicMotion();
	void RequestFire();		//プレイヤーが成功したら呼ぶ
	void OnBallLanded();	//着地時に呼ばれるイベント
	void OnUmbrella();		//傘に乗った瞬間の処理
	void StartQTE();		//QTE開始
	void SpinningFailed();	//傘回し失敗時
	void FailFallMotion();

	void StartFallTest();
	void StartParabolaTest();

private:
	BallState m_state = BallState::Idle;
	//ModelRender m_modelRender;
	ModelRender m_eggRender;
	ModelRender m_eggCrackedRender;
	Vector3 m_position = Vector3::Zero;
	Vector3 m_moveSpeed = Vector3::Zero;
	Vector3 m_gravity = { 0.0f,-0.5f,0.0f };
	bool m_isFlying = false;	//球が飛んでいるかどうか
	bool m_isCracked = false;
};

