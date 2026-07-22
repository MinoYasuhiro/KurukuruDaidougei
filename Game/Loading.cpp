#include "stdafx.h"
#include "Loading.h"
#include <chrono>
#include "Game.h"
#include "Title.h"
#include "LoadingManager.h"

namespace LOADING
{
    // 背景
    const Vector3 BACKGROUND_POSITION = { 0.0f, 0.0f, 0.0f };

    // バー位置
    const Vector3 BAR_POSITION = { 0.0f, -300.0f, 0.0f };

    // バー枠位置
    const Vector3 BASEBAR_POSITION = { -10.0f,-305.0f,0.0f };
}

namespace LETTER
{
    const Vector3 POSITION = { 600.0f, -380.0f, 0.0f };
}

bool Loading::Start()
{
    //---------------------------------------
    // 背景
    //---------------------------------------
    m_Sprite.Init(
        "Assets/Sprite/LoadingBackGround.DDS",
        1920,
        1080);

    m_Sprite.SetPosition(
        LOADING::BACKGROUND_POSITION);

    //---------------------------------------
    // LoadingManager
    //---------------------------------------
    m_loadingManager = NewGO<LoadingManager>(0, "loadingManager");


    //---------------------------------------
    // バー本体
    //---------------------------------------
    m_fillBar.Init(
        "Assets/Sprite/bar_file.DDS",
        m_barWidth,
        40);

    m_fillBar.SetPosition(
        LOADING::BAR_POSITION);

    //---------------------------------------
    // バー枠
    //---------------------------------------
    m_baseBar.Init(
        "Assets/Sprite/LoadingBar.DDS",
        780,
        65);

    m_baseBar.SetPosition(
        LOADING::BASEBAR_POSITION);

    //---------------------------------------
    // Loading文字
    //---------------------------------------
    m_Letter.Init(
        "Assets/Sprite/LoadingLetter.DDS",
        400,
        150);

    m_Letter.SetPosition(
        LETTER::POSITION);

    //---------------------------------------
    // 初期化
    //---------------------------------------
    m_progress = 0.0f;
    m_displayProgress = 0.0f;
    m_finishTimer = 0.0f;
    m_isWaiting = false;

    return true;
}

void Loading::Update()
{
    m_Sprite.Update();

    UpdateLoading();

    //---------------------------------------
    // ロード完了
    //---------------------------------------
    if (m_loadingManager &&
        m_loadingManager->IsFinished() &&
        m_displayProgress >= 0.99f)
    {
        Game* game =
            FindGO<Game>("game");

        if (!game)
        {
            game =
                NewGO<Game>(0, "game");
        }

        game->ResetGame();

        DeleteGO(this);

        return;
    }
}

void Loading::UpdateLoading()
{
    //---------------------------------------
    // LoadingManager更新
    //---------------------------------------
    if (m_loadingManager)
    {
        SetProgress(
            m_loadingManager->GetProgress()
        );
    }

    //---------------------------------------
    // バーを滑らかに補間
    //---------------------------------------
    m_displayProgress +=
        (m_progress - m_displayProgress)
        * 0.08f;

    //---------------------------------------
    // バー描画更新
    //---------------------------------------
    UpdateBar();

    //---------------------------------------
    // Loading文字点滅
    //---------------------------------------
    LoadingLetter();
}


void Loading::UpdateBar()
{

    if (m_displayProgress <= 0.0f)
    {
        return;
    }

    //外枠
    m_baseBar.Update();
    // ------------------------
    // 進行度
    // ------------------------

    float safeProgress = m_displayProgress;

    // 範囲固定
    safeProgress = max(0.001f, safeProgress);
    safeProgress = min(1.0f, safeProgress);

    // NaN・異常検知
    if (!std::isfinite(safeProgress))
    {
        safeProgress = 0.001f;
    }

    // 左端固定
    float leftX = LOADING::BAR_POSITION.x - m_barWidth * 0.5f;

    //丸めた幅を使う
    float currentWidth = floorf(m_barWidth * safeProgress);


    //スケール
    m_fillBar.SetScale({ currentWidth / m_barWidth, 1.0f, 1.0f });

    //位置も丸めた値で計算
    m_fillBar.SetPosition({
        leftX + currentWidth * 0.5f,
        LOADING::BAR_POSITION.y,
        0.0f
        });
    // ------------------------
    // 色を和風に
    // ------------------------
    float glow = sinf(m_displayProgress * 10.0f) * 0.2f + 0.8f;

    m_fillBar.SetMulColor({
        0.9f,
        0.6f + glow * 0.2f,
        0.2f,
        1.0f
        });


    m_fillBar.Update();
}

/// <summary>
/// Loading文字を点滅表示する処理
/// </summary>
void Loading::LoadingLetter()
{
    //---------------------------------------
    // 点滅タイマー
    //---------------------------------------
    m_animTimer +=
        g_gameTime->GetFrameDeltaTime();

    //---------------------------------------
    // α値
    //---------------------------------------
    float alpha =
        0.5f +
        0.5f *
        sinf(m_animTimer * 3.0f);

    m_Letter.SetMulColor(
        {
            1.0f,
            1.0f,
            1.0f,
            alpha
        });

    m_Letter.Update();
}


void Loading::SetProgress(float progress)
{
    m_progress = progress;

    if (m_progress < 0.0f)
    {
        m_progress = 0.0f;
    }

    if (m_progress > 1.0f)
    {
        m_progress = 1.0f;
    }
}


void Loading::Render(RenderContext& rc)
{
    //---------------------------------------
    // 背景
    //---------------------------------------
    m_Sprite.Draw(rc);

    //---------------------------------------
    // バー
    //---------------------------------------
    m_fillBar.Draw(rc);
    m_baseBar.Draw(rc);

    //---------------------------------------
    // Loading文字
    //---------------------------------------
    m_Letter.Draw(rc);
}

