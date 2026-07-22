#include "stdafx.h"
#include "Item.h"
#include <random>
#include "Game.h"
#include "Player.h"
#include "Circle.h"
#include "SEManager.h"
#include "Umbrella.h"
#include "QTEButton.h"
#include "HID/GamePadVibration.h"
Item::Item()
{
	//起動時にポインタをnullptrで安全に初期化
	m_modelRender = nullptr;
	m_failureModelRender = nullptr;
	m_qteButton = nullptr;
}

Item::~Item()
{
	if (m_modelRender != nullptr)
	{
		delete m_modelRender;
		m_modelRender = nullptr;
	}

	if (m_failureModelRender != nullptr)
	{
		delete m_failureModelRender;
		m_failureModelRender = nullptr;
	}

	if (m_qteButton)
	{
		DeleteGO(m_qteButton);
	}
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
	m_umbrella = FindGO<Umbrella>("umbrella");

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

void Item::SetActive(bool isActive)
{
	m_isActive = isActive;
}

void Item::Init(ItemType type)
{
	if(m_qteButton!=nullptr)
	{
		DeleteGO(m_qteButton);
		m_qteButton = nullptr;
	}
	//次のアイテムへの切り替えに伴う古いリソースの解放
	//Initが何度も呼び出されるため、newする前に古いモデルのメモリを確実に解放しDirectXのメモリ領域のリークや二重確保によるクラッシュを防ぐ
	if (m_modelRender != nullptr)
	{
		delete m_modelRender;
		m_modelRender = nullptr;
	}

	if (m_failureModelRender != nullptr)
	{
		delete m_failureModelRender;
		m_failureModelRender = nullptr;
	}

	m_isModelInited = true;
	m_isFailureModelInited = false;

	m_type = type;

	m_isProcessed = false;
	m_wasOnUmbrella = false;
	m_isCracked = false;
	m_hasPlayedLandSE = false;
	m_onUmbrellaTimer = 0.0f;

	m_modelRender = new ModelRender();
	m_failureModelRender = nullptr;

	switch (m_type)
	{
	case ItemType::ball:
		m_category = ItemCategory::Normal;
		m_hasFailureModel = false;
		m_modelRender->Init("Assets/modelData/ball.tkm");
		m_modelRender->SetScale({ 2.0f,2.0f,2.0f });
		m_isModelInited = true;
		if (!m_player) m_player = FindGO<Player>("player");
		if (m_player) {
			m_player->SetSpinCountSuccess(20);
		}
		break;
	case ItemType::egg:
		m_category = ItemCategory::QTE;
		m_hasFailureModel = true;
		m_modelRender->Init("Assets/modelData/egg.tkm");
		m_modelRender->SetScale({ 5.0f,5.0f,5.0f });

		m_myQTEPattern = { ButtonType::A,ButtonType::B,ButtonType::X };

		m_isModelInited = true;

		m_failureModelRender = new ModelRender();

		m_failureModelRender->Init("Assets/modelData/eggCracked.tkm");
		m_failureModelRender->SetScale({ 5.0f,5.0f,5.0f });
		m_isFailureModelInited = true;
		if (!m_player) m_player = FindGO<Player>("player");
		if (m_player) {
			m_player->SetSpinCountSuccess(25);
		}
		break;
	case ItemType::box:
		m_category = ItemCategory::Normal;
		m_hasFailureModel = false;
		m_modelRender->Init("Assets/modelData/Lift.tkm");
		m_modelRender->SetScale({ 0.75f,0.75f,0.75f });
		m_isModelInited = true;
		if (!m_player) m_player = FindGO<Player>("player");
		if (m_player) {
			m_player->SetSpinCountSuccess(29);
		}
		break;
	case ItemType::skeleton:
		m_category = ItemCategory::QTE;
		m_hasFailureModel = true;
		m_modelRender->Init("Assets/modelData/Skeleton.tkm");
		m_modelRender->SetScale({ 2.0f,2.0f,2.0f });

		m_myQTEPattern = { ButtonType::B,ButtonType::B,ButtonType::A,ButtonType::Y };

		m_isModelInited = true;

		m_failureModelRender = new ModelRender();

		m_failureModelRender->Init("Assets/modelData/SkeletonBroken.tkm");
		m_failureModelRender->SetScale({ 2.0f,2.0f,2.0f });
		m_isFailureModelInited = true;
		if (!m_player) m_player = FindGO<Player>("player");
		if (m_player) {
			m_player->SetSpinCountSuccess(33);
		}
		break;
	case ItemType::teaBowl:
		m_category = ItemCategory::Normal;
		m_hasFailureModel = true;
		m_modelRender->Init("Assets/modelData/TeaBowl.tkm");
		m_modelRender->SetScale({ 2.5f,2.5f,2.5f });
		m_isModelInited = true;

		m_failureModelRender = new ModelRender();

		m_failureModelRender->Init("Assets/modelData/TeaBowlBroken.tkm");
		m_failureModelRender->SetScale({ 2.5f,2.5f,2.5f });
		m_isFailureModelInited = true;
		if (!m_player) m_player = FindGO<Player>("player");
		if (m_player) {
			m_player->SetSpinCountSuccess(36);
		}
		break;
	case ItemType::penguin:
		m_category = ItemCategory::QTE;
		m_hasFailureModel = false;

		m_penguinAnimationState[enPenguinAnimation_Run].Load("Assets/animData/PenguinRun.tka");
		m_penguinAnimationState[enPenguinAnimation_Run].SetLoopFlag(true);

		m_penguinAnimationState[enPenguinAnimation_Anger].Load("Assets/animData/PenguinAnger.tka");
		m_penguinAnimationState[enPenguinAnimation_Anger].SetLoopFlag(true);

		m_modelRender->Init("Assets/modelData/Penguin.tkm", m_penguinAnimationState, enPenguinAnimation_Num);

		m_modelRender->PlayAnimation(enPenguinAnimation_Run);
		m_modelRender->SetScale({ 0.75f,0.75f,0.75f });

		m_myQTEPattern = { ButtonType::X,ButtonType::A,ButtonType::X,ButtonType::X,ButtonType::Y,ButtonType::B };

		m_isModelInited = true;
		if (!m_player) m_player = FindGO<Player>("player");
		if (m_player) {
			m_player->SetSpinCountSuccess(39);
		}
		break;
	case ItemType::phone:
		m_category = ItemCategory::Normal;
		m_hasFailureModel = true;
		m_modelRender->Init("Assets/modelData/Phone.tkm");
		m_modelRender->SetScale({ 2.5f,2.5f,2.5f });
		m_isModelInited = true;

		m_failureModelRender = new ModelRender();

		m_failureModelRender->Init("Assets/modelData/PhoneBroken.tkm");
		m_failureModelRender->SetScale({ 2.5f,2.5f,2.5f });
		m_isFailureModelInited = true;
		if (!m_player) m_player = FindGO<Player>("player");
		if (m_player) {
			m_player->SetSpinCountSuccess(42);
		}
		break;
	default:
		m_category = ItemCategory::Normal;
		m_hasFailureModel = false;

		m_modelRender->Init("Assets/modelData/ball.tkm");
		m_modelRender->SetScale({ 2.0f,2.0f,2.0f });

		m_isModelInited = true;
		break;
	}
}

//更新処理
void Item::Update()
{
	// QTEフェーズかつボタンが存在する場合
	if (m_state == BallState::Spinning && m_qteButton != nullptr)
	{
		m_qteButton->Update(); // ここでカウントダウンと入力を更新

		if (m_umbrella)
		{
			Vector3 position = m_umbrella->GetVisualPosition();
			Quaternion umbrellaRot = m_umbrella->GetRotation();

			float offsetDistance = 85.0f;

			if (m_type == ItemType::penguin)
			{
				offsetDistance = 85.0f;
			}
			position.y += offsetDistance;
			m_position = position;

			if (m_type == ItemType::penguin && m_modelRender)
			{
				Vector3 umbrellaUp = Vector3::Up;
				umbrellaRot.Apply(umbrellaUp);
				umbrellaUp.Normalize();

				Quaternion tiltRot;
				tiltRot.SetRotation(Vector3::Up, umbrellaUp);
				m_modelRender->SetRotation(tiltRot);
			}
			else if (m_modelRender)
			{
				m_rotX += 8.0f;
				Quaternion rotation;
				rotation.SetRotationDegX(m_rotX);

				if (m_modelRender)
				{
					m_modelRender->SetRotation(rotation);
				}
			}
		}
		if (m_qteButton->IsFinished())
		{
			// ★ここを追加
			if (m_player)
			{
				m_player->GetUmbrella()->SetAutoSpin(false);
			}

			if (m_qteButton->IsSuccess()) {
				m_state = BallState::SuccessThrow;
				// Gameクラスに成功を通知
				m_game->RequestQTESuccess();
			}
			else {
				m_game->RequestFailureLetter();
				m_player->m_playerError++;
				SpinningFailed();
			}

			// ボタンを削除して無効化
			DeleteGO(m_qteButton);
			m_qteButton = nullptr;
			return;
		}
	}

	//無効状態なら何もしない
	if (!m_isActive)return;

	//モデルが準備できていなければ処理しない
	if (!m_isModelInited)return;

	//Gameオブジェクトを取得(未取得なら探す)
	if (!m_game)
	{
		m_game = FindGO<Game>("game");
		if (!m_game)return;
	}
	//ゲームがプレイ中じゃなければ停止
	if (m_game->GetState() != GameState::Playing)return;

	static BallState lastState = BallState::Idle;
	if (m_state != lastState)
	{
		lastState = m_state;
	}

	switch (m_state)
	{
		//待機中は何もしない
	case BallState::Idle:
		break;
	case BallState::Flying:
		//空中を飛んでいる→放物運動
		ParabolicMotion();
		break;
	case BallState::OnUmbrella:
		//プレイヤーの傘の上に載っている状態
		OnUmbrella();
		break;
	case BallState::Spinning:
		if (m_qteButton == nullptr) {
			StartQTE();
		}
		break;
	case BallState::DropPrepare:
		//傘の端へ移動して落とされる準備
		DropPrepare();
		break;
	case BallState::FailFall:
		//ミスして落下中
		FailFallMotion();
		break;
	case BallState::SuccessThrow:
		//成功
		ParabolicMotion();
		break;
	}

	Vector3 renderPosition = m_position;
	if (m_state == BallState::Flying)
	{
		const float visualOffsetZ = 100.0f;
		renderPosition.z += visualOffsetZ;
	}

	if (m_isCracked && m_hasFailureModel && m_isFailureModelInited)
	{
		//卵が割れたモデル表示
		m_failureModelRender->SetPosition(renderPosition);
		m_failureModelRender->Update();
	}
	else if (m_isModelInited)
	{
		//通常モデル表示
		m_modelRender->SetPosition(renderPosition);
		m_modelRender->Update();
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

		if (m_position.y <= 130.0f && m_state != BallState::SuccessThrow && m_moveSpeed.y < 0.0f)
		{
			if (m_game)
			{
				m_player = FindGO<Player>("player");
				m_circle = m_game->GetCircle();
			}

			//傘に乗ったか判定
			if (m_player && m_circle)
			{
				bool success = IsInsideCircle(
					m_player->GetPosition(),
					m_circle->GetPosition(),
					m_circle->GetRadius());

				//成功
				if (success && !m_isProcessed)
				{
					m_wasOnUmbrella = true;
					m_player->m_itemOnUmbrella = true;
					m_state = BallState::OnUmbrella;
					return;
				}
			}
		}
		//地面に付いたらリセット
		if (m_position.y <= 0.0f)
		{
			//成功して飛んだあとの着地処理
			if (m_state == BallState::SuccessThrow)
			{
				if (m_isQTEFinished)
				{
					m_game->RequestQTESuccess();
					m_isQTEFinished = false; // フラグをリセット
				}

				m_position.y = 0.0f;			//地面に位置を固定

				m_moveSpeed = Vector3::Zero;	//移動速度を完全に停止

				m_isFlying = false;				//飛行状態を終了

				m_rotX = 0.0f;

				m_shakeTimer = 0.0f;

				if (m_modelRender)
				{
					m_modelRender->SetRotation(Quaternion::Identity);
				}

				m_state = BallState::Idle;		//状態を待機中に戻す
			}
			//通常の落下(最初の登場時と傘から落ちたとき)の処理
			else
			{
				m_moveSpeed = { 0.0f,0.0f,0.0f };
				m_isFlying = false;
				//失敗
				m_state = BallState::FailFall;
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
		m_rotX = 0.0f;
		m_shakeTimer = 0.0f;

		if (m_modelRender)
		{
			m_modelRender->SetRotation(Quaternion::Identity);
		}

		//地面に付いたらプレイヤーの方向を向き怒る
		if (m_type == ItemType::penguin && m_modelRender)
		{
			//アニメーション切り替え
			m_modelRender->PlayAnimation(enPenguinAnimation_Anger);

			//プレイヤーが存在することを確認して向きを計算
			if (m_player)
			{
				//ペンギンからプレイヤーへの方向ベクトルを計算
				Vector3 targetDir = m_player->GetPosition() - m_position;

				//高さの差を無視して、地面での向きにする
				targetDir.y = 0.0f;

				//ベクトルを正規化
				targetDir.Normalize();

				//方向ベクトルから回転クォータニオンを作成する関数
				Quaternion lookAtRotation;
				lookAtRotation.SetRotation({ 0.0f,0.0f,1.0f }, targetDir);

				//計算した向きをペンギンモデルに適用
				m_modelRender->SetRotation(lookAtRotation);
			}
		}

		//SE再生(一回だけ)
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

			case ItemType::box:
				SEManager::Play(SE_masu, false);
				break;

			case ItemType::skeleton:
				SEManager::Play(SE_bone, false);
				break;

			case ItemType::teaBowl:
				SEManager::Play(SE_teaBowl, false);
				break;

			case ItemType::penguin:
				SEManager::Play(SE_penguin, false);
				break;

			case ItemType::phone:
				SEManager::Play(SE_smartphone, false);
				break;

			default:
				break;
			}
			m_hasPlayedLandSE = true;
		}

		//ゲームに失敗通知
		if (!m_isProcessed)
		{
			if (m_game)
			{
				m_game->RequestFailureLetter();
				m_player->m_playerError++;
			}
		}
		//状態リセット
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
	std::uniform_real_distribution<float>distX(-700.0f, 700.0f);
	//std::uniform_real_distribution<float>distY(0.0f, 10.0f);
	//std::uniform_real_distribution<float>distZ(-400.0f, 200.0f);

	//ランダムに生成した値をそれぞれの座標に代入
	m_position.x = distX(mt);
	m_position.y = 10.0f;
	m_position.z = -400.0f;
}

void Item::OnUmbrella()
{
	if (!m_player)
	{
		m_player = FindGO<Player>("player");
	}

	if (!m_player)return;

	if (!m_circle)return;

	if (!m_umbrella)
	{
		m_umbrella = FindGO<Umbrella>("umbrella");
	}

	if (m_umbrella == nullptr)
	{
		return;
	}

	switch (m_type)
	{
	case ItemType::ball:
		m_umbrella->SetShakePower(0.4f);
		break;

	case ItemType::egg:
		m_umbrella->SetShakePower(0.4f);
		break;

	case ItemType::box:
		m_umbrella->SetShakePower(0.5f);
		break;

	case ItemType::skeleton:
		m_umbrella->SetShakePower(0.5f);
		break;

	case ItemType::teaBowl:
		m_umbrella->SetShakePower(0.6f);
		break;

	case ItemType::penguin:
		m_umbrella->SetShakePower(0.6f);
		break;

	case ItemType::phone:
		m_umbrella->SetShakePower(0.7f);
		break;
	}

	//プレイヤーの頭の上に固定
	Vector3 position = m_umbrella->GetVisualPosition();

	Quaternion umbrellaRot = m_umbrella->GetRotation();

	/*Vector3 umbrellaUp = Vector3::Up;
	umbrellaRot.Apply(umbrellaUp);
	umbrellaUp.Normalize();*/

	float offsetDistance = 85.0f;

	if (m_type == ItemType::penguin)
	{
		offsetDistance = 75.0f;
	}
	position.y += offsetDistance;

	//アイテムのタイプによって制御を切り分ける
	if (m_type != ItemType::penguin)
	{
		//ペンギン以外のアイテムの処理
		//傘の上で回転したり、小刻みに揺れる挙動
		m_rotX += 8.0f;
		m_shakeTimer += 0.5f;

		float shakeIntensity = 1.0f;
		float shakeX = sinf(m_shakeTimer * 1.3f) * shakeIntensity;
		float shakeY = cosf(m_shakeTimer * 1.7f) * shakeIntensity;

		Vector3 shakeOffset = Vector3(shakeX, shakeY, 0.0f);
		umbrellaRot.Apply(shakeOffset);
		position += shakeOffset;

		m_position = position;

		//ペンギン以外のアイテムは、回転させる
		Quaternion rotation;
		rotation.SetRotationDegX(m_rotX);

		if (m_modelRender)
		{
			m_modelRender->SetRotation(rotation);
		}
	}
	else
	{
		//ペンギンの処理
		//コード側で回転や揺れを上書きすると3Dアニメーションとバッティングするため、位置の追従だけを行い、回転や見た目の動きはすべてアニメーション側に委ねる
		m_position = position;

		if (m_modelRender)
		{
			Vector3 umbrellaUp = Vector3::Up;
			umbrellaRot.Apply(umbrellaUp);
			umbrellaUp.Normalize();

			Quaternion tiltRot;
			tiltRot.SetRotation(Vector3::Up, umbrellaUp);

			m_modelRender->SetRotation(tiltRot);

			if (m_state == BallState::OnUmbrella)
			{
				m_modelRender->PlayAnimation(enPenguinAnimation_Run);
			}
		}

	}

	m_position = position;

	// 傘の上にいる時間を加算
	m_onUmbrellaTimer += 1.0f / 60.0f; // 60FPS想定

	// 成功
	if (m_player->m_playerState == 4 && m_state != BallState::SuccessThrow)
	{
		// 傘回しが一定時間終わっていなければQTEや成功に遷移しない
		if (m_onUmbrellaTimer < m_onUmbrellaLimitTimer)return;

		// QTEありアイテムかどうかで状態を分ける
		if (m_type == ItemType::egg || m_type == ItemType::skeleton || m_type == ItemType::penguin)
		{
			m_state = BallState::Spinning; // QTEへ移行
		}
		else
		{
			m_state = BallState::SuccessThrow; // 通常演出用のステート
			m_game->RequestNormalSuccess();     // 通常演出の文字表示
		}

		m_isFlying = true;
		m_isProcessed = true;
		m_moveSpeed = { 0.0f, 10.0f, 15.0f };
		m_onUmbrellaTimer = 0.0f; // タイマーリセット
		return;
	}

	//失敗
	if (m_player->m_playerState == 1 && m_state != BallState::DropPrepare)
	{
		SpinningFailed();
		return;
	}
}

//QTE開始処理
void Item::StartQTE()
{
	if (m_qteButton != nullptr) return;

	// ★QTE中は傘を自動回転
	if (m_player)
	{
		m_player->GetUmbrella()->SetAutoSpin(true);
	}


	m_qteButton = NewGO<QTEButton>(0);

	m_qteButton->StartQTE(m_myQTEPattern, 5.0f);
	m_qteButton->SetPosition({ 0.0f, -300.0f, 0.0f });

	//m_qteButton->Update();

	if (m_qteButton->IsFinished())
	{
		if (m_qteButton->IsSuccess())
		{
			m_state = BallState::SuccessThrow;
			m_moveSpeed = { 0.0f, 10.0f, 15.0f }; // 成功時の発射

			m_isQTEFinished = true;
		}
		else
		{
			m_game->RequestFailureLetter();
			m_state = BallState::DropPrepare; // 失敗して落下

		}
		DeleteGO(m_qteButton);
		m_qteButton = nullptr;
	}
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
	m_isCracked = false;
	m_hasPlayedLandSE = false;
	m_rotX = 0.0f;
	m_shakeTimer = 0.0f;
	m_onUmbrellaTimer = 0.0f;

	static std::random_device rd;
	static std::mt19937 mt(rd());

	std::uniform_real_distribution<float>targetDistZ(400.0f, 800.0f);
	float targetZ = targetDistZ(mt);

	float vy = 15.0f;
	float g = m_gravity.y;

	float discriminant = vy * -2.0f * g * m_position.y;
	float t = (-vy - sqrtf(discriminant)) / g;

	float vz = (targetZ - m_position.z) / t;

	m_plannedVelocity = Vector3(0.0f, vy, vz);

	//m_plannedVelocity = Vector3(0.0f, 15.0f, 10.0f);
	m_isFlying = false;
	m_state = BallState::Idle;

	if (m_type == ItemType::penguin)
	{
		m_modelRender->PlayAnimation(enPenguinAnimation_Run);
	}
}

void Item::ResetItem()
{
	if (m_qteButton != nullptr)
	{
		DeleteGO(m_qteButton);
		m_qteButton = nullptr;
	}

	m_state = BallState::Idle;
	m_isFlying = false;
	m_wasOnUmbrella = false;
	m_isCracked = false;
	m_onUmbrellaLimitTimer = 0.0f;
	m_isActive = false;
}

void Item::Render(RenderContext& rc)
{
	if (!m_isActive)return;

	if (m_isCracked && m_hasFailureModel && m_isFailureModelInited)
	{
		m_failureModelRender->Draw(rc);
	}
	else if (m_isModelInited)
	{
		m_modelRender->Draw(rc);
	}
}