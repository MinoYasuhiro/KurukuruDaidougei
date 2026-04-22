#include "stdafx.h"
#include "Circle.h"

bool Circle::Start()
{
	//円のモデル読み込み
	m_modelRender.Init("Assets/modelData/circle.tkm");
	//モデルに位置を反映
	m_modelRender.SetPosition(m_position);

	return true;
}

void Circle::SetPosition(const Vector3& position)
{
	//メンバ変数に位置を保存
	m_position = position;
	
	//モデル描画クラスにも位置を反映
	m_modelRender.SetPosition(m_position);
}

void Circle::Update()
{
	m_modelRender.Update();
}

void Circle::Render(RenderContext& rc)
{
	//非表示フラグが立っていると描画しない
	if (!m_isVisible)return;

	m_modelRender.Draw(rc);
}