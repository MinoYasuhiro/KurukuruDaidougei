#include "stdafx.h"
#include "QTETimerUI.h"
#include "Game.h"

bool QTETimerUI::Start()
{
	/*m_frame.Init("Assets/sprite/QTETimeBar.dds", 500, 100);
	m_frame.SetPosition(Vector3{ 0.0f,-200.0f,0.0f });*/
	m_colorBars.Init("Assets/sprite/QTETimeBarColor.dds", 500, 100);
	m_colorBars.SetPivot(Vector2{ 0.0f,0.5f });
	m_colorBars.SetPosition(Vector3{ -250.0f,-400.0f,0.0f });

	return true;
}

void QTETimerUI::StartQTE(float limitTime)
{
	m_maximumTime = limitTime;
	m_remainingTime = limitTime;
	m_activeFlag = true;
}

void QTETimerUI::Update()
{
	if (Game::GetState() != GameState::Playing)return;
}

void QTETimerUI::SetProgress(float rate)
{
	if (rate < 0.05f)rate = 0.0f;
	if (rate > 1.0f)rate = 1.0f;

	m_colorBars.SetScale(Vector3{ rate,1.0f,1.0f });
}

void QTETimerUI::Render(RenderContext& rc)
{
	/*m_frame.Update();
	m_frame.Draw(rc);*/

	m_colorBars.Update();
	m_colorBars.Draw(rc);
}