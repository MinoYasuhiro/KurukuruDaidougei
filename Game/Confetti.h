#pragma once
class Confetti :public IGameObject
{
public:
	Confetti() {};
	~Confetti() {};

	bool Start()override;
	void Update()override;
	void Render(RenderContext& renderContext)override;

	void Reset();

	//エフェクトを発生させる
	void Play();

private:
	//紙吹雪1枚分のデータ
	struct ConfettiParticle
	{
		Vector3 position;	//位置
		Vector3 speed;		//速度
		float rotation;		//回転角度
		float rotSpeed;		//回転速度
		int type;			//種類
		bool active;		//有効フラグ(表示するか)
	};

	//最大数
	static const int CONFETTI_MAX = 300;
	//種類数
	static const int TYPE_NUM = 4;

	//紙吹雪データ配列
	ConfettiParticle m_confettis[CONFETTI_MAX];

	//描画用スプライト
	SpriteRender m_confetti[CONFETTI_MAX];

	//次に使うインデックス(再利用用)
	int m_nextIndex = 0;

	//使用中の紙吹雪インデックス
	std::vector<int>m_activeList;

	//まだ生成していない残りの数
	int m_spawnCount = 0;
	//1フレーム当たりの生成数
	int m_spawnPerFrame = 20;
};

