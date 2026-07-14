#include "stdafx.h"
#include "MenuUI.h"
#include "SEManager.h"

MenuUI::MenuUI()
{

}
MenuUI::~MenuUI()
{



}

bool MenuUI::Start()
{
	//スプライトの読み込み
	m_backTitleRender.Init("Assets/sprite/BackTitle.dds", 700.0f, 500.0f);
	m_retryRender.Init("Assets/sprite/Retry.dds", 700.0f, 500.0f);
	m_restartRender.Init("Assets/sprite/Restart.dds", 700.0f, 500.0f);
	m_soundTestRender.Init("Assets/sprite/Soundtest.dds", 700.0f, 500.0f);


	//各UIの配置
	m_backTitleRender.SetPosition({ -300.0f, -250.0f, 0.0f });
	m_retryRender.SetPosition({ 300.0f, -250.0f, 0.0f });
	m_restartRender.SetPosition({ -300.0f, 50.0f, 0.0f });
	m_soundTestRender.SetPosition({ 300.0f, 50.0f, 0.0f });

	SEManager::Init();


	return true;
}

void MenuUI::Update()
{

	Input();
	SelectScale();

	m_backTitleRender.Update();
	m_retryRender.Update();
	m_restartRender.Update();
	m_soundTestRender.Update();
}

void MenuUI::Input()
{
	//変更前の選択位置を保存
	int oldSelectIndex = m_selectIndex;

	//上入力
	if (g_pad[0]->IsPress(enButtonUp))
	{
		//上側の項目へ移動
		if (m_selectIndex == 0)m_selectIndex = 2;
		else if (m_selectIndex == 1)m_selectIndex = 3;
	}
	//下入力
	else if (g_pad[0]->IsPress(enButtonDown))
	{
		//下側の項目へ移動
		if (m_selectIndex == 2)m_selectIndex = 0;
		else if (m_selectIndex == 3)m_selectIndex = 1;
	}
	//左入力
	else if (g_pad[0]->IsPress(enButtonLeft))
	{
		//右側の項目なら左へ移動
		if (m_selectIndex % 2 == 1)
		{
			m_selectIndex -= 1;
		}
	}
	//右入力
	else if (g_pad[0]->IsPress(enButtonRight))
	{
		//左側の項目なら右へ移動
		if (m_selectIndex % 2 == 0)
		{
			m_selectIndex += 1;
		}
	}
	//決定ボタン
	else if (g_pad[0]->IsPress(enButtonA))
	{
		SEManager::Play(SE_decision, false);

		switch (m_selectIndex)
		{
			//タイトルに戻る
		case 0:
			m_result = MenuResult::BackTitle;
			break;

			//始めから
		case 1:
			m_result = MenuResult::Retry;
			break;

			//ゲーム再開
		case 2:
			m_result = MenuResult::Restart;
			break;

			//サウンドオプション
		case 3:
			m_result = MenuResult::SoundOption;
			break;
		}
	}

	//選択位置が変化した場合のみSEを鳴らす
	if (oldSelectIndex != m_selectIndex)
	{
		SEManager::Play(SE_choice, false);
	}
}

void MenuUI::SelectScale()
{
	//通常サイズに戻す
	m_backTitleRender.SetScale({ 1.0f,1.0f,1.0f });
	m_retryRender.SetScale({ 1.0f,1.0f,1.0f });
	m_restartRender.SetScale({ 1.0f,1.0f,1.0f });
	m_soundTestRender.SetScale({ 1.0f,1.0f,1.0f });

	//選択中の項目だけ少し大きくする
	switch (m_selectIndex)
	{
	case 0:
		m_backTitleRender.SetScale({ 1.2f,1.2f,1.2f });
		break;
	case 1:
		m_retryRender.SetScale({ 1.2f,1.2f,1.2f });
		break;
	case 2:
		m_restartRender.SetScale({ 1.2f,1.2f,1.2f });
		break;
	case 3:
		m_soundTestRender.SetScale({ 1.2f,1.2f,1.2f });
		break;
	}
}

void MenuUI::Render(RenderContext& rc)
{
	m_backTitleRender.Draw(rc);
	m_retryRender.Draw(rc);
	m_restartRender.Draw(rc);
	m_soundTestRender.Draw(rc);
}