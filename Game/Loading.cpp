#include "stdafx.h"
#include "Loading.h"
#include <chrono>
#include "Game.h"
#include "Title.h"

namespace LOADING
{
    const Vector3 BACKGROUND_POSITION = { 0.0f, 0.0f, 0.0f };

    // 円（右下）
    const Vector3 CIRCLE_POSITION = { 700.0f, -400.0f, 0.0f };

    // バー位置（中央寄りに調整）
    const Vector3 BAR_POSITION = { 0.0f, -300.0f, 0.0f };

    // バーの枠の位置
    const Vector3 BASEBAR_POSITION = { -10.0f,-305.0f,0.0f };
};

namespace LIGHT
{
    const Vector3 SCALE = { 2.0f, 2.0f, 1.0f };
};

namespace LETTER
{
    const Vector3 POSITION = { 600.0f, -380.0f, 0.0f };
}

bool Loading::Start()
{
    // ------------------------
    // 背景
    // ------------------------
    m_Sprite.Init("Assets/Sprite/LoadingBackGround.DDS", 1920, 1080);
    m_Sprite.SetPosition(LOADING::BACKGROUND_POSITION);

    // ------------------------
    // メーター（中身）
    // ------------------------
    m_fillBar.Init("Assets/Sprite/bar_file.DDS", m_barWidth, 40);
    m_fillBar.SetPosition(LOADING::BAR_POSITION);

    // ------------------------
    // メーター（外枠）
    // ------------------------
    m_baseBar.Init("Assets/Sprite/LoadingBar.DDS", 780, 65);
    m_baseBar.SetPosition(LOADING::BASEBAR_POSITION);

    m_Letter.Init("Assets/Sprite/LoadingLetter.DDS", 400, 150);
    m_Letter.SetPosition(LETTER::POSITION);

   

    // ✔ 色（UIに合わせる）
   // m_fillBar.SetMulColor({ 0.8f, 0.2f, 1.0f, 1.0f });


    // ------------------------
    // 時間
    // ------------------------
    m_startTime = std::chrono::steady_clock::now();

    return true;
}

void Loading::Update()
{
    m_Sprite.Update();
   
    // ------------------------
    // 進行度
    // ------------------------
    LoadingMate();

    // ------------------------
    // バー更新
    // ------------------------
    UpdateBar();

    //-------------------------
    //文字点滅
    //-------------------------
    LoadingLetter();
    // ------------------------
    // 遷移
    // ------------------------
    if (m_isWaiting)
    {
        m_finishTimer += g_gameTime->GetFrameDeltaTime();

        if (m_finishTimer >= m_finishWaitTime)
        {
            Game* game = FindGO<Game>("game");

            if (!game)
            {
                game = NewGO<Game>(0, "game");
            }

            game->ResetGame();

            DeleteGO(this);
            return;
        }
    }
   
}

void Loading::LoadingMate()
{
    auto now = std::chrono::steady_clock::now();

    float elapsed = std::chrono::duration<float>(now - m_startTime).count();

    m_progress = elapsed / m_totalTime;

    if (m_progress >= 1.0f)
    {
        m_progress = 1.0f;
        m_isFinished = true;
        m_isWaiting = true; //待機スタート
    }

}

void Loading::UpdateBar()
{

    if (m_progress <= 0.0f)
    {
        return;
    }

    //外枠
    m_baseBar.Update();
    // ------------------------
    // 進行度
    // ------------------------

    float safeProgress = m_progress;

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
    float glow = sinf(m_progress * 10.0f) * 0.2f + 0.8f;
  
    m_fillBar.SetMulColor({
        0.9f,
        0.6f + glow * 0.2f,
        0.2f,
        1.0f
        });

    m_fillBar.Update();
}

void Loading::LoadingLetter()
{
    
    // タイマー加算
    m_animTimer += g_gameTime->GetFrameDeltaTime();

    // 点滅（sin波）
    float alpha = 0.5f + 0.5f * sinf(m_animTimer * 3.0f);

    m_Letter.SetMulColor({
        1.0f,
        1.0f,
        1.0f,
        alpha
        });

    m_Letter.Update();


}

void Loading::Render(RenderContext& rc)
{
    m_Sprite.Draw(rc);

    // ✅ メーター
    m_fillBar.Draw(rc);
    m_baseBar.Draw(rc);

    //文字
    m_Letter.Draw(rc);
   
}
