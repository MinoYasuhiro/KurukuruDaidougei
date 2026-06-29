#pragma once
class QTETimerUI:public IGameObject
{
public:
	QTETimerUI() {};
	~QTETimerUI() {};

	bool Start()override;
	void Update()override;
	void Render(RenderContext& renderContext)override;

	void StartQTE(float limitTime);		//タイマー起動関数
	bool IsTimeOut();					//時間切れ確認関数
	float GetProgress();				//進行度取得関数
	void SetProgress(float rate);		//割合を引数として受け取る関数

private:
	//時間を管理する変数
	float m_maximumTime = 0.0f;
	float m_remainingTime = 0.0f;
	bool m_activeFlag = false;

	//画面・画像を管理する変数
	float m_displayX = 0.0f;
	float m_displayY = 0.0f;
	float m_maximumWidth = 0.0f;
	SpriteRender m_frame;
	SpriteRender m_colorBars;
};

