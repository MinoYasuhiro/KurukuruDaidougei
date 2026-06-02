#pragma once
enum class CoinLevel
{
	Empty,		//空(箱のみ)
	Small,		//少量
	Medium,		//中量
	Large,		//大量
	ExtraLarge,	//特大量
	Max			//最大量
};
class CoinBox :public IGameObject
{
public:
	CoinBox();
	~CoinBox();
	bool Start()override;
	void Update()override;
	void Render(RenderContext& renderContext)override;

	//コインを1段階追加
	void AddCoin();

	//コインの状態をリセット(空にする)
	void Reset();

private:
	ModelRender m_coinBoxRender;		//空
	ModelRender m_smallCoinRender;		//小
	ModelRender m_mediumCoinRender;		//中
	ModelRender m_largeCoinRender;		//大
	ModelRender m_extraLargeCoinRender;	//特大
	ModelRender m_maxCoinRender;		//最大

	//位置
	Vector3 m_position = Vector3::Zero;

	//現在のコイン状態
	CoinLevel m_coinLevel = CoinLevel::Empty;
};

