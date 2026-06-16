#include "stdafx.h"
#include "SoundOption.h"
#include "Game.h"
#include "SoundUI.h"
#include "Pause.h"
#include "MenuUI.h"
#include "Title.h"

bool SoundOption::Start()
{
	//サウンドオプション画面の背景表示
	m_spriteRender.Init("Assets/Sprite/SoundBackGround.DDS", 1920, 1080);
	//サウンド文字
	m_textRender.Init("Assets/Sprite/Soundtext.DDS", 400, 100);
	m_textRender.SetPosition(Vector3(0.0f, 330.0f, 0.0f));

	//操作説明
	m_operationInstructions.Init("Assets/Sprite/operationInstructions.dds", 1920, 1080);

	//サウンド設定UIを生成
	m_soundUI = new SoundUI();
	m_soundUI->Start();

	return true;
}

void SoundOption::Update()
{
	//背景スプライトの更新
	m_spriteRender.Update();

	//文字スプライトの更新
	m_textRender.Update();

	//操作説明スプライトの更新
	m_operationInstructions.Update();

	//サウンドUIの更新
	m_soundUI->Update();

	//サウンドUIが終了したら元の画面へ戻る
	if (m_soundUI->IsFinished())
	{
		//サウンドUIを削除
		DeleteGO(m_soundUI);
		m_soundUI = nullptr;

		if (m_transitionSource == FromPause)
		{
			//ゲーム状態をポーズに戻す
			Game::SetState(GameState::Pause);

			//ポーズ画面を再生成
			NewGO<Pause>(0, "pause");
		}
		else if (m_transitionSource == FromTitle)
		{
			//ゲーム状態をタイトルに戻す
			Game::SetState(GameState::Title);

			//タイトル画面を再生成
			NewGO<Title>(0, "title");
		}

		//サウンドオプション画面の背景を削除
		DeleteGO(this);
	}
}

void SoundOption::SetTransitionSource(TransitionSource transitionSource)
{
	m_transitionSource = transitionSource;
}

void SoundOption::Render(RenderContext& rc)
{
	//背景を描画
	m_spriteRender.Draw(rc);

	//文字を描画
	m_textRender.Draw(rc);

	//操作説明描画
	m_operationInstructions.Draw(rc);

	//サウンド設定UIを描画
	m_soundUI->Render(rc);
}