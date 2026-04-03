#include "stdafx.h"
#include "MenuUI.h"

MenuUI::MenuUI()
{

}
MenuUI::~MenuUI()
{

}

bool MenuUI::Start()
{
	m_backTitleRender.Init("Assets/sprite/Backritle.dds", 800.0f, 800.0f);
	m_endGameRender.Init("Assets/sprite/Endgame.dds", 800.0f, 800.0f);
	m_restartRender.Init("Assets/sprite/Restart.dds", 800.0f, 800.0f);
	m_soundTestRender.Init("Assets/sprite/Soundtest.dds", 800.0f, 800.0f);


	m_backTitleRender.SetPosition({ -200.0f, -200.0f, 0.0f });
	m_endGameRender.SetPosition({ 200.0f, -200.0f, 0.0f });
	m_restartRender.SetPosition({ -200.0f, 0.0f, 0.0f });
	m_soundTestRender.SetPosition({ 200.0f, 0.0f, 0.0f });


	return true;
}

void MenuUI::Update()
{

	Input();
	SelectScale();

	m_backTitleRender.Update();
	m_endGameRender.Update();
	m_restartRender.Update();
	m_soundTestRender.Update();
}

void MenuUI::Input()
{
	if (g_pad[0]->IsTrigger(enButtonUp))
	{
		if (m_selectIndex == 0)m_selectIndex = 2;
		else if (m_selectIndex == 1)m_selectIndex = 3;
	}
	if (g_pad[0]->IsTrigger(enButtonDown))
	{
		if (m_selectIndex == 2)m_selectIndex = 0;
		else if (m_selectIndex == 3)m_selectIndex = 1;
	}
	if (g_pad[0]->IsTrigger(enButtonLeft))
	{
		if (m_selectIndex % 2 == 1)
		{
			m_selectIndex -= 1;
		}
	}
	if (g_pad[0]->IsTrigger(enButtonRight))
	{
		if (m_selectIndex % 2 == 0)
		{
			m_selectIndex += 1;
		}
	}
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		switch (m_selectIndex)
		{
		case 0:
			m_testFont.SetText(L"BackTitle");
			m_testFont.SetPosition({ 0.0f,500.0f,0.0f });
			m_testFont.SetColor(g_vec4White);
			break;
		case 1:
			m_testFont.SetText(L"EndGame");
			m_testFont.SetPosition({ 0.0f,500.0f,0.0f });
			m_testFont.SetColor(g_vec4White);
			break;
		case 2:
			m_testFont.SetText(L"Restart");
			m_testFont.SetPosition({ 0.0f,500.0f,0.0f });
			m_testFont.SetColor(g_vec4White);
			break;
		case 3:
			m_testFont.SetText(L"SoundTest");
			m_testFont.SetPosition({ 0.0f,500.0f,0.0f });
			m_testFont.SetColor(g_vec4White);
			break;
		}
	}
}

void MenuUI::SelectScale()
{
	m_backTitleRender.SetScale({ 1.0f,1.0f,1.0f });
	m_endGameRender.SetScale({ 1.0f,1.0f,1.0f });
	m_restartRender.SetScale({ 1.0f,1.0f,1.0f });
	m_soundTestRender.SetScale({ 1.0f,1.0f,1.0f });

	switch (m_selectIndex)
	{
	case 0:
		m_backTitleRender.SetScale({ 1.2f,1.2f,1.2f });
		break;
	case 1:
		m_endGameRender.SetScale({ 1.2f,1.2f,1.2f });
		break;
	case 2:
		m_restartRender.SetScale({ 1.2f,1.2f,1.2f });
		break;
	case 3:
		m_soundTestRender.SetScale({ 1.2f,1.2f,1.2f });
		break;
	}
}

void MenuUI::Render(RenderContext&rc)
{
	m_backTitleRender.Draw(rc);
	m_endGameRender.Draw(rc);
	m_restartRender.Draw(rc);
	m_soundTestRender.Draw(rc);
	m_testFont.Draw(rc);
}