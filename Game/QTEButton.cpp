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
	//各ボタンのスプライトを読み込み
	m_AbuttonRender.Init("Assets/sprite/Abutton.dds", 200.0f, 200.0f);
	m_BbuttonRender.Init("Assets/sprite/Bbutton.dds", 200.0f, 200.0f);
	m_YbuttonRender.Init("Assets/sprite/Ybutton.dds", 200.0f, 200.0f);
	m_XbuttonRender.Init("Assets/sprite/Xbutton.dds", 200.0f, 200.0f);

	return true;
}

void QTEButton::StartQTE(ButtonType target, float limitTime)
{
	m_targetButton = target;	//押すべきボタンを設定
	m_limitTime = limitTime;	//制限時間を設定
	m_timer = 0.0f;				//タイマー初期化
	m_isFinished = false;		//QTE未終了
	m_isSuccess = false;		//成功状態リセット
}

void QTEButton::Update()
{
	//すでに終了しているなら何もしない
	if (m_isFinished)return;

	//経過時間を加算
	m_timer += g_gameTime->GetFrameDeltaTime();

	//制限時間を超えたら失敗
	if (m_timer >= m_limitTime)
	{
		m_isSuccess = false;	//失敗
		m_isFinished = true;	//終了
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
	//押されたボタンを取得
	ButtonType pressed = GetPressedButton();

	//何も押されていなければ処理しない
	if (pressed == ButtonType::None)return;

	//正しいボタンかどうか判定
	m_isSuccess = (pressed == m_targetButton);

	//間違ったボタンを押した時点でQTE終了
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

void QTEButton::Render(RenderContext& rc)
{
	//指定されたターゲットボタンのみ表示
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