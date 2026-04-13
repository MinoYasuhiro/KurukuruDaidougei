#pragma once
#include "Math.h"
#include "Game.h"

class Player;

class GameCamera : public IGameObject
{
public:
	GameCamera() {}
	~GameCamera() {}

	bool Start() override;
	void Update() override;

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

private:
	// Update 内分割
	bool UpdatePlayer();
	void UpdatePhase();
	void UpdateZoom();
	void ApplyCamera();

	// フェーズ別処理
	void MoveCameraForMovePhase();
	void MoveCameraBehindPlayer();

	static float EaseInOutCubic(float t);
};

