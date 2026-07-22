#pragma once

enum class  LoadStep
{
	Start,
	CreateGame,
	WaitGameCreate,
	Finish,
};

class LoadingManager;
class Loading : public IGameObject
{
public:
	Loading() {}
	~Loading() {}
	bool Start();
	void Update();
	void Render(RenderContext& renderContext);

public:
	///////// LodingUI関連の処理////////

	/// <summary>
	/// Loading開始から終了までのメータ処理
	/// </summary>
	void LoadingMate();

	/// <summary>
	/// メーターを時間計測に沿って伸ばしていく処理
	/// </summary>
	void UpdateBar();

	/// <summary>
	/// Loading文字を点滅表示する処理
	/// </summary>
	void LoadingLetter();


	void SetProgress(float progress);
	void UpdateLoading();




private:
	LoadStep m_loadStep = LoadStep::Start;
	Vector3 m_position;
	LoadingManager* m_loadingManager = nullptr;
	SpriteRender m_Sprite;
	SpriteRender m_CircleRender;
	SpriteRender m_baseBar;
	SpriteRender m_fillBar;
	SpriteRender m_Letter;
	FontRender m_font;
	std::unique_ptr<SpriteRender> m_lightSprite;

	/// <summary>
	/// 開始時間を記録
	/// </summary>
	//std::chrono::steady_clock::time_point m_startTime;

private:


	float m_angle = 0.0f;
	float m_barWidth = 580.0f;
	float m_elapsedTime = 0.0f;   // 経過時間
	const float m_totalTime = 5.0f;     // ロード完了までの時間（秒）
	float m_progress = 0.0f;      // 0.0～1.0
	bool  m_isFinished = false;
	float radius = 350.0f;


	//LoadingPhase m_phase = LoadingPhase::Gear1;

	float m_animTimer = 0.0f;
	float m_animInterval = 0.2f;

	////////遅延、待機時間処理////////

	float m_finishTimer = 0.0f;
	const float m_finishWaitTime = 1.0f; // 1秒待つ
	bool m_isWaiting = false;
	float m_displayProgress = 0.0f; // 0.0～1.0

};




