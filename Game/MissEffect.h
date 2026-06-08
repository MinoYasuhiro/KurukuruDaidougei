#pragma once
class MissEffect :public IGameObject
{
public:
	MissEffect() {};
	~MissEffect() {};

	bool Start()override;
	void Update()override;
	void Render(RenderContext& renderContext)override;

	void Reset();

	//エフェクトを発生させる
	void Play(int failCount);

private:

	//現在動いているオブジェクトのインデックス一覧
	//Update/Renderの対象を限定して処理負荷を軽減する
	std::vector<int>m_activeList;

	//最大同時表示数
	static const int MISS_MAX = 50;

	//投げられる物の種類
	enum ThrowType
	{
		Bucket,
		Basket,
		Colander,
		Fan,
		SakeBottle,
		Stone,
		Type_NUM
	};

	//一つ分のデータ
	struct ThrowData
	{
		Vector3 position;	//現在位置
		Vector3 speed;		//速度(移動方向・速さ)
		float rotation;		//回転角度
		bool active;		//表示中かどうか
		int type;			//表示する種類
	};

	//全オブジェクトの状態配列
	ThrowData m_objects[MISS_MAX];

	//描画用スプライト配列
	SpriteRender m_sprite[MISS_MAX];

	//次に使うインデックス
	int m_nextIndex;
};

