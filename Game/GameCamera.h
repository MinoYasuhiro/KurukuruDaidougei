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
	void UpdateTestZoom();
	void UpdateQTECamera();
	void MoveCameraForMovePhase();
	void MoveCameraBehindPlayer();
	void Reset();

private:
	Player* m_player = nullptr;
	Vector3 m_toCameraPos = Vector3::Zero;

	GamePhase m_currentPhase;

	bool    m_isZooming = false;
	float   m_zoomT = 0.0f;
	Vector3 m_zoomFromOffset;
	Vector3 m_zoomToOffset;

	bool  m_isTestZoom = false;
	float m_testZoomTimer = 0.0f;
	bool m_useElastic = false;
	bool m_hasRotated = false;

private:
	bool UpdatePlayer();
	void UpdatePhase();
	void UpdateZoom();
	void ApplyCamera();
	void ZoomIn();

	

	static float EaseInOutCubic(float t);
	static float EaseOutElastic(float t);
};
