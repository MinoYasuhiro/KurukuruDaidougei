#pragma once

class Player;

class WaterPuddle : public IGameObject
{
public:
    WaterPuddle();
    ~WaterPuddle();

    bool Start();
    void Update();
    void Render(RenderContext& rc);

    void SetPosition(const Vector3& pos);

    ModelRender m_model;

private:
    

    Vector3 m_position;

    float m_radius = 80.0f;

    Player* m_player = nullptr;

    bool m_playerInside = false;
};


