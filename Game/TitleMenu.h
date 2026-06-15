#pragma once


enum class MenuResult
{
    None,
    GameStart,
    SoundOption,
    Operation,
    Rule
};

class TitleMenu : public IGameObject
{
public:
    TitleMenu() {}
    ~TitleMenu() {}

    bool Start() override;
    void Update() override;
    void Render(RenderContext& rc) override;

    MenuResult GetResult() const { return m_result; }

private:
    void Input();
    void SelectScale();

private:
    SpriteRender m_startRender;
    SpriteRender m_soundRender;
    SpriteRender m_operationRender;
    SpriteRender m_ruleRender;
    MenuResult m_result = MenuResult::None;
private:
    bool m_skipInputFrame = true;
    int m_selectIndex = 0;
 
    /// <summary>
    /// 入力防止タイマー
    /// </summary>
    float m_inputTimer = 0.0f;
    const float m_inputinterval = 0.2f; //押されてから0.2秒間動かない
};
