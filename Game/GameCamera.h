#pragma once
#include "Math.h"
#include "Game.h"

enum class CameraEaseType
{
    Cubic,
	Elastic,
    Back
};
class Player;
class ItemSpawner;
class Item;
class GameCamera : public IGameObject
{
public:
	GameCamera() {}
	~GameCamera() {}

	bool Start() override;
	void Update() override;
	//void UpdateTestZoom();
	void UpdateQTECamera();
	//void MoveCameraForMovePhase();
	//void MoveCameraBehindPlayer();
	void Reset();

	/// <summary>
	/// ゲームクリア時のカメラ演出更新
	/// </summary>
	void ClearCameraMove();

	/// <summary>
	/// ゲームオーバー時のカメラ演出更新
	/// </summary>
	void GameOverCameraMove();

private:
	Player* m_player = nullptr;
	Vector3 m_toCameraPos = Vector3::Zero;
	Vector3 m_qteCameraBasePos;
	GamePhase m_currentPhase;
	bool m_isEndingCamera = false;
	bool    m_isZooming = false;
	float   m_zoomT = 0.0f;
	Vector3 m_zoomFromOffset;
	Vector3 m_zoomToOffset;
	bool m_isEndingCameraFinished = false;
	CameraEaseType m_easeType
		= CameraEaseType::Cubic;
	Vector3 m_normalCameraPos =
	{

		0.0f,
		220.0f,
		-300.0f
	};

	Vector3 m_closeCameraPos =
	{
		0.0f,
		220.0f,
		-150.0f
	};

	Vector3 m_currentCameraPos =
	{
		0.0f,
		220.0f,
		-300.0f
	};

	bool  m_isTestZoom = false;
	float m_testZoomTimer = 0.0f;
	bool m_useElastic = false;
	bool m_hasRotated = false;
	float m_startCameraTimer = 0.0f;
	bool m_isStartCamera = true;
	ItemSpawner* m_spawner = nullptr;
	bool m_prevQTEActive = false;

private:
	bool UpdatePlayer();
	void UpdatePhase();
	void UpdateZoom();
	void ApplyCamera();
	void ZoomIn();

	

	static float EaseInOutCubic(float t);
	static float EaseOutElastic(float t);
	static float EaseOutBack(float t);
};
