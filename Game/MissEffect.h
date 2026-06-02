#pragma once
class MissEffect:public IGameObject
{
public:
	MissEffect() {};
	~MissEffect() {};

	bool Start()override;
	void Update()override;
	void Render(RenderContext& renderContext)override;

	//エフェクトを発生させる
	void Play(int failCount);

private:

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
		Vector3 position;
		Vector3 speed;
		float rotation;
		bool active;
		int type;
	};

	//本体データ
	ThrowData m_objects[MISS_MAX];

	SpriteRender m_sprite[MISS_MAX];

	//次に使うインデックス
	int m_nextIndex;
};

