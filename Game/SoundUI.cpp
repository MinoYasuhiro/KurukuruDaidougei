#include "stdafx.h"
#include "SoundUI.h"

SoundUI::SoundUI()
{

}
SoundUI::~SoundUI()
{

}

bool SoundUI::Start()
{
	m_masterBarRender.Init("Assets/sprite/Whitebar.dds",1000.0f,400.0f);
	m_SEBarRender.Init("Assets/sprite/Whitebar.dds",1000.0f,400.0f);
	m_BGMBarRender.Init("Assets/sprite/Whitebar.dds", 1000.0f, 400.0f);

	m_masterBarRender.SetPosition({ 0.0f,100.0f,0.0f });
	m_SEBarRender.SetPosition({ 0.0f,-300.0f,0.0f });
	m_BGMBarRender.SetPosition({ 0.0f,-100.0f,0.0f });

	m_mastarFillRender.Init("Assets/sprite/memory.dds", 1000.0f, 400.0f);
	m_SEFillRender.Init("Assets/sprite/memory.dds", 1000.0f, 400.0f);
	m_BGMFillRender.Init("Assets/sprite/memory.dds", 1000.0f, 400.0f);

	return true;
}

void SoundUI::Update()
{
	Input();
	FillBars();
	ApplyVolumes();

	m_masterBarRender.Update();
	m_SEBarRender.Update();
	m_BGMBarRender.Update();

	m_mastarFillRender.Update();
	m_SEFillRender.Update();
	m_BGMFillRender.Update();
}

void SoundUI::Input()
{
	if (m_isEditing)
	{
		if (g_pad[0]->IsTrigger(enButtonRight))
		{
			if (m_selectedIndex == 0)m_masterValue += 0.05f;
			if (m_selectedIndex == 2)m_SEValue += 0.05f;
			if (m_selectedIndex == 1)m_BGMValue += 0.05f;
		}
		if (g_pad[0]->IsTrigger(enButtonLeft))
		{
			if (m_selectedIndex == 0)m_masterValue -= 0.05f;
			if (m_selectedIndex == 2)m_SEValue -= 0.05f;
			if (m_selectedIndex == 1)m_BGMValue -= 0.05f;
		}
		if (m_masterValue < 0.0f)m_masterValue = 0.0f;
		if (m_masterValue > 1.0f)m_masterValue = 1.0f;
		if (m_SEValue < 0.0f)m_SEValue = 0.0f;
		if (m_SEValue > 1.0f)m_SEValue = 1.0f;
		if (m_BGMValue < 0.0f)m_BGMValue = 0.0f;
		if (m_BGMValue > 1.0f)m_BGMValue = 1.0f;
		if (g_pad[0]->IsTrigger(enButtonB))
		{
			m_isEditing = false;
		}
		return;
	}
	if (g_pad[0]->IsTrigger(enButtonUp))
	{
		m_selectedIndex--;
		if (m_selectedIndex < 0)m_selectedIndex = 2;
	}
	if (g_pad[0]->IsTrigger(enButtonDown))
	{
		m_selectedIndex++;
		if (m_selectedIndex > 2)m_selectedIndex = 0;
	}
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		m_isEditing = true;
	}
}

void SoundUI::FillBars()
{
	auto calcX = [&](float v)
		{
			return m_minX + (m_maxX - m_minX) * v;
		};
	m_mastarFillRender.SetPosition({ calcX(m_masterValue),100.0f,0.0f });
	m_SEFillRender.SetPosition({ calcX(m_SEValue),-100.0f,0.0f });
	m_BGMFillRender.SetPosition({ calcX(m_BGMValue),-300.0f,0.0f });
}

void SoundUI::ApplyVolumes()
{
	if (m_BGMSource)
	{
		m_BGMSource->SetVolume(m_masterValue * m_BGMValue);
	}
	if (m_SESource)
	{
		m_SESource->SetVolume(m_masterValue * m_SEValue);
	}
}

void SoundUI::Render(RenderContext& rc)
{
	m_masterBarRender.Draw(rc);
	m_SEBarRender.Draw(rc);
	m_BGMBarRender.Draw(rc);

	m_mastarFillRender.Draw(rc);
	m_SEFillRender.Draw(rc);
	m_BGMFillRender.Draw(rc);
}