#pragma once
class Player;
class Audience :public IGameObject
{
public:
	Audience();
	~Audience();

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;
	void DecideAnimation();
	void SetPosition(const Vector3& pos);

	void SetRotation(const Quaternion& rot);

public:
	ModelRender m_modelRender;

	enum EnAudienceAnimationState {	//アニメーションの状態
		enAudienceAnimationState_Idle,
		enAudienceAnimationState_Clap,
		enAudienceAnimationState_Clap2,
		enAudienceAnimationState_Clap3,
		enAudienceAnimationState_Clap4,
		enAudienceAnimationState_Clap5,
		enAudienceAnimationState_Clap6,
		enAudienceAnimationState_Happy,
		enAudienceAnimationState_Sippai1,
		enAudienceAnimationState_Sippai2,
		enAudienceAnimationState_Num,
	};
	AnimationClip m_audienceAnimationState[enAudienceAnimationState_Num];

	Player* m_player = nullptr;

	int m_currentAudienceAnim = 0;
	int m_prevPlayerState = -1;

	Vector3 m_position;

	Quaternion m_rotation;
};

