#include "stdafx.h"
#include "GameOver.h"
#include "Title.h"


bool GameOver::Start()
{
	m_SpriteRender.Init("Assets/Sprite/GameOver.DDS", 1920, 1080);
	return true;
}

void GameOver::Update()
{

    m_SpriteRender.Update();

    if (g_pad[0]->IsTrigger(enButtonA))
    {
        NewGO<Title>(0, "title");
        DeleteGO(this);
    }

}

void GameOver::Render(RenderContext& rc)
{
	m_SpriteRender.Draw(rc);
}