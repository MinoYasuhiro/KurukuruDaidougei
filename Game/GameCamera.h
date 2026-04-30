#pragma once
#include "Math.h"
#include "Game.h"


enum class CameraMode
{
	StartView,        // 開始：正面・舞台全体
	MoveView,         // 移動フェーズ：引き
	UmbrellaBehind,   // 傘回し：背後
};

class Player;
class GameCamera : public IGameObject
{
public:
	GameCamera() {}
	~GameCamera() {}

	bool Start() override;
	void Update() override;
	void UpdateTestZoom();
	void Reset();
private:
	// --- 基本 ---
	Player* m_player = nullptr;
	Vector3 m_toCameraPos = Vector3::Zero;

	// --- フェーズ管理 ---
	GamePhase m_currentPhase;

	// --- ズーム補間 ---
	bool    m_isZooming = false;
	float   m_zoomT = 0.0f;
	Vector3 m_zoomFromOffset;
	Vector3 m_zoomToOffset;
	
	//----成功時に引き寄せる用のカメラ管理----

    // テスト用ズーム制御
	bool  m_isTestZoom = false;
	float m_testZoomTimer = 0.0f;


private:
	/// <summary>
	/// プレイヤー参照用の関数
	/// </summary>
	/// <returns></returns>
	bool UpdatePlayer();
	/// <summary>
	/// ゲームフェーズ変化への監視
	/// </summary>
	void UpdatePhase();
	/// <summary>
    /// ズーム補間処理
    /// </summary>
	void UpdateZoom();
	/// <summary>
	/// 実際にカメラへ反映
	/// </summary>
	void ApplyCamera();
	/// <summary>
    /// カメラを寄せる（ズームイン）成功時のシーンや傘を回し終わった時とかに使ってください
    /// </summary>
	void ZoomIn();

	// フェーズ別処理
	void MoveCameraForMovePhase();
	void MoveCameraBehindPlayer();
	/// <summary>
/// カメラのイージング処理
/// </summary>
/// <param name="t"></param>
/// <returns></returns>
	static float EaseInOutCubic(float t);

};

