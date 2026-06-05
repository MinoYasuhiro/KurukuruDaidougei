#pragma once
class Arrow:public IGameObject
{
public:
	Arrow() {};
	~Arrow() {};

	bool Start()override;
	void Update()override;
	void Render(RenderContext& renderContext)override;

	//方向を計算
	void SetDirection(const Vector3& playerPos, const Vector3& targetPos);

	//外から制御する関数
	void SetActive(bool flag) { m_isActive = flag; }

private:
	//各方向の矢印のスプライト
	SpriteRender m_aboveArrow;
	SpriteRender m_underArrow;
	SpriteRender m_leftArrow;
	SpriteRender m_rightArrow;
	SpriteRender m_upperLeftArrow;
	SpriteRender m_upperRightArrow;
	SpriteRender m_lowerLeftArrow;
	SpriteRender m_lowerRightArrow;

	//表示位置
	Vector3 m_position = { 0.0f,200.0f,0.0f };

	//今表示する方向
	int m_currentIndex = 0;

	//表示するかどうか
	bool m_isActive = true;
};

