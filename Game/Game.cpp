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
#include "QTE.h"
#include "QTEButton.h"
#include <random>
#include "Umbrella.h"
#include "CoinBox.h"
#include "SEManager.h"
#include "Item.h"
#include "Title.h"
#include "Circle.h"
#include "SoundSettings.h"

GamePhase Game::m_phase = GamePhase::Start;
GameState Game::m_gameState = GameState::Playing;

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

    m_circle = NewGO<Circle>(0, "circle");

    m_backGround = NewGO<BackGround>(0, "background");
    //m_umbrella = NewGO<Umbrella>(0, "umbrella");
    m_player = NewGO<Player>(0, "player");
    m_gameCamera = NewGO<GameCamera>(0, "gameCamera");

    m_startLetter.Init("Assets/sprite/Start.dds", 800.0f, 200.0f);
    m_startLetter.SetPosition({ 640.0f,360.0f,0.0f });
    m_startLetter.SetScale({ 2.0f,2.0f,2.0f });

    m_failureLetter.Init("Assets/sprite/Failure.dds", 800.0f, 200.0f);
    m_failureLetter.SetPosition({ 640.0f,360.0f,0.0f });
    m_failureLetter.SetScale({ 2.0f,2.0f,2.0f });

    m_successLetter.Init("Assets/sprite/Success.dds", 800.0f, 200.0f);
    m_successLetter.SetPosition({ 640.0f,360.0f,0.0f });
    m_successLetter.SetScale({ 2.0f,2.0f,2.0f });

    m_startTimer = 0.0f;
    m_showStart = true;

    m_failureTimer = 0.0f;
    m_showFailure = false;

    m_successTimer = 0.0f;
    m_showSuccess = false;

    m_BGM = FindGO<BGMManager>("bgmManager");

    if (m_BGM == nullptr)
    {
        m_BGM = NewGO<BGMManager>(0, "bgmManager");
        m_BGM->Init();
    }

    m_BGM->Play(BGM_NormalUmbrella);

    //m_coinBox = NewGO<CoinBox>(0, "coinBox");

    return true;
}

void Game::Update()
{
    nsK2EngineLow::GamePad::BeginFrame();
    g_pad[0]->Update();

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

    m_modelRender.Update();
    //m_coinBox->Update();
    if (m_circle)
    {
        m_circle->Update();
    }
}

GamePhase Game::GetPhase()
{
    return m_phase;
}

void Game::ResetGame()
{
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

    // ★ 残っているオブジェクトを消す
    if (m_item)
    {
        DeleteGO(m_item);
        m_item = nullptr;
    }

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
        m_BGM->Play(BGM_NormalUmbrella);
    }

    m_phase = GamePhase::Start;

    m_requestStart = true;
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
    if (m_phase != GamePhase::QTEMove&& g_pad[0]->IsPress(enButtonY) && m_gameState == GameState::Playing)
    {
        SEManager::StopLoop(SE_run);

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
        if (m_startTimer >= 2.0f)
        {
            m_showStart = false;
            RequestMovePhase();
        }
        break;

    case GamePhase::MovePhase:
        m_movePhaseTimer += deltaTime;
        //アイテム未生成時のみ生成
        if (!m_itemMove)
        {
            if (m_item)
            {
                DeleteGO(m_item);
                m_item = nullptr;
            }

            m_item = NewGO<Item>(0, "item");

            //放物運動準備
            m_item->PrepareParabola();

            m_itemMove = true;
            m_hasThrownItem = false;
        }
        if (m_movePhaseTimer >= 15.0f)
        {
            m_movePhaseTimer = 0.0f;

            if (m_phaseStep == 0)
            {
                m_phase = GamePhase::AfterMove;      // 通常傘回し
            }
            else if (m_phaseStep == 1)
            {
                m_phase = GamePhase::SpecialMove;    // 特殊傘回し
            }
            else
            {
                m_phase = GamePhase::QTEMove;        // QTE
            }
        }
        break;

    case GamePhase::AfterMove:
        ////////通常傘回し用の処理////////
        m_clearTimer += deltaTime;
        if (m_clearTimer >= 30.0f)
        {
            m_clearTimer = 0.0f;
            m_phaseStep = 1;          // 次は特殊
            m_phase = GamePhase::MovePhase;
        }
        break;

    case GamePhase::SpecialMove:
        //特殊傘回し用の処理
        m_clearTimer += deltaTime;

        if (m_clearTimer >= 30.0f)    // 秒数
        {
            m_clearTimer = 0.0f;
            m_phaseStep = 2;          // 次はQTE
            m_phase = GamePhase::MovePhase;
        }
        break;

    case GamePhase::QTEMove:
        if (!m_qteStarted)
        {
            m_button = NewGO<QTEButton>(0, "qteButton");
            m_button->StartQTE(ButtonType::Y, 8.0f);
            m_qteStarted = true;
            break;
        }

        
        m_button->Update();

        if (m_button->IsFinished())
        {
            m_qteResultSuccess = m_button->IsSuccess();
            m_waitQTEResult = true;

            if (m_button->IsSuccess())
            {
                // 成功処理
                m_gameState = GameState::GameClear;
                NewGO<GameClear>(10, "gameClear");
            }
            else
            {
                // 失敗処理
                m_gameState = GameState::GameOver;
                NewGO<GameOver>(10, "gameOver");
            }
                
            m_phase = GamePhase::Start;

            DeleteGO(m_button);
            m_button = nullptr;
            m_qteStarted = false;

            return;
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
    if (m_item && m_phase == GamePhase::MovePhase && !m_hasThrownItem && !m_item->IsFlying())
    {
        Vector3 landingPosition = m_item->GetPlannedLandingPosition();
        m_circle->SetPosition(landingPosition);
        m_circle->SetVisible(true);
    }
    else
    {
        m_circle->SetVisible(false);
    }

    //一定時間後にアイテムを投げる
    if (m_item && !m_item->IsFlying() && !m_hasThrownItem && m_movePhaseTimer >= 2.0f)
    {
        m_item->Move();
        m_hasThrownItem = true;
    }

    if (m_showFailure)
    {
        m_failureTimer += 1.0f / 60.0f;
        if (m_failureTimer >= 2.0f)
        {
            m_showFailure = false;
        }
    }

    if (m_showSuccess)
    {
        m_successTimer += 1.0f / 60.0f;
        if (m_successTimer >= 2.0f)
        {
            m_showSuccess = false;
        }
    }
}

//void Game::RequestGameClear()
//{
//    if (m_gameState != GameState::Playing)
//        return;
//
//    m_gameState = GameState::GameClear;
//    m_isGameClearShown = true;
//    NewGO<GameClear>(10, "gameClear");
//}

void Game::RequestTitle()
{

    // すでに Title なら何もしない
    if (m_gameState == GameState::Title)
        return;

    // ★ これが重要
    m_requestStart = false;

    // ★ GameOver判定用リセット
    m_idleTimer = 0.0f;
    m_prevPos = Vector3::Zero;

    m_BGM = FindGO<BGMManager>("bgmManager");

    if (m_BGM != nullptr)
    {
        m_BGM->Stop();
    }

    // Title状態
    m_gameState = GameState::Title;

    // Title を前面に生成
    NewGO<Title>(20, "title");

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

void Game::RequestFailureLetter()
{
    m_showFailure = true;
    m_failureTimer = 0.0f;
    SEManager::Play(SE_booing,false);
}

void Game::RequestSuccessLetter()
{
    m_showSuccess = true;
    m_successTimer = 0.0f;
    SEManager::Play(SE_cheers,false);
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

    //m_coinBox->Render(rc);
    if (m_circle)
    {
        m_circle->Render(rc);
    }
}
