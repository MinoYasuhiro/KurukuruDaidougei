#include "stdafx.h"
#include "Item.h"
#include <random>
#include "Game.h"
#include "Player.h"
#include "Circle.h"
#include "SEManager.h"
Item::Item()
{

}

Item::~Item()
{

}

namespace
{
	bool IsInsideCircle(const Vector3& playerPosition, const Vector3& circlePosition, float radius)
	{
		//プレイヤー位置と円の中心位置の差を計算(距離ベクトル)
		Vector3 diff = playerPosition - circlePosition;
		//高さ方向は無視して、XZ平面のみで判定する
		diff.y = 0.0f;

		//ベクトルの長さを求めて、半径以内かどうかを判定
		return diff.Length() <= radius;
	}
}

//初期化
bool Item::Start()
{
	RandomSpawn();	//初期位置をランダムにする

	m_game = FindGO<Game>("game");
	m_player = FindGO<Player>("player");

	if (m_game)
	{
		m_circle = m_game->GetCircle();
	}
	else
	{
		m_circle = nullptr;
	}

	return true;
}

void Item::Init(ItemType type)
{
	m_type = type;

	m_isProcessed = false;
	m_wasOnUmbrella = false;
	m_isCracked = false;
	m_hasPlayedLandSE = false;

	switch (m_type)
	{
	case ItemType::ball:
		m_category = ItemCategory::Normal;
		m_hasFailureModel = false;
		m_modelRender.Init("Assets/modelData/ball.tkm");
		m_modelRender.SetScale({ 2.0f,2.0f,2.0f });
		break;
	case ItemType::egg:
		m_category = ItemCategory::QTE;
		m_hasFailureModel = true;
		m_modelRender.Init("Assets/modelData/egg.tkm");
		m_modelRender.SetScale({ 5.0f,5.0f,5.0f });

		m_failureModelRender.Init("Assets/modelData/eggCracked.tkm");
		m_failureModelRender.SetScale({ 5.0f,5.0f,5.0f });
		break;
	default:
		m_category = ItemCategory::Normal;
		m_hasFailureModel = false;
		break;
	}
}

//更新処理
void Item::Update()
{
	if (!m_game)
	{
		m_game = FindGO<Game>("game");
		if (!m_game)return;
	}
	if (m_game->GetState() != GameState::Playing)return;
	switch (m_state)
	{
		//待機中は何もしない
	case BallState::Idle:
		break;
	case BallState::Flying:
		ParabolicMotion();
		break;
	case BallState::OnUmbrella:
		OnUmbrella();
		break;
	case BallState::Spinning:
		StartQTE();
		break;
	case BallState::DropPrepare:
		DropPrepare();
		break;
	case BallState::FailFall:
		FailFallMotion();
		break;
	case BallState::SuccessThrow:
		ParabolicMotion();
		break;
	}

	if (m_isCracked && m_hasFailureModel)
	{
		m_failureModelRender.SetPosition(m_position);
		m_failureModelRender.Update();
	}
	else
	{
		m_modelRender.SetPosition(m_position);
		m_modelRender.Update();
	}
}

//球の放物運動（重力による落下を含む）を処理する関数
void Item::ParabolicMotion()
{
	if (m_isFlying)
	{
		//重力を速度に加算
		m_moveSpeed += m_gravity;
		//位置を更新
		m_position += m_moveSpeed;

		//地面に付いたらリセット
		if (m_position.y <= 0.0f)
		{
			m_moveSpeed = { 0.0f,0.0f,0.0f };
			m_isFlying = false;

			if (m_game)
			{
				m_player = FindGO<Player>("player");
				m_circle = m_game->GetCircle();

				if (m_player && m_circle)
				{
					bool success = IsInsideCircle(
						m_player->GetPosition(),
						m_circle->GetPosition(),
						m_circle->GetRadius());

					if (success && !m_isProcessed)
					{
						m_wasOnUmbrella = true;
						m_player->m_itemOnUmbrella = true;
						m_state = BallState::OnUmbrella;
						return;
					}
				}
				// ★ここを置き換え
				if (!m_isProcessed)
				{
					// 傘に乗せ失敗
					if (m_player)
					{
						m_player->m_playerState = 1;
						m_player->m_playerError++;
					}

					m_state = BallState::FailFall;
				}
				else
				{
					// 成功後の弾は終了
					m_state = BallState::Idle;
				}
			}
		}
	}
}

//球の状態を外部から変更するための関数
void Item::SetState(BallState newState)
{
	m_state = newState;
}

//落下処理
void Item::FailFallMotion()
{
	//落下処理
	m_moveSpeed.y -= 0.3f;
	m_position += m_moveSpeed;

	//地面に付いたら停止(卵は破損状態に)する
	if (m_position.y <= 0.0f)
	{
		m_position.y = 0.0f;
		m_moveSpeed = Vector3::Zero;
		m_isFlying = false;
		m_isCracked = true;

		if (!m_hasPlayedLandSE)
		{
			switch (m_type)
			{
			case ItemType::ball:
				SEManager::Play(SE_ball, false);
				break;

			case ItemType::egg:
				SEManager::Play(SE_crackedEgg, false);
				break;

			default:
				break;
			}
			//SEManager::Play(SE_crackedEgg);
			m_hasPlayedLandSE = true;
		}

		if (!m_isProcessed)
		{
			if (m_game)
			{
				m_game->RequestFailureLetter();
			}
		}
		m_state = BallState::Idle;
	}
}

//外部からの球を飛ばす要求を処理する関数
void Item::RequestFire()
{
	if (!m_isFlying)
	{
		Move();
	}
}

//球を飛ばすための初速を設定し、飛行状態に移行させる関数
void Item::Move()
{
	m_isFlying = true; //球が飛ぶフラグON
	m_moveSpeed = m_plannedVelocity;
	m_state = BallState::Flying;
}

//球が地面に着地したときに呼ばれる処理
void Item::OnBallLanded()
{
	m_isFlying = false;
	m_moveSpeed = { 0.0f,0.0f,0.0f };

	m_state = BallState::Idle;
	//Move();	//動作確認用
}

//球の位置をランダムな座標に設定する
void Item::RandomSpawn()
{
	//一度だけ初期化される乱数デバイス(シード生成用)
	static std::random_device rd;
	//乱数生成期を初期化
	static std::mt19937 mt(rd());

	//座標をランダム生成
	std::uniform_real_distribution<float>distX(-100.0f, 100.0f);
	std::uniform_real_distribution<float>distY(0.0f, 10.0f);
	std::uniform_real_distribution<float>distZ(-50.0f, 50.0f);

	//ランダムに生成した値をそれぞれの座標に代入
	m_position.x = distX(mt);
	m_position.y = distY(mt);
	m_position.z = distZ(mt);
}

//傘の上に乗ったときの処理
void Item::OnUmbrella()
{
	if (!m_player)
	{
		m_player = FindGO<Player>("player");
	}

	if (!m_player)return;

	Vector3 position = m_player->GetPosition();
	position.y += 130.0f;
	m_position = position;

	if (m_player->m_playerState == 4 && m_state != BallState::SuccessThrow)
	{
		m_state = BallState::SuccessThrow;
		m_isFlying = true;
		m_isProcessed = true;
		m_moveSpeed = { 0.0f,10.0f,15.0f };

		if (m_game)
		{
			m_game->RequestSuccessLetter();
		}
		return;
	}

	if (m_player->m_playerState == 1 && m_state != BallState::DropPrepare)
	{
		SpinningFailed();
		return;
	}
}

//QTE開始処理
void Item::StartQTE()
{

}

//傘回し失敗時の処理
void Item::SpinningFailed()
{
	m_state = BallState::DropPrepare;
	m_player->m_playerState = 1;
}

void Item::DropPrepare()
{
	//傘判定用の円が取得できない場合は処理しない
	if (!m_circle)return;

	//円の中心位置
	Vector3 center = m_circle->GetPosition();

	//円中心から現在位置への方向ベクトル
	Vector3 diff = m_position - center;
	diff.y = 0.0f;

	//中心からの距離
	float dist = diff.Length();
	//円の半径
	float radius = m_circle->GetRadius();

	//円(開いた傘)の中心付近にいる場合、外側へ少しずつ移動させる
	if (dist > 0.01f)
	{
		Vector3 dir = diff;
		dir.Normalize();
		m_position += dir * 1.0f;
	}

	//円(開いた傘)の外に出たら落下処理へ移行
	if (dist >= radius)
	{
		Vector3 dir = diff;
		dir.Normalize();

		//外向き+下向きの初速を設定
		m_moveSpeed = dir * 2.0f;
		m_moveSpeed.y = -1.0f;

		m_isFlying = true;
		m_state = BallState::FailFall;
	}
}

//現在の速度から地面に到達する位置を掲載
Vector3 Item::GetLandingPosition()const
{
	//初期の高さ
	float y0 = m_position.y;
	//初速
	float vy = m_moveSpeed.y;
	//重力加速度
	float g = m_gravity.y;

	//二次方程式の判別式
	float discriminant = vy * vy - 2.0f * g * y0;

	//解が存在しない場合は現在位置を返す
	if (discriminant < 0.0f)
	{
		return m_position;
	}

	//地面に到達するまでの時間
	float t = (-vy - sqrtf(discriminant)) / g;

	//着地点を計算
	Vector3 landingPosition = m_position;
	landingPosition.x += m_moveSpeed.x * t;
	landingPosition.z += m_moveSpeed.z * t;
	landingPosition.y = 0.0f;

	return landingPosition;
}

//発射した場合の着地点を事前に予測
Vector3 Item::GetPlannedLandingPosition()const
{
	float y0 = m_position.y;
	float vy = m_plannedVelocity.y;
	float g = m_gravity.y;

	float discriminant = vy * vy - 2.0f * g * y0;

	if (discriminant < 0.0f)
	{
		return m_position;
	}

	float t = (-vy - sqrtf(discriminant)) / g;

	Vector3 landingPosition = m_position;
	landingPosition.x += m_plannedVelocity.x * t;
	landingPosition.z += m_plannedVelocity.z * t;
	landingPosition.y = 0.0f;

	return landingPosition;
}

//放物運動を行うための事前準備処理
void Item::PrepareParabola()
{
	RandomSpawn();

	m_isProcessed = false;
	m_wasOnUmbrella = false;

	m_plannedVelocity = Vector3(0.0f, 15.0f, 10.0f);
	m_isFlying = false;
	m_state = BallState::Idle;
}

void Item::Render(RenderContext& rc)
{
	if (m_isCracked && m_hasFailureModel)
	{
		m_failureModelRender.Draw(rc);
	}
	else
	{
		m_modelRender.Draw(rc);
	}
}