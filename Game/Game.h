#pragma once
#include "Level3DRender/LevelRender.h"

class BackGround;
class Player;
class GameCamera;
class Pause;
class QTE;
class BGMManager;
enum class GamePhase
{
	Start,
	MovePhase,   // 傘回し移動フェーズ
	AfterMove
};

enum class GameState
{

	Playing,
	Pause,
	GameOver,
	GameClear,
	SoundTest

	
};


class Game : public IGameObject
{
public:
	Game() {}
	~Game() {}
	bool Start();
	void Update();
	static GamePhase GetPhase();
	void Render(RenderContext& rc);
	
private:
	ModelRender m_modelRender;
	Vector3 m_pos;
	BackGround* m_backGround;
	Player* m_player;
	SpriteRender* m_SpriteRender;
	GameCamera* m_gameCamera;
	float m_movePhaseTimer = 0.0f;   // ★ 移動フェーズ経過時間
	float m_clearTimer = 0.0f;      // 経過時間
	bool  m_isGameClearShown = false; // すでに表示したか
	static GamePhase m_phase;

	static GameState m_gameState;
	static GameState GetState() { return m_gameState; }
	static void SetState(GameState state) { m_gameState = state; }

};

