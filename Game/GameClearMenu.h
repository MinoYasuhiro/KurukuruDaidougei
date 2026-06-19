#pragma once

enum class ClearMenuResult
{
    None,
    Retry,
    BTitle
};

class GameClearMenu : public IGameObject
{
public:
    bool Start() override;
    void Update() override;
    void Render(RenderContext& rc) override;

    ClearMenuResult GetResult() const { return m_result; }

private:
    void Input();
    void SelectScale();

    SpriteRender m_retryRender;
    SpriteRender m_titleRender;

    ClearMenuResult m_result = ClearMenuResult::None;

    int m_selectIndex = 0;

    float m_inputTimer = 0.0f;
    const float m_inputInterval = 0.15f;
};


