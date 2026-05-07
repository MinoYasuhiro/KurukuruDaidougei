#include "stdafx.h"
#include "QTEButton.h"

QTEButton::QTEButton()
{

}
QTEButton::~QTEButton()
{

}

bool QTEButton::Start()
{
	m_AbuttonRender.Init("Assets/sprite/Abutton.dds",200.0f,200.0f);
	m_BbuttonRender.Init("Assets/sprite/Bbutton.dds",200.0f,200.0f);
	m_YbuttonRender.Init("Assets/sprite/Ybutton.dds",200.0f,200.0f);
	m_XbuttonRender.Init("Assets/sprite/Xbutton.dds",200.0f,200.0f);

	return true;
}

void QTEButton::StartQTE(ButtonType target,float limitTime)
{
	m_targetButton = target;
	m_limitTime = limitTime;
	m_timer = 0.0f;
	m_isFinished = false;
	m_isSuccess = false;
}

void QTEButton::Update()
{
	if (m_isFinished)return;

	m_timer += g_gameTime->GetFrameDeltaTime();

	if (m_timer >= m_limitTime)
	{
		m_isSuccess = false;
		m_isFinished = true;
		return;
	}

	Input();
	m_AbuttonRender.Update();
	m_BbuttonRender.Update();
	m_YbuttonRender.Update();
	m_XbuttonRender.Update();
}

void QTEButton::Input()
{
	ButtonType pressed = GetPressedButton();

	if (pressed == ButtonType::None)return;

	m_isSuccess = (pressed == m_targetButton);
	m_isFinished = true;
}

ButtonType QTEButton::GetPressedButton()const
{
	if (g_pad[0]->IsTrigger(enButtonA))return
		ButtonType::A;
	if (g_pad[0]->IsTrigger(enButtonB))return
		ButtonType::B;
	if (g_pad[0]->IsTrigger(enButtonX))return
		ButtonType::X;
	if (g_pad[0]->IsTrigger(enButtonY))return
		ButtonType::Y;

	return ButtonType::None;
}

void QTEButton::SetPosition(const Vector3& pos)
{
	m_AbuttonRender.SetPosition(pos);
	m_BbuttonRender.SetPosition(pos);
	m_XbuttonRender.SetPosition(pos);
	m_YbuttonRender.SetPosition(pos);
}

void QTEButton::Render(RenderContext&rc)
{
	switch (m_targetButton)
	{
	case ButtonType::A:
		m_AbuttonRender.Draw(rc);
		break;
	case ButtonType::B:
		m_BbuttonRender.Draw(rc);
		break;
	case ButtonType::Y:
		m_YbuttonRender.Draw(rc);
		break;
	case ButtonType::X:
		m_XbuttonRender.Draw(rc);
		break;
	default:
		break;
	}
}