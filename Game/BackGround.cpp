#include "stdafx.h"
#include "BackGround.h"

namespace {

	/// <summary>
	/// ステージの大きさを拡大
	/// </summary>
	const Vector3 STEGE_SCALE = { 6.0f,2.0f,2.0f };
}

bool BackGround::Start()
{
	m_modelRender.Init("Assets/modelData/ground.tkm");
	m_modelRender.SetScale(Vector3(STEGE_SCALE));
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
	m_modelRender.Update();

	return true;
}

void BackGround::Update()
{
	
}

void BackGround::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}