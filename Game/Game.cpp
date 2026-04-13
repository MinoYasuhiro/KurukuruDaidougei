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

GamePhase Game::m_phase = GamePhase::Start;
GameState Game::m_gameState = GameState::Playing;
bool Game::Start()
{
	NewGO<BackGround>(0, "background");
	NewGO<Player>(0, "player");
	NewGO<GameCamera>(0, "gameCamera");
	

	return true;
}


void Game::Update()
{
    nsK2EngineLow::GamePad::BeginFrame();
    g_pad[0]->Update();

    /// ===== ① 状態遷移====
    if (g_pad[0]->IsPress(enButtonA))
    {
        NewGO<Pause>(0, "pause");
        m_gameState = GameState::Pause;
        DeleteGO(this);
        return;
    }


    /// ===== ② GamePhase更新（カメラ用）=====
    const float deltaTime = 1.0f / 60.0f;

    switch (m_phase)
    {
    case GamePhase::Start:
        m_phase = GamePhase::MovePhase;
        m_movePhaseTimer = 0.0f;
        break;

    case GamePhase::MovePhase:
        if (m_gameState == GameState::Playing)
        {
            m_movePhaseTimer += deltaTime;
            if (m_movePhaseTimer >= 15.0f)
            {
                m_phase = GamePhase::AfterMove;
            }
        }
        break;

    case GamePhase::AfterMove:

        m_clearTimer += deltaTime;
        if (m_clearTimer >= 3.0f && !m_isGameClearShown)
        {
           // NewGO<GameClear>(0);
            m_gameState = GameState::GameClear;
            m_isGameClearShown = true;
        }

        break;
    }

    // ===== ③ Playing時だけゲーム進行 =====
    if (m_gameState != GameState::Playing)
        return;

    // --- プレイヤー操作・敵AI・判定 ---

   
}

GamePhase Game::GetPhase()
{
	return m_phase;
}


void Game::Render(RenderContext& rc)
{
	
}
