#pragma once
#include "ButtonType.h"
class QTEButton :public IGameObject
{
public:
	QTEButton();
	~QTEButton();
	bool Start()override;
	void Update()override;
	void Render(RenderContext& renderContext)override;

	//QTE開始
	void StartQTE(ButtonType targetButton, float limitTime);
	//入力処理
	void Input();

	//押されたボタン取得
	ButtonType GetPressedButton()const;

	//表示位置設定
	void SetPosition(const Vector3& pos);

	//QTEが終了しているか
	bool IsFinished()const { return m_isFinished; }
	//成功したかどうか
	bool IsSuccess()const { return m_isSuccess; }

private:
	bool m_isFinished = false;	//判定終了フラグ
	bool m_isSuccess = false;	//成功フラグ

	float m_limitTime = 0.0f;	//制限時間
	float m_timer = 0.0f;		//経過時間

	ButtonType m_targetButton = ButtonType::None;//正解のボタン

	//各ボタンUI
	SpriteRender m_AbuttonRender;
	SpriteRender m_BbuttonRender;
	SpriteRender m_YbuttonRender;
	SpriteRender m_XbuttonRender;

};

