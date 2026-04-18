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
#include "SoundTest.h"
#include "QTE.h"
#include <random>
#include "Umbrella.h"
#include "CoinBox.h"
#include "SEManager.h"

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
    SEManager::Init();

    //ゲームの初期化
    m_phase = GamePhase::Start;

    m_backGround = NewGO<BackGround>(0, "background");
    m_umbrella = NewGO<Umbrella>(0, "umbrella");
    m_player = NewGO<Player>(0, "player");
    m_gameCamera = NewGO<GameCamera>(0, "gameCamera");

    m_startLetter.Init("Assets/sprite/Start.dds", 800.0f, 200.0f);
    m_startLetter.SetPosition({ 640.0f,360.0f,0.0f });
    m_startLetter.SetScale({ 2.0f,2.0f,2.0f });

    m_startTimer = 0.0f;
    m_showStart = true;

    //m_coinBox = NewGO<CoinBox>(0, "coinBox");

    return true;
}
void Game::Update()
{
    nsK2EngineLow::GamePad::BeginFrame();
    g_pad[0]->Update();

    // Title中・GameClear中は一切処理しない
    if (m_gameState != GameState::Playing)
        return;

    // ===== ポーズ呼び出し =====
    if (g_pad[0]->IsPress(enButtonY) && m_gameState == GameState::Playing)
    {
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
            m_phase = GamePhase::MovePhase;
            m_movePhaseTimer = 0.0f;
        }
        break;

    case GamePhase::MovePhase:
        m_movePhaseTimer += deltaTime;
        if (m_movePhaseTimer >= 15.0f)
            m_phase = GamePhase::AfterMove;
        break;

    case GamePhase::AfterMove:
        m_clearTimer += deltaTime;
        if (m_clearTimer >= 10.0f)
        {
            NewGO<GameClear>(10, "gameClear"); // 前面表示
            m_gameState = GameState::GameClear;

        }
        break;
    }

    //m_coinBox->Update();
}
GamePhase Game::GetPhase()
{
    return m_phase;
}

void Game::ResetGame()
{

    m_gameState = GameState::Playing;
    m_phase = GamePhase::Start;

    // タイマー初期化
    m_movePhaseTimer = 0.0f;
    m_clearTimer = 0.0f;
    m_isGameClearShown = false;

    //// Player 初期化
    //Player* player = FindGO<Player>("player");
    //if (player)
    //{
    //    player->Reset();
    //}

    if (GameCamera* cam = FindGO<GameCamera>("gameCamera"))
        cam->Reset();
}


void Game::Render(RenderContext& rc)
{
    if (m_phase == GamePhase::Start && m_showStart)
    {
        m_startLetter.Draw(rc);
    }

    //m_coinBox->Render(rc);
}