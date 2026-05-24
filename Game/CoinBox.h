#pragma once
enum class CoinLevel
{
	Empty,
	Small,
	Medium,
	Large,
	ExtraLarge,
	Max
};
class CoinBox :public IGameObject
{
public:
	CoinBox();
	~CoinBox();
	bool Start()override;
	void Update()override;
	void Render(RenderContext& renderContext)override;

private:
	ModelRender m_coinBoxRender;
	ModelRender m_smallCoinRender;
	ModelRender m_mediumCoinRender;
	ModelRender m_largeCoinRender;
	ModelRender m_extraLargeCoinRender;
	ModelRender m_maxCoinRender;
	Vector3 m_position = Vector3::Zero;
	CoinLevel m_coinLevel = CoinLevel::Empty;
	float m_testTime = 0.0f;
};

