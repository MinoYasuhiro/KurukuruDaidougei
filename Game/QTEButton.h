#pragma once
#include "ButtonType.h"
class QTEButton:public IGameObject
{
public:
	QTEButton();
	~QTEButton();
	bool Start()override;
	void Update()override;
	void Render(RenderContext& renderContext)override;

	void StartQTE(ButtonType targetButton,float limitTime);
	void Input();

	ButtonType GetPressedButton()const;

	void SetPosition(const Vector3& pos);

	bool IsFinished()const { return m_isFinished; }
	bool IsSuccess()const { return m_isSuccess; }

private:
	bool m_isFinished = false;
	bool m_isSuccess = false;

	float m_limitTime = 0.0f;
	float m_timer = 0.0f;

	ButtonType m_targetButton = ButtonType::None;

	SpriteRender m_AbuttonRender;
	SpriteRender m_BbuttonRender;
	SpriteRender m_YbuttonRender;
	SpriteRender m_XbuttonRender;
	
};

