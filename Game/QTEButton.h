#pragma once
#include "ButtonType.h"
class Game;
class QTETimerUI;
class SEManager;
class QTEButton :public IGameObject
{
public:
	QTEButton();
	~QTEButton();
	void Update()override;
	void Render(RenderContext& renderContext)override;

	//QTE開始
	void StartQTE(const std::vector<ButtonType>& targetButtons, float limitTime);
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

	std::vector<ButtonType> m_targetButtons;	//成功となるボタンのシーケンス
	int m_currentStep = 0;						//現在の進行度(入力済みのボタンの数)
	Vector3 m_drawPositions[10];				//各ボタンUIの画面上での描画座標
	Vector3 m_position;							//QTEUI全体の基準位置

	ButtonType m_lastPressedButton = ButtonType::None;	//長押しによる連続入力を防ぐための判定用変数

	//同じボタンが複数並ぶ場合でも個別に座標管理するため、固定メンバではなく動的配列として保持する
	std::vector<SpriteRender*>m_buttonSprites;
	//各ボタンUI
	SpriteRender m_AbuttonRender;
	SpriteRender m_BbuttonRender;
	SpriteRender m_YbuttonRender;
	SpriteRender m_XbuttonRender;

	float m_inputCooldown = 0.0f;		// クールタイム管理用
	const float COOLDOWN_TIME = 0.2f;	// 0.2秒間は次の入力を受け付けない

	QTETimerUI* m_qteTimerUI = nullptr;

	float m_successEffectTimer = 0.0f;
	// 光らせるボタン番号
	int m_effectIndex = -1;


	//追加
	float m_failureEffectTimer = 0.0f;
	int m_failureEffectIndex = -1;

};

