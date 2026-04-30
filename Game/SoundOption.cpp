//後々追加
#include "stdafx.h"
#include "SoundOption.h"
#include "Game.h"
#include "SoundUI.h"
#include "Pause.h"
#include "MenuUI.h"

bool SoundOption::Start()
{
	//サウンドオプション画面の背景表示
	m_spriteRender.Init("Assets/sprite/soundOption.dds", 1920, 1080);

	//サウンド設定UIを生成
	m_soundUI = new SoundUI();
	m_soundUI->Start();

	return true;
}

void SoundOption::Update()
{
	//背景スプライトの更新
	m_spriteRender.Update();

	//サウンドUIの更新
	m_soundUI->Update();

	//サウンドUIが終了したら元の画面へ戻る
	if (m_soundUI->IsFinished())
	{
		//サウンドUIを削除
		DeleteGO(m_soundUI);
		m_soundUI = nullptr;

		//ゲーム状態をポーズに戻す
		Game::SetState(GameState::Pause);

		//ポーズ画面を再生成
		NewGO<Pause>(0, "pause");

		//サウンドオプション画面の背景を削除
		DeleteGO(this);
	}
}

void SoundOption::Render(RenderContext& rc)
{
	//背景を描画
	m_spriteRender.Draw(rc);

	//サウンド設定UIを描画
	m_soundUI->Render(rc);
}