#include "stdafx.h"
#include "TitleMenu.h"
#include "SEManager.h"


namespace {
    const Vector3 RULE_POS = { -750.0f, -250.0f, 0.0f };//左端
    const Vector3 OP_POS = { -250.0f, -250.0f, 0.0f }; //左
    const Vector3 START_POS = { 250.0f, -250.0f, 0.0f }; //右
    const Vector3 SOUND_POS = { 750.0f, -250.0f, 0.0f }; //右端
}



bool TitleMenu::Start()
{
    m_selectIndex = 0;
    m_result = MenuResult::None;

    m_startRender.Init("Assets/Sprite/GameStart.DDS", 300, 100);
    m_startRender.SetPosition(START_POS);

    m_soundRender.Init("Assets/sprite/Soundtest.dds", 500, 300);
    m_soundRender.SetPosition(SOUND_POS);

    m_operationRender.Init("Assets/Sprite/OperationButton.DDS", 300, 100);
    m_operationRender.SetPosition(OP_POS);

    m_ruleRender.Init("Assets/Sprite/RuleButton.DDS", 300, 100);
    m_ruleRender.SetPosition(RULE_POS);

    m_inputTimer = 0.12f;   
    m_skipInputFrame = true;

    return true;
}

void TitleMenu::Update()
{
    m_inputTimer -= 1.0f / 60.0f;
    SelectScale();

    m_startRender.Update();
    m_soundRender.Update();
    m_operationRender.Update();
    m_ruleRender.Update();

    //クールタイム中は入力無効
    if (m_inputTimer > 0.0f)return;

    if (m_skipInputFrame)
    {
        m_skipInputFrame = false;
        return; // ★このフレームは入力受けない
    }

    if (m_result != MenuResult::None)
    {
        return; 
    }

    Input();
    
}


void TitleMenu::Input()
{
   

    if (g_pad[0]->IsPress(enButtonLeft))
    {
        m_selectIndex--;
        if (m_selectIndex < 0) m_selectIndex = 3;
        
        SEManager::Play(SE_cursor, false);
        m_inputTimer = m_inputinterval; 
    }

    if (g_pad[0]->IsPress(enButtonRight))
    {
        m_selectIndex++;
        if (m_selectIndex > 3) m_selectIndex = 0;

        SEManager::Play(SE_cursor, false);
        m_inputTimer = m_inputinterval; 
    }

    if (g_pad[0]->IsPress(enButtonA))
    {
		SEManager::Play(SE_choice, false);
        switch (m_selectIndex)
        {
        case 0: m_result = MenuResult::Operation; break;
        case 1: m_result = MenuResult::GameStart; break;
        case 2: m_result = MenuResult::SoundOption; break;
        case 3: m_result = MenuResult::Rule; break;
        }

        m_inputTimer = m_inputinterval; 
    }
}

void TitleMenu::SelectScale()
{
    // 全体リセット
    m_startRender.SetScale({ 1,1,1 });
    m_soundRender.SetScale({ 1,1,1 });
    m_operationRender.SetScale({ 1,1,1 });
    m_ruleRender.SetScale({ 1,1,1 });

    // 選択中だけ拡大
    if (m_selectIndex == 0) m_operationRender.SetScale({ 1.2f,1.2f,1.2f });
    if (m_selectIndex == 1) m_startRender.SetScale({ 1.2f,1.2f,1.2f });
    if (m_selectIndex == 2) m_soundRender.SetScale({ 1.2f,1.2f,1.2f });
    if (m_selectIndex == 3) m_ruleRender.SetScale({ 1.2f,1.2f,1.2f });
}

void TitleMenu::Render(RenderContext& rc)
{
    m_startRender.Draw(rc);
    m_soundRender.Draw(rc);
    m_operationRender.Draw(rc);
    m_ruleRender.Draw(rc);
}
