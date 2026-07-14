#include "stdafx.h"
#include "SoundUI.h"
#include "SEManager.h"
#include "BGMManager.h"
#include "SoundSettings.h"
#include "SEManager.h"

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

	//選択肢の初期化
	m_masterRender.Init("Assets/sprite/Master.dds", 225.0f, 225.0f);
	m_SERender.Init("Assets/sprite/SE.dds", 150.0f, 150.0f);
	m_BGMRender.Init("Assets/sprite/BGM.dds", 175.0f, 175.0f);

	//各選択肢の位置設定
	m_masterRender.SetPosition({ -750.0f,200.0f,0.0f });
	m_SERender.SetPosition({ -750.0f,0.0f,0.0f });
	m_BGMRender.SetPosition({ -750.0f,-200.0f,0.0f });

	//バーの初期化
	m_masterBarRender.Init("Assets/sprite/Bar.dds", 1500.0f, 400.0f);
	m_SEBarRender.Init("Assets/sprite/Bar.dds", 1500.0f, 400.0f);
	m_BGMBarRender.Init("Assets/sprite/Bar.dds", 1500.0f, 400.0f);

	//各バーの位置設定
	m_masterBarRender.SetPosition({ 0.0f,200.0f,0.0f });
	m_SEBarRender.SetPosition({ 0.0f,-200.0f,0.0f });
	m_BGMBarRender.SetPosition({ 0.0f,0.0f,0.0f });

	//つまみ部分の初期化
	m_mastarFillRender.Init("Assets/sprite/memory.dds", 150.0f, 150.0f);
	m_SEFillRender.Init("Assets/sprite/memory.dds", 150.0f, 150.0f);
	m_BGMFillRender.Init("Assets/sprite/memory.dds", 150.0f, 150.0f);

	m_masterFillSelection.Init("Assets/sprite/KnnbSelection.dds", 175.0f, 175.0f);
	m_SEFillSelection.Init("Assets/sprite/KnnbSelection.dds", 175.0f, 175.0f);
	m_BGMFillSelection.Init("Assets/sprite/KnnbSelection.dds", 175.0f, 175.0f);

	float bar[3] = { 200.0f,0.0f,-200.0f };
	for (int j = 0;j < 3;j++)
	{
		for (int i = 0;i < kSeparatorNum;i++)
		{
			m_separator[j][i].Init("Assets/sprite/Separator.dds", 100.0f, 100.0f);

			float t = (float)i / (kSeparatorNum - 1);
			float x = m_minX + (m_maxX - m_minX) * t;

			m_separator[j][i].SetPosition({ x,bar[j] - 5.0f,0.0f });
		}
	}

	m_inputTimer = m_inputInterval;

	SEManager::Init();

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

	m_masterRender.Update();
	m_SERender.Update();
	m_BGMRender.Update();

	m_masterBarRender.Update();
	m_SEBarRender.Update();
	m_BGMBarRender.Update();

	m_mastarFillRender.Update();
	m_SEFillRender.Update();
	m_BGMFillRender.Update();

	m_masterFillSelection.Update();
	m_SEFillSelection.Update();
	m_BGMFillSelection.Update();

	for (int j = 0;j < 3;j++)
	{
		for (int i = 0;i < kSeparatorNum;i++)
		{
			m_separator[j][i].Update();
		}
	}
}

void SoundUI::Input()
{
	//クールタイム中は入力無効
	if (m_inputTimer > 0.0f)return;

	//編集モード中の処理
	if (m_isEditing)
	{
		//右入力で音量アップ
		if (g_pad[0]->IsPress(enButtonRight))
		{
			if (m_selectedIndex == 0)m_masterValue += 0.1f;
			if (m_selectedIndex == 1)m_SEValue += 0.1f;
			if (m_selectedIndex == 2)m_BGMValue += 0.1f;

			m_inputTimer = m_inputInterval;
		}
		//左入力で音量ダウン
		if (g_pad[0]->IsPress(enButtonLeft))
		{
			if (m_selectedIndex == 0)m_masterValue -= 0.1f;
			if (m_selectedIndex == 1)m_SEValue -= 0.1f;
			if (m_selectedIndex == 2)m_BGMValue -= 0.1f;

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
		m_selectedIndex--;
		if (m_selectedIndex < 0)m_selectedIndex = 2;
		m_inputTimer = m_inputInterval;
	}
	if (g_pad[0]->IsPress(enButtonDown))
	{
		m_selectedIndex++;
		if (m_selectedIndex > 2)m_selectedIndex = 0;
		m_inputTimer = m_inputInterval;
	}
	//Aボタンで編集モードに入る
	if (g_pad[0]->IsPress(enButtonA))
	{
		m_isEditing = true;
		m_inputTimer = m_inputInterval;
	}
	//Bボタンで終了&保存
	if (g_pad[0]->IsPress(enButtonB))
	{
		//設定を保存
		SoundSettings::Save();

		//SEキャッシュをクリア
		SEManager::ClearCache();

		// 戻るSE
		SEManager::Play(SE_return, false);

		//このUIを終了状態にする
		m_isFinished = true;
		m_inputTimer = m_inputInterval;
	}
}

void SoundUI::FillBars()
{
	//値(0～1)をX座標に変換
	auto calcX = [&](float v)
		{
			return m_minX + (m_maxX - m_minX) * v;
		};
	//各バーの位置更新
	m_mastarFillRender.SetPosition({ calcX(m_masterValue),200.0f,0.0f });
	m_SEFillRender.SetPosition({ calcX(m_SEValue),0.0f,0.0f });
	m_BGMFillRender.SetPosition({ calcX(m_BGMValue),-200.0f,0.0f });

	m_masterFillSelection.SetPosition({ calcX(m_masterValue),200.0f,0.0f });
	m_SEFillSelection.SetPosition({ calcX(m_SEValue),0.0f,0.0f });
	m_BGMFillSelection.SetPosition({ calcX(m_BGMValue),-200.0f,0.0f });
}

void SoundUI::SelectScale()
{
	//通常サイズに戻す
	m_mastarFillRender.SetScale({ 1.0f,1.0f,1.0f });
	m_SEFillRender.SetScale({ 1.0f,1.0f,1.0f });
	m_BGMFillRender.SetScale({ 1.0f,1.0f,1.0f });

	//選択中だけ拡大
	switch (m_selectedIndex)
	{
	case 0:
		m_mastarFillRender.SetScale({ 1.2f,1.2f,1.2f });
		break;
	case 1:
		m_SEFillRender.SetScale({ 1.2f,1.2f,1.2f });
		break;
	case 2:
		m_BGMFillRender.SetScale({ 1.2f,1.2f,1.2f });
		break;
	}
}

bool SoundUI::IsFinished()const
{
	return m_isFinished;
}

void SoundUI::Render(RenderContext& rc)
{
	m_masterRender.Draw(rc);
	m_SERender.Draw(rc);
	m_BGMRender.Draw(rc);

	m_masterBarRender.Draw(rc);
	m_SEBarRender.Draw(rc);
	m_BGMBarRender.Draw(rc);

	for (int j = 0;j < 3;j++)
	{
		for (int i = 0;i < kSeparatorNum;i++)
		{
			m_separator[j][i].Draw(rc);
		}
	}

	m_mastarFillRender.Draw(rc);
	m_SEFillRender.Draw(rc);
	m_BGMFillRender.Draw(rc);

	if (m_isEditing)
	{
		switch (m_selectedIndex)
		{

		case 0:
			m_masterFillSelection.Draw(rc);
			break;
		case 1:
			m_SEFillSelection.Draw(rc);
			break;
		case 2:
			m_BGMFillSelection.Draw(rc);
			break;
		}
	}
}