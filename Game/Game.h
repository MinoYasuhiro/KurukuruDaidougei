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
enum class GamePhase
{
	Start,
	MovePhase,   // 傘回し移動フェーズ
	AfterMove
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


private:
	ModelRender m_modelRender;
	Vector3 m_pos;
	BackGround* m_backGround;
	Player* m_player;
	Umbrella* m_umbrella;
	SpriteRender* m_SpriteRender;
	SpriteRender m_startLetter;
	GameCamera* m_gameCamera;
	CoinBox* m_coinBox = nullptr;
	Item* m_item = nullptr;
	float m_movePhaseTimer = 0.0f;   // ★ 移動フェーズ経過時間
	float m_clearTimer = 0.0f;      // 経過時間
	float m_startTimer = 0.0f;
	bool m_showStart = true;
	bool  m_isGameClearShown = false; // すでに表示したか
	bool m_itemMove = false;
	static GamePhase m_phase;

	static GameState m_gameState;

	/// <summary>
	/// ゲームオーバー（仮）
	/// </summary>
	Vector3 m_prevPlayerPos;
	float   m_idleTimer = 0.0f;
	const float m_idleLimitTime = 8.0f; // 何秒でGameOverにするか

};