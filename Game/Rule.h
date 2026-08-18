#pragma once
class Fade;
class SEManager;
class Rule : public IGameObject
{
public:
	Rule() {}
	~Rule();
	bool Start();
	void Update();
	void Render(RenderContext& renderContext);
	void UpdateAnimation();
	void UpdateInput();
	void UpdateExit();

private:
   /// <summary>
   /// ボタン
   /// </summary>
	SpriteRender m_LeftButtonRender;
	SpriteRender m_RightButtonRender;

private:
	Vector3 m_position;
	bool m_isEnding = false;
    float m_time = 0.0f;
	int m_pageIndex = 0;
	int m_maxPage = 7; // 7枚


	/// <summary>
  /// 入力防止タイマー
  /// </summary>
	float m_inputTimer = 0.0f;
	const float m_inputInterval = 0.15f; //押されてから0.15秒間動かない
	std::vector<SpriteRender*> m_rules;




};

