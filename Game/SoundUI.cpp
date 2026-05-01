#include "stdafx.h"
#include "SoundUI.h"
#include "SEManager.h"
#include "BGMManager.h"
#include "SoundSettings.h"

SoundUI::SoundUI()
{

}
SoundUI::~SoundUI()
{

}

bool SoundUI::Start()
{
	//保存されている音量設定を読み込む
	SoundSettings::Load();

	//読み込んだ値をUI用の変数に反映
	m_masterValue = SoundSettings::Master;
	m_BGMValue = SoundSettings::BGM;
	m_SEValue = SoundSettings::SE;

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
	//現在の値を設定クラスへ保存
	SoundSettings::Master = m_masterValue;
	SoundSettings::BGM = m_BGMValue;
	SoundSettings::SE = m_SEValue;

	if (BGMManager* bgm = FindGO<BGMManager>("bgmManager"))
	{
		bgm->ApplyVolume();
	}

	//入力クールタイム減少
	m_inputTimer -= 1.0f / 60.0f;

	Input();
	SelectScale();
	FillBars();

	m_masterBarRender.Update();
	m_SEBarRender.Update();
	m_BGMBarRender.Update();

	m_mastarFillRender.Update();
	m_SEFillRender.Update();
	m_BGMFillRender.Update();
}

void SoundUI::Input()
{
	if (m_inputTimer > 0.0f)return;

	//編集モード中の処理
	if (m_isEditing)
	{
		//右入力で音量アップ
		if (g_pad[0]->IsPress(enButtonRight))
		{
			if (m_selectedIndex == 0)m_masterValue += 0.1f;
			if (m_selectedIndex == 2)m_SEValue += 0.1f;
			if (m_selectedIndex == 1)m_BGMValue += 0.1f;

			m_inputTimer = m_inputInterval;
		}
		//左入力で音量ダウン
		if (g_pad[0]->IsPress(enButtonLeft))
		{
			if (m_selectedIndex == 0)m_masterValue -= 0.1f;
			if (m_selectedIndex == 2)m_SEValue -= 0.1f;
			if (m_selectedIndex == 1)m_BGMValue -= 0.1f;

			m_inputTimer = m_inputInterval;
		}
		//音量値を0.0～1.0に制限
		if (m_masterValue < 0.0f)m_masterValue = 0.0f;
		if (m_masterValue > 1.0f)m_masterValue = 1.0f;
		if (m_SEValue < 0.0f)m_SEValue = 0.0f;
		if (m_SEValue > 1.0f)m_SEValue = 1.0f;
		if (m_BGMValue < 0.0f)m_BGMValue = 0.0f;
		if (m_BGMValue > 1.0f)m_BGMValue = 1.0f;
		//Bボタンで編集終了
		if (g_pad[0]->IsPress(enButtonB))
		{
			m_isEditing = false;
			m_inputTimer = m_inputInterval;
		}
		return;
	}
	//選択モード中の処理
	if (g_pad[0]->IsPress(enButtonUp))
	{
		m_selectedIndex++;
		if (m_selectedIndex > 2)m_selectedIndex = 0;
		m_inputTimer = m_inputInterval;
	}
	if (g_pad[0]->IsPress(enButtonDown))
	{
		m_selectedIndex--;
		if (m_selectedIndex < 0)m_selectedIndex = 2;
		m_inputTimer = m_inputInterval;
	}
	//Aボタンで編集モードに入る
	if (g_pad[0]->IsPress(enButtonA))
	{
		m_isEditing = true;
		m_inputTimer = m_inputInterval;
	}
	if (g_pad[0]->IsPress(enButtonB))
	{
		SoundSettings::Save();

		SEManager::ClearCache();

		m_isFinished = true;
		m_inputTimer = m_inputInterval;
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

void SoundUI::SelectScale()
{
	m_mastarFillRender.SetScale({ 1.0f,1.0f,1.0f });
	m_SEFillRender.SetScale({ 1.0f,1.0f,1.0f });
	m_BGMFillRender.SetScale({ 1.0f,1.0f,1.0f });

	switch (m_selectedIndex)
	{
	case 0:
		m_mastarFillRender.SetScale({ 1.2f,1.2f,1.2f });
		break;
	case 1:
		m_BGMFillRender.SetScale({ 1.2f,1.2f,1.2f });
		break;
	case 2:
		m_SEFillRender.SetScale({ 1.2f,1.2f,1.2f });
		break;
	}
}

bool SoundUI::IsFinished()const
{
	return m_isFinished;
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