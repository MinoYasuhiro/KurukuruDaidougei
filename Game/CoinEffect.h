#pragma once
class CoinEffect:public IGameObject
{
public:
	CoinEffect() {};
	~CoinEffect() {};

	bool Start()override;
	void Update()override;
	void Render(RenderContext& renderContext)override;

	void Play();

private:
	struct Coin
	{
		Vector3 position;
		Vector3 speed;
		bool active;
	};

	static const int COIN_MAX = 20;

	Coin m_coins[COIN_MAX];

	SpriteRender m_coin[COIN_MAX];
	int m_nextIndex = 0;
};

