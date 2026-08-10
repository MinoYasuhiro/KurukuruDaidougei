#include "stdafx.h"
#include "Game.h"
#include "BackGround.h"
#include "Player.h"
#include "GameCamera.h"
#include "GameClear.h"
#include "GameOver.h"
#include "Pause.h" 
#include "BGM.h"
#include "BGMManager.h"
#include "QTEButton.h"
#include <random>
#include "Umbrella.h"
#include "CoinBox.h"
#include "SEManager.h"
#include "Item.h"
#include "ItemSpawner.h"
#include "Title.h"
#include "Circle.h"
#include "SoundSettings.h"
#include "CoinEffect.h"
#include "AudienceManager.h"
#include "Confetti.h"
#include "MissEffect.h"
#include "Arrow.h"
#include "WaterPuddle.h"

GamePhase Game::m_phase = GamePhase::Start;
GameState Game::m_gameState = GameState::Playing;
GameState Game::m_prevState = GameState::Title;

Game::Game()
{
}

Game::~Game()
{

}

bool Game::Start()
{
    SoundSettings::Load();
    SEManager::Init();

    //ゲームの初期化
    m_phase = GamePhase::Start;

    m_spawner = NewGO<ItemSpawner>(0, "itemSpawner");

    m_circle = NewGO<Circle>(0, "circle");

    m_coin = NewGO<CoinEffect>(0, "coinEffect");

    m_confetti = NewGO<Confetti>(0, "confetti");

    m_missEffect = NewGO<MissEffect>(0, "missEffect");

	//m_audienceManager = NewGO<AudienceManager>(0, "audienceManager");

    m_backGround = NewGO<BackGround>(0, "background");
    //m_umbrella = NewGO<Umbrella>(0, "umbrella");
    m_player = NewGO<Player>(0, "player");

    m_gameCamera = NewGO<GameCamera>(0, "gameCamera");

    m_waterPuddle1 = NewGO<WaterPuddle>(0);
    m_waterPuddle1->SetPosition({ -300.0f, 1.0f, 500.0f });
	m_waterPuddle1->m_model.SetScale({ 5.0f,5.0f,5.0f });

    m_waterPuddle2 = NewGO<WaterPuddle>(0);
    m_waterPuddle2->SetPosition({ 0.0f, 1.0f, 700.0f });
    m_waterPuddle2->m_model.SetScale({ 5.0f,5.0f,5.0f });
    m_waterPuddle3 = NewGO<WaterPuddle>(0);
    m_waterPuddle3->SetPosition({ 300.0f, 1.0f, 550.0f });
    m_waterPuddle3->m_model.SetScale({ 5.0f,5.0f,5.0f });

    m_skyCube = NewGO<SkyCube>(0, "skyCube");
    m_skyCube->SetType(enSkyCubeType_SunriseToon);

    m_startLetter.Init("Assets/sprite/Start.dds", 500.0f, 300.0f);

    m_failureLetter.Init("Assets/sprite/Failure.dds", 500.0f, 300.0f);

    m_successLetter.Init("Assets/sprite/Success.dds", 500.0f, 300.0f);

    m_AudienceSpriteRender.Init("Assets/modelData/kyaku.DDS", 1920.0f, 700.0f);
    m_AudienceSpriteRender1.Init("Assets/modelData/kyaku.sippai.DDS", 1920.0f, 700.0f);
    m_AudienceSpriteRender2.Init("Assets/modelData/kyaku.seikou.DDS", 1920.0f, 700.0f);


    m_AudienceSpriteRender.SetPosition({ 0.0f, -300.0f, 0.0f });
    m_AudienceSpriteRender1.SetPosition({ 0.0f, -300.0f, 0.0f });
    m_AudienceSpriteRender2.SetPosition({ 0.0f, -300.0f, 0.0f });

    m_AudienceSpriteRender.Update();
    m_AudienceSpriteRender1.Update();
    m_AudienceSpriteRender2.Update();

    m_sprite1.Init("Assets/modelData/okyaku1.DDS", 1920.0f, 700.0f);
    m_sprite2.Init("Assets/modelData/okyaku2.DDS", 1920.0f, 700.0f);
    m_sprite3.Init("Assets/modelData/okyaku3.DDS", 1920.0f, 700.0f);
    m_sprite4.Init("Assets/modelData/okyaku4.DDS", 1920.0f, 700.0f);
    m_sprite5.Init("Assets/modelData/okyaku5.DDS", 1920.0f, 700.0f);
    m_sprite6.Init("Assets/modelData/okyaku6.DDS", 1920.0f, 700.0f);
    m_sprite7.Init("Assets/modelData/okyaku7.DDS", 1920.0f, 700.0f);
    m_sprite8.Init("Assets/modelData/okyaku8.DDS", 1920.0f, 700.0f);

    m_basePos1 = { 0.0f, -300.0f, 0.0f };
    m_basePos2 = { 0.0f, -300.0f, 0.0f };
    m_basePos3 = { 0.0f, -300.0f, 0.0f };
    m_basePos4 = { 0.0f, -300.0f, 0.0f };
    m_basePos5 = { 0.0f, -300.0f, 0.0f };
    m_basePos6 = { 0.0f, -300.0f, 0.0f };
    m_basePos7 = { 0.0f, -300.0f, 0.0f };
    m_basePos8 = { 0.0f, -300.0f, 0.0f };

    m_sprite1.SetPosition(m_basePos1);
    m_sprite2.SetPosition(m_basePos2);
    m_sprite3.SetPosition(m_basePos3);
    m_sprite4.SetPosition(m_basePos4);
    m_sprite5.SetPosition(m_basePos5);
    m_sprite6.SetPosition(m_basePos6);
    m_sprite7.SetPosition(m_basePos7);
    m_sprite8.SetPosition(m_basePos8);

    m_sprite1.Update();
    m_sprite2.Update();
    m_sprite3.Update();
    m_sprite4.Update();
    m_sprite5.Update();
    m_sprite6.Update();
    m_sprite7.Update();
    m_sprite8.Update();

    m_count1.Init("Assets/sprite/count1.dds",125.0f,125.0f);
    m_count2.Init("Assets/sprite/count2.dds",125.0f,125.0f);
    m_count3.Init("Assets/sprite/count3.dds",125.0f,125.0f);
    m_countDown.Init("Assets/sprite/countDown.dds", 400.0f, 400.0f);

    m_count1.SetPosition({ 0.0f,375.0f,0.0f });
    m_count2.SetPosition({ 0.0f,375.0f,0.0f });
    m_count3.SetPosition({ 0.0f,375.0f,0.0f });
    m_countDown.SetPosition({ 0.0f,475.0f,0.0f });

    m_startTimer = 0.0f;
    m_showStart = true;

    m_failureTimer = 0.0f;
    m_showFailure = false;

    m_successTimer = 0.0f;
    m_showSuccess = false;

    //m_BGM = FindGO<BGMManager>("bgmManager");

    if (m_BGM == nullptr)
    {
        m_BGM = NewGO<BGMManager>(0, "bgmManager");
        m_BGM->Init();
    }
    
    else
    {
        m_BGM->Stop();
    }

    m_BGM->Play(BGM_NormalUmbrella);

    m_coinBox = NewGO<CoinBox>(0, "coinBox");
    
   // m_player = FindGO<Player>("player");

    return true;
}

void Game::Update()
{
    nsK2EngineLow::GamePad::BeginFrame();
    g_pad[0]->Update();

    if (m_isGameClearAnimation)
    {
        return;
    }

    // ★ 次フレーム開始処理
    if (m_requestStart)
    {
        m_requestStart = false;
        m_gameState = GameState::Playing;
        m_isResetting = false;
        return; // ← 今フレームは何も動かさない
    }
    switch (m_gameState)
    {
    case GameState::Playing:
        UpdatePlaying();   // ← 今の Playing 用ロジックをここに分離
        break;

    case GameState::GameClear:
        // GameClear が Update / Render するので Game は何もしない
        break;

    case GameState::Title:
        // Title が Update / Render するので Game は何もしない
        break;

    case GameState::Pause:
        break;

    case GameState::GameOver:
        break;
    }

    m_time += g_gameTime->GetFrameDeltaTime();

    if (m_player->m_playerState == 4 && !m_isSuccess)
    {
        m_isSuccess = true;
        m_successTimer2 = 3.0f;
    }

    if (m_isSuccess)
    {
        m_successTimer2 -= g_gameTime->GetFrameDeltaTime();

        if (m_successTimer2 <= 0.0f)
        {
            m_isSuccess = false;
            m_bigSuccess = false;   // ←追加
        }


    }

    float t = m_time;

    if (m_isSuccess && !m_bigSuccess)
    {
        Vector3 pos;

        pos = m_basePos1;
        pos.y += sinf(t * 10.0f + 0.0f) * 8.0f;
        m_sprite1.SetPosition(pos);

        pos = m_basePos2;
        pos.y += sinf(t * 10.0f + 0.3f) * 8.0f;
        m_sprite2.SetPosition(pos);

        pos = m_basePos3;
        pos.y += sinf(t * 10.0f + 0.6f) * 8.0f;
        m_sprite3.SetPosition(pos);

        pos = m_basePos4;
        pos.y += sinf(t * 10.0f + 0.9f) * 8.0f;
        m_sprite4.SetPosition(pos);

        pos = m_basePos5;
        pos.y += sinf(t * 10.0f + 1.2f) * 8.0f;
        m_sprite5.SetPosition(pos);

        pos = m_basePos6;
        pos.y += sinf(t * 10.0f + 1.5f) * 8.0f;
        m_sprite6.SetPosition(pos);

        pos = m_basePos7;
        pos.y += sinf(t * 10.0f + 1.8f) * 8.0f;
        m_sprite7.SetPosition(pos);

        pos = m_basePos8;
        pos.y += sinf(t * 10.0f + 2.1f) * 8.0f;
        m_sprite8.SetPosition(pos);
    }

    else if (m_isSuccess && m_bigSuccess)
    {
        Vector3 pos;

        pos = m_basePos1;
        pos.y += sinf(t * 14.0f + 0.0f) * 15.0f;
        m_sprite1.SetPosition(pos);

        pos = m_basePos2;
        pos.y += sinf(t * 14.0f + 0.3f) * 15.0f;
        m_sprite2.SetPosition(pos);

        pos = m_basePos3;
        pos.y += sinf(t * 14.0f + 0.6f) * 15.0f;
        m_sprite3.SetPosition(pos);

        pos = m_basePos4;
        pos.y += sinf(t * 14.0f + 0.9f) * 15.0f;
        m_sprite4.SetPosition(pos);

        pos = m_basePos5;
        pos.y += sinf(t * 14.0f + 1.2f) * 15.0f;
        m_sprite5.SetPosition(pos);

        pos = m_basePos6;
        pos.y += sinf(t * 14.0f + 1.5f) * 15.0f;
        m_sprite6.SetPosition(pos);

        pos = m_basePos7;
        pos.y += sinf(t * 14.0f + 1.8f) * 15.0f;
        m_sprite7.SetPosition(pos);

        pos = m_basePos8;
        pos.y += sinf(t * 14.0f + 2.1f) * 15.0f;
        m_sprite8.SetPosition(pos);
    }

    else
    {
        m_sprite1.SetPosition(m_basePos1);
        m_sprite2.SetPosition(m_basePos2);
        m_sprite3.SetPosition(m_basePos3);
        m_sprite4.SetPosition(m_basePos4);
        m_sprite5.SetPosition(m_basePos5);
        m_sprite6.SetPosition(m_basePos6);
        m_sprite7.SetPosition(m_basePos7);
        m_sprite8.SetPosition(m_basePos8);
    }

    m_sprite1.Update();
    m_sprite2.Update();
    m_sprite3.Update();
    m_sprite4.Update();
    m_sprite5.Update();
    m_sprite6.Update();
    m_sprite7.Update();
    m_sprite8.Update();


    if (m_circle!=nullptr)
    {
        m_circle->Update();
    }

    if (m_coin)
    {
        m_coin->Update();
    }

    m_count1.Update();
    m_count2.Update();
    m_count3.Update();
    m_countDown.Update();

    switch (m_player->m_playerState)
    {
    case 0:
    case 2:
    case 3:
        AudienceImage = 0;
        break;
    case 1:
    case 5:
    case 9:
    case 10:
    case 30:
        AudienceImage = 1;
        break;
    case 4:
    case 6:
    case 7:
    case 8:
        AudienceImage = 2;
        break;

    default:
        AudienceImage = 0;
        break;

    }
}

GamePhase Game::GetPhase()
{
    return m_phase;
}

void Game::ResetGame()
{
    // ★ プレイヤー初期化
    if (Player* player = FindGO<Player>("player"))
    {
        player->Reset();
    }

    if (GameCamera* cam = FindGO<GameCamera>("gameCamera"))
        cam->Reset();

    m_BGM = FindGO<BGMManager>("bgmManager");

    if (m_BGM != nullptr)
    {
        m_BGM->Stop();
        m_BGM->Play(BGM_NormalUmbrella);
    }

    if (Arrow* arrow = FindGO<Arrow>("arrow"))
    {
        arrow->Reset();
    }

    if (Circle* circle = FindGO<Circle>("circle"))
    {
        circle->Reset();
    }

    if (CoinEffect* coinEffect = FindGO<CoinEffect>("coinEffect"))
    {
        coinEffect->Reset();
    }

    if (Confetti* confetti = FindGO<Confetti>("confetti"))
    {
        confetti->Reset();
    }

    if (MissEffect* missEffect = FindGO<MissEffect>("missEffect"))
    {
        missEffect->Reset();
    }

    m_failCount = 0;

    // Title からのみ許可
    if (m_gameState != GameState::Title &&
        m_gameState != GameState::GameClear && 
        m_gameState != GameState::GameOver&&
        m_gameState !=GameState::Pause)
        return;

    m_isResetting = true;
    SEManager::ClearCache();

    // 一旦停止状態にする
    m_gameState = GameState::Pause;
    
    m_phase = GamePhase::Start;
    m_phaseStep = 0;
    m_startTimer = 0.0f;
    m_showStart = true;
    m_movePhaseTimer = 0.0f;
    m_clearTimer = 0.0f;
    m_isGameClearShown = false;
    m_itemMove = false;

    m_qteStarted = false;
    m_button = nullptr;

    m_showSuccess = false;
    m_showFailure = false;

    m_successTimer = 0.0f;
    m_failureTimer = 0.0f;

    m_countTimer = 0.0f;
    m_isCounting = false;
    m_countNumber = 0.0f;

    m_isSuccess = false;
    m_bigSuccess = false;
    m_QTESuccess30 = false;

    m_isGameClearAnimation = false;

    m_count1.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    m_count2.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    m_count3.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));

    if (m_spawner)
    {
        Item* currentItem = m_spawner->GetCurrentItem();
        if (currentItem != nullptr)
        {
            currentItem->ResetItem();
            DeleteGO(currentItem);
        }
        m_spawner->Reset();
    }

    if (QTEButton* button = FindGO<QTEButton>("qteButton"))
    {
        DeleteGO(button);
    }

    if (m_coinBox)
    {
        m_coinBox->Reset();
    }

    m_phase = GamePhase::Start;

    m_requestStart = true;

    m_coinPlayed = false;
}

void Game::RequestMovePhase()
{
    m_phase = GamePhase::MovePhase;
    m_movePhaseTimer = 0.0f;
    m_itemMove = false;
}

void Game::UpdatePlaying()
{

    // ===== ポーズ呼び出し =====
    if (m_phase != GamePhase::QTEMove&& g_pad[0]->IsPress(enButtonStart) && m_gameState == GameState::Playing)
    {
        SEManager::StopLoop(SE_run);

        if (m_showFailure || m_showSuccess)
        {
            return;
        }

		SEManager::Play(SE_pause, false);

        NewGO<Pause>(1, "pause");
        m_gameState = GameState::Pause;
        return;                      // Game は消さない

    }

    // ===== ポーズ中は「更新だけ」止める =====
    if (m_gameState == GameState::Pause)
        return;



    // ===== 通常ゲーム更新 =====
    const float deltaTime = 1.0f / 60.0f;

    switch (m_phase)
    {
    case GamePhase::Start:
        m_startTimer += deltaTime;
        if (m_startTimer >= 3.0f)
        {
            m_showStart = false;
            RequestMovePhase();
        }
        break;

    case GamePhase::MovePhase:
    {
        m_movePhaseTimer += deltaTime;

        m_circle->SetVisible(false);

        //アイテム未生成時のみ生成
        if (!m_itemMove)
        {
            m_spawner->SpawnNext();

            m_itemMove = true;
            m_hasThrownItem = false;

            m_countTimer = 0.0f;
            m_isCounting = true;
            m_countNumber = 3;

            return;
        }

        //現在のアイテムを取得
        Item* current = m_spawner->GetCurrentItem();

        //アイテムが存在しない場合
        if (current == nullptr)
        {
            //表示委する必要がないので円を非表示にする
            m_circle->SetVisible(false);
            break;
        }
        else
        {
#ifdef _DEBUG
            //デバッグ用:ポインタの安全チェック
            assert(current != nullptr);
            assert(_CrtIsValidPointer(current, sizeof(Item), TRUE));
#endif // DEBUG

            //「移動フェーズ中かつまだ投げていない」場合のみ予測表示する
            if (m_phase == GamePhase::MovePhase && !m_hasThrownItem)
            {
                //アイテムがまだ飛んでいない
                if (!current->IsFlying())
                {
                    //着地点(これから飛ばしたときに落ちる位置)を計算取得
                    Vector3 landingPosition = current->GetPlannedLandingPosition();

                    landingPosition.y += 1.0f;

                    //予測円の位置を着地点に合わせる
                    m_circle->SetPosition(landingPosition);
                    //円を表示
                    m_circle->SetVisible(true);

                    if (Arrow* arrow = FindGO<Arrow>("arrow"))
                    {
                        if (m_player != nullptr)
                        {
                            arrow->SetDirection(m_player->GetPosition(), landingPosition);
                        }
                    }
                }
                else
                {
                    //すでに飛んでいるときは予測は不要なので非表示
                    m_circle->SetVisible(false);

                    if (Arrow* arrow = FindGO<Arrow>("arrow"))
                    {
                        arrow->SetActive(false);
                    }
                }
            }
        }

        if (m_movePhaseTimer >= 10.0f)
        {
            m_movePhaseTimer = 0.0f;

            m_phase = GamePhase::AfterMove;
        }
        break;
    }
    case GamePhase::AfterMove:
        ////////通常傘回し用の処理////////
        m_clearTimer += deltaTime;
        if (m_clearTimer >= 1.0f)
        {
            m_clearTimer = 0.0f;

            Item* item = m_spawner->GetCurrentItem();

            if (item == nullptr)
            {
                RequestMovePhase();
            }
            else
            {
                //アイテムの種類に応じて次のフェーズを決定
                //QTEが必要なアイテムならQTE専用フェーズへ、通常ならそのまま次の移動へ
                if (item->IsQTEItem())
                {
                    m_phase = GamePhase::QTEMove;
                }
                else
                {
                    RequestMovePhase();
                }
            }
        }
        break;

    case GamePhase::SpecialMove:
        //特殊傘回し用の処理
        m_clearTimer += deltaTime;

        if (m_clearTimer >= 1.0f)    // 秒数
        {
            m_clearTimer = 0.0f;
            m_phaseStep = 2;          // 次はQTE
         
            RequestMovePhase();
        }
        break;

    case GamePhase::QTEMove:
    {
        //QTE実行中の結果待ち及び演出待機フェーズ
        Item* item = m_spawner->GetCurrentItem();
        //アイテムがSpinning(QTE中)でなくなった=結果が出た後の待機処理
        if (item != nullptr && item->GetState() != BallState::Spinning)
        {
            m_movePhaseTimer += deltaTime;
            //結果表示後の演出時間を確保してから次の移動フェーズへ
            if (m_movePhaseTimer >= 1.0f) {
                RequestMovePhase();
            }
        }
    }
    break;
    }

    // ===== ここから追加（Game.cpp完結のGameOver判定）=====
    //if (m_gameState == GameState::Playing)
    //{
        //if (Player* player = FindGO<Player>("player"))
        //{
           // Vector3 pos = player->GetPosition();
           // const float moveThreshold = 0.05f; // 動いたとみなす距離

            //if ((pos - m_prevPos).Length() < moveThreshold)
            //{
                //m_idleTimer += 1.0f / 60.0f;
            //}
            //else
            //{
                //m_idleTimer = 0.0f;
                //m_prevPos = pos;
            ////}

            // 8秒何もしなかったらGameOver
            //if (m_idleTimer >= m_idleLimitTime)
            //{
                //RequestGameOver();
            //}
       // }
   // }

    //アイテムが飛んでいない間は着地点予測を表示
    //if (m_item && m_phase == GamePhase::MovePhase && !m_hasThrownItem && !m_item->IsFlying())
    //{
    //    Vector3 landingPosition = m_item->GetPlannedLandingPosition();
    //    m_circle->SetPosition(landingPosition);
    //    m_circle->SetVisible(true);
    //}
    //else
    //{
    //    m_circle->SetVisible(false);
    //}

    ////一定時間後にアイテムを投げる
    //if (m_item && !m_item->IsFlying() && !m_hasThrownItem && m_movePhaseTimer >= 2.0f)
    //{
    //    m_item->Move();
    //    m_hasThrownItem = true;
    //}

    if (m_isCounting)
    {
        m_countTimer += deltaTime;

        float alpha = 0.3 + (1.0f - m_countTimer) * 0.7f;

        if (alpha < 0.0f)
        {
            alpha = 0.0f;
        }

        if (m_countNumber == 3)
        {
            m_count3.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, alpha));
        }
        else if (m_countNumber == 2)
        {
            m_count2.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, alpha));
        }
        else if (m_countNumber == 1)
        {
            m_count1.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, alpha));
        }

        if (m_countTimer >= 1.0f)
        {
            m_countTimer = 0.0f;
            m_countNumber--;

            m_count1.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
            m_count2.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
            m_count3.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));

            if (m_countNumber <= 0)
            {
                m_isCounting = false;

                m_spawner->StartThrow();
                m_hasThrownItem = true;
            }
        }
    }

    if (m_showFailure)
    {
        m_failureTimer += 1.0f / 60.0f;
        if (m_failureTimer >= 4.0f)
        {
            m_showFailure = false;
        }
    }

    if (m_showSuccess)
    {
        m_successTimer += 1.0f / 60.0f;
        if (m_successTimer >= 4.0f)
        {
            m_showSuccess = false;
        }
    }

    if (m_showSuccess || m_showFailure)
    {
        if ((m_showSuccess && m_successTimer >= 4.0f) || (m_showFailure && m_failureTimer >= 4.0f))
        {
            m_showSuccess = false;
            m_showFailure = false;
            m_coinPlayed = false;
            RequestMovePhase();
        }
    }
}

void Game::RequestGameClear()
{
    if (m_gameState != GameState::Playing)
        return;

    m_gameState = GameState::GameClear;
    m_isGameClearShown = true;
    NewGO<GameClear>(10, "gameClear");
}

void Game::RequestTitle()
{

    // すでに Title なら何もしない
    if (m_gameState == GameState::Title)
        return;

    if (Player* player = FindGO<Player>("player"))
    {
        player->Reset();
    }

    // ★ これが重要
    m_requestStart = false;

    // ★ GameOver判定用リセット
    m_idleTimer = 0.0f;
    m_prevPos = Vector3::Zero;

    m_phase = GamePhase::Start;
    m_phaseStep = 0;
    m_itemMove = false;
    m_hasThrownItem = false;

    if (m_spawner)
    {
        Item* currentItem = m_spawner->GetCurrentItem();
        if (currentItem != nullptr)
        {
            currentItem->ResetItem();
            DeleteGO(currentItem);
        }
        m_spawner->Reset();
    }

    if (QTEButton* button = FindGO<QTEButton>("qteButton"))
    {
        DeleteGO(button);
    }

    m_BGM = FindGO<BGMManager>("bgmManager");

    if (m_BGM != nullptr)
    {
        m_BGM->Stop();
    }

    // Title状態
    m_gameState = GameState::Title;

    // Title を前面に生成
    NewGO<Title>(0, "title");

}


void Game::RequestGameOver()
{
    // すでに終了状態なら無視
    if (m_gameState != GameState::Playing)
        return;

    m_requestStart = false;

    // ★ GameOver判定用リセット
    m_idleTimer = 0.0f;
    m_prevPos = Vector3::Zero;

    NewGO<GameOver>(10, "gameOver");
    m_gameState = GameState::GameOver;


}

void Game::SetPrevState(GameState state)
{
    m_prevState = state;
}

GameState Game::GetPrevState()
{
    return m_prevState;
}

void Game::RequestFailureLetter()
{
    m_showFailure = true;
    m_failureTimer = 0.0f;
    SEManager::Play(SE_booing,false);

    m_failCount++;
    if (m_missEffect)
    {
        m_missEffect->Play(m_failCount);
    }
}

void Game::RequestNormalSuccess()
{
    m_showSuccess = true;
    m_successTimer = 0.0f;
    SEManager::Play(SE_cheers, false);

    m_coin->Play();
}

void Game::RequestQTESuccess()
{
    m_isSuccess = true;          // ←追加
    m_successTimer2 = 3.0f;      // ←追加
    m_bigSuccess = true;   // 大成功なら
    m_showSuccess = true;
    m_successTimer = 0.0f;
    SEManager::Play(SE_cheers, false);

    m_coin->Play();
    m_confetti->Play();
}

void Game::Render(RenderContext& rc)
{
    if (m_phase == GamePhase::Start && m_showStart)
    {
        m_startLetter.Draw(rc);
    }

    if (m_showFailure)
    {
        m_failureLetter.Draw(rc);
    }

    if (m_showSuccess)
    {
        m_successLetter.Draw(rc);
    }

    if (m_circle)
    {
        m_circle->Render(rc);
    }

    if (m_coin)
    {
        m_coin->Render(rc);
    }

    if (m_confetti)
    {
        m_confetti->Render(rc);
    }

    if (m_missEffect)
    {
        m_missEffect->Render(rc);
    }

    if (m_isCounting)
    {
        m_countDown.Draw(rc);
        if (m_countNumber == 3)
        {
            m_count3.Draw(rc);
        }
        else if (m_countNumber == 2)
        {
            m_count2.Draw(rc);
        }
        else if (m_countNumber == 1)
        {
            m_count1.Draw(rc);
        }
    }

    switch (AudienceImage)
    {
    case 0:
        m_AudienceSpriteRender.Draw(rc);
        break;
    case 1:
        m_AudienceSpriteRender1.Draw(rc);
        break;
    case 2:
        m_sprite1.Draw(rc);
        m_sprite2.Draw(rc);
        m_sprite3.Draw(rc);
        m_sprite4.Draw(rc);
        m_sprite5.Draw(rc);
        m_sprite6.Draw(rc);
        m_sprite7.Draw(rc);
        m_sprite8.Draw(rc);
        break;
    }
}
