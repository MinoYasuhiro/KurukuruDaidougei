#include "stdafx.h"
#include "QTEButton.h"
#include "QTETimerUI.h"
#include "Game.h"

QTEButton::QTEButton()
{

}
QTEButton::~QTEButton()
{
	for (auto sprite : m_buttonSprites)
	{
		delete sprite;
	}
	m_buttonSprites.clear();

	if (m_qteTimerUI != nullptr)
	{
		DeleteGO(m_qteTimerUI);
		m_qteTimerUI = nullptr;
	}
}

void QTEButton::StartQTE(const std::vector<ButtonType>& targetButtons, float limitTime)
{
	m_targetButtons = targetButtons;	//押すべきボタンを設定
	m_limitTime = limitTime;			//制限時間を設定
	m_timer = 0.0f;						//タイマー初期化
	m_isFinished = false;				//QTE未終了
	m_isSuccess = false;				//成功状態リセット
	m_currentStep = 0;

	//同じボタンが連続しても正しく描画できるよう、ボタン数に応じてスプライトを動的に生成する
	for (auto sprite : m_buttonSprites)
	{
		delete sprite;
	}
	m_buttonSprites.clear();

	for (const auto& type : targetButtons)
	{
		SpriteRender* sprite = new SpriteRender();

		const char* path = "";
		switch (type)
		{
		case ButtonType::A:path = "Assets/sprite/Abutton.dds";
			break;
		case ButtonType::B:path = "Assets/sprite/Bbutton.dds";
			break;
		case ButtonType::X:path = "Assets/sprite/Xbutton.dds";
			break;
		case ButtonType::Y:path = "Assets/sprite/Ybutton.dds";
			break;
		}

		sprite->Init(path, 150.0f, 150.0f);
		m_buttonSprites.push_back(sprite);
	}

	m_qteTimerUI = NewGO<QTETimerUI>(0);

	m_qteTimerUI->StartQTE(limitTime);
}

void QTEButton::Update()
{
	if (Game::GetState() != GameState::Playing)
	{
		if (g_pad[0]->IsPress(enButtonA))m_lastPressedButton = ButtonType::A;
		else if (g_pad[0]->IsPress(enButtonB))m_lastPressedButton = ButtonType::B;
		else if (g_pad[0]->IsPress(enButtonX))m_lastPressedButton = ButtonType::X;
		else if (g_pad[0]->IsPress(enButtonY))m_lastPressedButton = ButtonType::Y;
		else m_lastPressedButton = ButtonType::None;

		return;
	}
	//すでに終了しているなら何もしない
	if (m_isFinished)return;

	//m_qteTimerUI->Update();

	// クールタイムを減らす
	if (m_inputCooldown > 0.0f)
	{
		m_inputCooldown -= g_gameTime->GetFrameDeltaTime();
	}

	//経過時間を加算
	m_timer += g_gameTime->GetFrameDeltaTime();

	float rate = (m_limitTime - m_timer) / m_limitTime;

	m_qteTimerUI->SetProgress(rate);

	//制限時間を超えたら失敗
	if (m_timer >= m_limitTime)
	{
		m_isSuccess = false;	//失敗
		m_isFinished = true;	//終了

		if (m_qteTimerUI != nullptr)
		{
			DeleteGO(m_qteTimerUI);
			m_qteTimerUI = nullptr;
		}
		return;
	}

	// クールタイム中以外なら入力受付
	if (m_inputCooldown <= 0.0f)
	{
		Input();
	}

	//画面中央からボタン数に応じて左右に並ぶよう座標を算出
	const float buttonSpacing = 250.0f;
	Vector3 drawPos = m_position;
	drawPos.x -= (float)(m_targetButtons.size() - 1) * buttonSpacing / 2.0f;

	for (int i = 0; i < (int)m_targetButtons.size(); ++i)
	{
		m_drawPositions[i] = drawPos;
		m_drawPositions[i].x += i * buttonSpacing;
	}

	//次フレームの判定用に現在の入力状態を保存しておく
	if (g_pad[0]->IsPress(enButtonA))m_lastPressedButton = ButtonType::A;
	else if (g_pad[0]->IsPress(enButtonB))m_lastPressedButton = ButtonType::B;
	else if (g_pad[0]->IsPress(enButtonX))m_lastPressedButton = ButtonType::X;
	else if (g_pad[0]->IsPress(enButtonY))m_lastPressedButton = ButtonType::Y;
	else m_lastPressedButton = ButtonType::None;
}

void QTEButton::Input()
{
	//押されたボタンを取得
	ButtonType pressed = GetPressedButton();

	//何も押されていなければ処理しない
	if (pressed == ButtonType::None)return;

	// ボタンが押されたらクールタイムをセット（連打防止）
	m_inputCooldown = COOLDOWN_TIME;

	//現在のステップの正解ボタンと比較
	if (pressed == m_targetButtons[m_currentStep])
	{
		m_currentStep++;
		//全てのボタンが正しく入力されたら成功
		if (m_currentStep >= (int)m_targetButtons.size())
		{
			m_isSuccess = true;
			m_isFinished = true;

			if (m_qteTimerUI != nullptr)
			{
				DeleteGO(m_qteTimerUI);
				m_qteTimerUI = nullptr;
			}
		}
	}
	else
	{
		//異なるボタンが押されたら即座に失敗
		m_isSuccess = false;
		m_isFinished = true;

		if (m_qteTimerUI != nullptr)
		{
			DeleteGO(m_qteTimerUI);
			m_qteTimerUI = nullptr;
		}
	}
}

ButtonType QTEButton::GetPressedButton()const
{
	ButtonType currentPressed = ButtonType::None;
	if (g_pad[0]->IsPress(enButtonA))currentPressed = ButtonType::A;

	else if (g_pad[0]->IsPress(enButtonB))currentPressed = ButtonType::B;

	else if (g_pad[0]->IsPress(enButtonX))currentPressed = ButtonType::X;

	else if (g_pad[0]->IsPress(enButtonY))currentPressed = ButtonType::Y;

	//今押されているかつ、前回は押されていなかったときのみ入力を許可する
	//これにより長押しを無視し、1回押すたびに1入力としてカウントする
	if (currentPressed != ButtonType::None && currentPressed != m_lastPressedButton)
	{
		return currentPressed;
	}

	return ButtonType::None;
}

void QTEButton::SetPosition(const Vector3& pos)
{
	m_position = pos;
	// 個別のRenderへも反映
	m_AbuttonRender.SetPosition(pos);
	m_BbuttonRender.SetPosition(pos);
	m_XbuttonRender.SetPosition(pos);
	m_YbuttonRender.SetPosition(pos);
}

void QTEButton::Render(RenderContext& rc)
{
	//未入力のボタンのみを順番に描画
	if (m_isFinished) return;

	for (int i = 0; i < (int)m_targetButtons.size(); ++i)
	{
		if (i < m_currentStep) continue;

		m_buttonSprites[i]->SetPosition(m_drawPositions[i]);
		m_buttonSprites[i]->Update();
		m_buttonSprites[i]->Draw(rc);
	}
}