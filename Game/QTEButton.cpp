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

void QTEButton::StartQTE(ButtonType target)
{
	m_targetButton = target;
	m_isFinished = false;
	m_isSuccess = false;
}

void QTEButton::Update()
{
	if (m_isFinished)return;
	Input();
	m_AbuttonRender.Update();
	m_BbuttonRender.Update();
	m_YbuttonRender.Update();
	m_XbuttonRender.Update();
}

void QTEButton::Input()
{
	switch (m_targetButton)
	{
	case ButtonType::A:
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			m_isSuccess = true;
			m_isFinished = true;
		}
		break;
	case ButtonType::B:
		if (g_pad[0]->IsTrigger(enButtonB))
		{
			m_isSuccess = true;
			m_isFinished = true;
		}
		break;
	case ButtonType::Y:
		if (g_pad[0]->IsTrigger(enButtonY))
		{
			m_isSuccess = true;
			m_isFinished = true;
		}
		break;
	case ButtonType::X:
		if (g_pad[0]->IsTrigger(enButtonX))
		{
			m_isSuccess = true;
			m_isFinished = true;
		}
		break;
	}
	if (m_isFinished) return;
	{
		if (g_pad[0]->IsTrigger(enButtonA) ||
			g_pad[0]->IsTrigger(enButtonB) ||
			g_pad[0]->IsTrigger(enButtonY) ||
			g_pad[0]->IsTrigger(enButtonX))
		{
			m_isSuccess = false;
			m_isFinished = true;
		}
	}
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
	}
}