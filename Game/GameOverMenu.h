#pragma once

enum class OverMenuResult
{
    None,
    Retry,
    Btitle
};

class GameOverMenu : public IGameObject
{
public:
    bool Start() override;
    void Update() override;
    void Render(RenderContext& rc) override;

    OverMenuResult GetResult() const { return m_result; }

private:
    void Input();
    void SelectScale();

    SpriteRender m_retryRender;
    SpriteRender m_titleRender;

    OverMenuResult m_result = OverMenuResult::None;

    int m_selectIndex = 0;

    float m_inputTimer = 0.0f;
    const float m_inputInterval = 0.15f;
};