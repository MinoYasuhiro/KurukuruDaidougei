#pragma once
class CoinEffect :public IGameObject
{
public:
	CoinEffect() {};
	~CoinEffect() {};

	bool Start()override;
	void Update()override;
	void Render(RenderContext& renderContext)override;

	//エフェクトを発生させる
	void Play();

private:

	//コイン１枚分の状態データ
	struct Coin
	{
		Vector3 position;	//位置
		Vector3 speed;		//速度
		bool active;		//使用中かどうか(表示するか)
	};

	//同時に扱うコインの最大数
	static const int COIN_MAX = 20;

	//コインの状態配列(ロジック側)
	Coin m_coins[COIN_MAX];

	SpriteRender m_coin[COIN_MAX];

	//次に使用するインデックス(使いまわし用)
	int m_nextIndex = 0;

	//現在表示中のコインのインデックス一覧
	//Update/Render対象を減らして高速化するために使用
	std::vector<int>m_activeList;
};

