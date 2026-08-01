#include "stdafx.h"
#include "Circle.h"

bool Circle::Start()
{
	m_effect.Init(0);

	// 位置の設定
	m_effect.SetPosition(m_position);
	m_effect.SetScale(Vector3(22.92f, 22.92f, 22.92f)); // 必要に応じてサイズ調整

	// 初期表示状態に合わせて再生
	if (m_isVisible)
	{
		m_effect.Play();
	}

	return true;
}

void Circle::SetPosition(const Vector3& position)
{
	//メンバ変数に位置を保存
	m_position = position;

	m_effect.SetPosition(m_position);
}

void Circle::SetVisible(bool visible)
{
	// 状態が変わった時だけ再生/停止を切り替え
	if (m_isVisible != visible)
	{
		m_isVisible = visible;
		if (m_isVisible)
		{
			m_effect.Play(); // 表示時は再生
		}
		else
		{
			m_effect.Stop(); // 非表示時は停止
		}
	}
}

void Circle::Update()
{
	m_effect.SetPosition(m_position);
	m_effect.Update();
}

void Circle::Reset()
{
	m_isVisible = false;
	m_position = { 0.0f,-10.0f,0.0f };
	m_effect.SetPosition(m_position);
}
