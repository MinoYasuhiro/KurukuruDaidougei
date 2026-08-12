#pragma once
#include "Level3DRender/LevelRender.h"

class BackGround;
class Player;
class GameCamera;
class Pause;
class Umbrella;
class QTE;
class QTEButton;
class BGMManager;
class CoinBox;
class Item;
class Circle;
class ItemSpawner;
class CoinEffect;
class AudienceManager;
class Confetti;
class MissEffect;
class WaterPuddle;
class QTERadialLines;
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

enum  EffectID
{
	EFFECT_QTE_RADIAL_LINES = 0,
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
	void RequestNormalSuccess();
	void RequestQTESuccess();
	void ShowQTERadialLines();
	void HideQTERadialLines();
	void StartClearAnimation()
	{
		m_isGameClearAnimation = true;
	}

	bool IsClearAnimation() const
	{
		return m_isGameClearAnimation;
	}

	Circle* GetCircle()const { return m_circle; }


	/// <summary>
	/// 直前のゲーム状態を保存する
	/// （画面遷移後に戻るために使用）
	/// </summary>
	/// <param name="state">保存する前の状態</param>
	static void SetPrevState(GameState state);


	/// <summary>
	/// 直前に保存されたゲーム状態を取得する
	/// （戻り先の画面判定に使用）
	/// </summary>
	/// <returns>直前のゲーム状態</returns>
	static GameState GetPrevState();
	bool m_fromTitleStart = false;

	//観客用
	bool m_QTESuccess30;

private:
	//ModelRender m_modelRender;
	Vector3 m_pos;
	BackGround* m_backGround;
	Player* m_player;
	Umbrella* m_umbrella;
	ItemSpawner* m_spawner = nullptr;
	SpriteRender* m_SpriteRender;
	AudienceManager* m_audienceManager;
	SpriteRender m_startLetter;
	SpriteRender m_failureLetter;
	SpriteRender m_successLetter;
	SpriteRender m_count1;
	SpriteRender m_count2;
	SpriteRender m_count3;
	SpriteRender m_countDown;
	SpriteRender m_AudienceSpriteRender;
	SpriteRender m_AudienceSpriteRender1;
	SpriteRender m_AudienceSpriteRender2;
	SkyCube* m_skyCube;
	GameCamera* m_gameCamera;
	WaterPuddle* m_waterPuddle1 = nullptr;
	WaterPuddle* m_waterPuddle2 = nullptr;
	WaterPuddle* m_waterPuddle3 = nullptr;
	CoinEffect* m_coin = nullptr;
	CoinBox* m_coinBox = nullptr;
	Item* m_item = nullptr;
	Circle* m_circle = nullptr;
	BGMManager* m_BGM = nullptr;
	Confetti* m_confetti = nullptr;
	MissEffect* m_missEffect = nullptr;
	QTERadialLines* m_qteRadialLines = nullptr;
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
	static GameState m_prevState;
	static GameState m_gameState;
	bool m_requestStart = false;
	bool m_isStarting = false;
	bool m_isGameClearAnimation = false;
	/// <summary>
	/// ゲームオーバー（仮）
	/// </summary>
	Vector3 m_prevPos = Vector3::Zero;
	float   m_idleTimer = 0.0f;
	const float m_idleLimitTime = 8.0f; // 何秒でGameOverにするか
	int AudienceImage;
	int m_phaseStep = 0;
	// 0: 通常傘回し
	// 1: 特殊傘回し
	// 2: QTE
	
	// ======================================
	// ■ QTEフェーズ用
	// ======================================
	QTEButton* m_button;				//現在表示中のQTEボタンUIへのポインタ
	float m_qteTimer = 0.0f;			//QTE中の経過時間計測用
	const float m_qteLimitTime = 3.0f;	//QTEの制限時間

	bool  m_qteStarted = false;			//QTEが開始されたかどうか
	bool m_waitQTEResult = false;		//QTE入力を受け付けて結果待ちの状態か
	bool m_qteResultSuccess = false;	//QTEに成功したかどうか
	bool m_isSuccessPending = false;	//成功演出を予約中か

	//3カウント用
	float m_countTimer = 0.0f;
	bool m_isCounting = false;
	int m_countNumber;

	bool m_coinPlayed = false;

	int m_failCount = 0;

	//観客用
	//観客用
	bool m_isSuccess = false;
	float m_successTimer2 = 0.0f;
	float m_time = 0.0f;
	bool m_bigSuccess = false;
	

	Vector3 m_basePos1;
	Vector3 m_basePos2;
	Vector3 m_basePos3;
	Vector3 m_basePos4;
	Vector3 m_basePos5;
	Vector3 m_basePos6;
	Vector3 m_basePos7;
	Vector3 m_basePos8;

	SpriteRender m_sprite1;
	SpriteRender m_sprite2;
	SpriteRender m_sprite3;
	SpriteRender m_sprite4;
	SpriteRender m_sprite5;
	SpriteRender m_sprite6;
	SpriteRender m_sprite7;
	SpriteRender m_sprite8;
};