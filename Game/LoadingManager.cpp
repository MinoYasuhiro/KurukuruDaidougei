#include "stdafx.h"
#include "LoadingManager.h"

#include "Player.h"
#include "GameCamera.h"
#include "ItemSpawner.h"
#include "CoinBox.h"
#include "Circle.h"
#include "Arrow.h"
#include "CoinEffect.h"
#include "Confetti.h"
#include "MissEffect.h"

bool LoadingManager::Start()
{
    m_state = LoadingState::ResetPlayer;

    m_progress = 0.0f;
    m_isFinished = false;

    return true;
}

void LoadingManager::SetProgress(int currentStep, int totalStep)
{
    m_progress =
        static_cast<float>(currentStep)
        / static_cast<float>(totalStep);
}

void LoadingManager::Update()
{
    constexpr int TOTAL_STEP = 9;

    switch (m_state)
    {
    case LoadingState::ResetPlayer:
    {
        if (Player* player = FindGO<Player>("player"))
        {
            player->Reset();
        }

        SetProgress(1, TOTAL_STEP);

        m_state = LoadingState::ResetCamera;
        break;
    }

    case LoadingState::ResetCamera:
    {
        if (GameCamera* camera =
            FindGO<GameCamera>("gameCamera"))
        {
            camera->Reset();
        }

        SetProgress(2, TOTAL_STEP);

        m_state =
            LoadingState::ResetSpawner;
        break;
    }

    case LoadingState::ResetSpawner:
    {
        if (ItemSpawner* spawner =
            FindGO<ItemSpawner>("itemSpawner"))
        {
            spawner->Reset();
        }

        SetProgress(3, TOTAL_STEP);

        m_state =
            LoadingState::ResetCoinBox;
        break;
    }

    case LoadingState::ResetCoinBox:
    {
        if (CoinBox* coinBox =
            FindGO<CoinBox>("coinBox"))
        {
            coinBox->Reset();
        }

        SetProgress(4, TOTAL_STEP);

        m_state =
            LoadingState::ResetCircle;
        break;
    }

    case LoadingState::ResetCircle:
    {
        if (Circle* circle =
            FindGO<Circle>("circle"))
        {
            circle->Reset();
        }

        SetProgress(5, TOTAL_STEP);

        m_state =
            LoadingState::ResetArrow;
        break;
    }

    case LoadingState::ResetArrow:
    {
        if (Arrow* arrow =
            FindGO<Arrow>("arrow"))
        {
            arrow->Reset();
        }

        SetProgress(6, TOTAL_STEP);

        m_state =
            LoadingState::ResetCoinEffect;
        break;
    }

    case LoadingState::ResetCoinEffect:
    {
        if (CoinEffect* coinEffect =
            FindGO<CoinEffect>("coinEffect"))
        {
            coinEffect->Reset();
        }

        SetProgress(7, TOTAL_STEP);

        m_state =
            LoadingState::ResetConfetti;
        break;
    }

    case LoadingState::ResetConfetti:
    {
        if (Confetti* confetti =
            FindGO<Confetti>("confetti"))
        {
            confetti->Reset();
        }

        SetProgress(8, TOTAL_STEP);

        m_state =
            LoadingState::ResetMissEffect;
        break;
    }

    case LoadingState::ResetMissEffect:
    {
        if (MissEffect* missEffect =
            FindGO<MissEffect>("missEffect"))
        {
            missEffect->Reset();
        }

        SetProgress(9, TOTAL_STEP);

        m_state =
            LoadingState::Finish;
        break;
    }

    case LoadingState::Finish:
    {
        m_progress = 1.0f;

        m_isFinished = true;

        break;
    }
    }
}