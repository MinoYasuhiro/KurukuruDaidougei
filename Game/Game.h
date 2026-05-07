#pragma once
#include "Level3DRender/LevelRender.h"

class BackGround;
class Player;
class GameCamera;
class Pause;
class Umbrella;
class QTE;
class BGMManager;
class CoinBox;
class Item;
class Circle;
enum class GamePhase
{
	Start,
	MovePhase,   // 傘回し移動フェーズ
	AfterMove,   //傘回しフェーズ
	SpecialMove, //傘回し特殊フェーズ
	QTEMove  //傘回しQTEフェーズ
};

enum class GameState
{
	Title,
	Playing,
	Pause,
	GameOver,
	GameClear,
	SoundTest
};


class Game : public IGameObject
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
	static GamePhase GetPhase();
	void Render(RenderContext& rc);
	void RequestGameOver();
	static void SetState(GameState state) { m_gameState = state; }
	static GameState GetState() { return m_gameState; }
	void RequestMovePhase();
	void ResetGame();
	void RequestGameClear();
	void RequestTitle();
	void UpdatePlaying();
	void RequestFailureLetter();
	void RequestSuccessLetter();

	Circle* GetCircle()const { return m_circle; }

private:
	ModelRender m_modelRender;
	Vector3 m_pos;
	BackGround* m_backGround;
	Player* m_player;
	Umbrella* m_umbrella;
	SpriteRender* m_SpriteRender;
	SpriteRender m_startLetter;
	SpriteRender m_failureLetter;
	SpriteRender m_successLetter;
	GameCamera* m_gameCamera;
	CoinBox* m_coinBox = nullptr;
	Item* m_item = nullptr;
	Circle* m_circle = nullptr;
	BGMManager* m_BGM = nullptr;
	float m_movePhaseTimer = 0.0f;   // ★ 移動フェーズ経過時間
	float m_clearTimer = 0.0f;      // 経過時間
	float m_startTimer = 0.0f;
	float m_failureTimer = 0.0f;
	float m_successTimer = 0.0f;
	bool m_showStart = true;
	bool m_showFailure = false;
	bool m_showSuccess = false;
	bool  m_isGameClearShown = false; // すでに表示したか
	bool m_itemMove = false;
	bool m_hasThrownItem = false;
	bool m_isResetting = false;
	static GamePhase m_phase;

	static GameState m_gameState;
	bool m_requestStart = false;
	bool m_isStarting = false;
	/// <summary>
	/// ゲームオーバー（仮）
	/// </summary>
	Vector3 m_prevPos = Vector3::Zero;
	float   m_idleTimer = 0.0f;
	const float m_idleLimitTime = 8.0f; // 何秒でGameOverにするか
	int m_phaseStep = 0;
	// 0: 通常傘回し
	// 1: 特殊傘回し
	// 2: QTE
	
	// ======================================
	// ■ QTEフェーズ用
	// ======================================
	QTE* m_qte = nullptr;
	float m_qteTimer = 0.0f;
	const float m_qteLimitTime = 3.0f;
	bool  m_qteStarted = false;
};