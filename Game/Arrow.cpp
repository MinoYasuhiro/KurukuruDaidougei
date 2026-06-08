#include "stdafx.h"
#include "Arrow.h"

bool Arrow::Start()
{
	//各方向の矢印スプライトの読み込み
	m_aboveArrow.Init("Assets/sprite/AboveArrow.dds", 100.0f, 50.0f);
	m_underArrow.Init("Assets/sprite/UnderArrow.dds", 100.0f, 50.0f);
	m_leftArrow.Init("Assets/sprite/LeftArrow.dds", 100.0f, 50.0f);
	m_rightArrow.Init("Assets/sprite/RightArrow.dds", 100.0f, 50.0f);
	m_upperLeftArrow.Init("Assets/sprite/UpperLeftArrow.dds", 100.0f, 50.0f);
	m_upperRightArrow.Init("Assets/sprite/UpperRightArrow.dds", 100.0f, 50.0f);
	m_lowerLeftArrow.Init("Assets/sprite/LowerLeftArrow.dds", 100.0f, 50.0f);
	m_lowerRightArrow.Init("Assets/sprite/LowerRightArrow.dds", 100.0f, 50.0f);

	//表示される位置
	m_aboveArrow.SetPosition(m_position);
	m_underArrow.SetPosition(m_position);
	m_leftArrow.SetPosition(m_position);
	m_rightArrow.SetPosition(m_position);
	m_upperLeftArrow.SetPosition(m_position);
	m_upperRightArrow.SetPosition(m_position);
	m_lowerLeftArrow.SetPosition(m_position);
	m_lowerRightArrow.SetPosition(m_position);

	return true;
}

void Arrow::Update()
{
	m_aboveArrow.Update();
	m_underArrow.Update();
	m_leftArrow.Update();
	m_rightArrow.Update();
	m_upperLeftArrow.Update();
	m_upperRightArrow.Update();
	m_lowerLeftArrow.Update();
	m_lowerRightArrow.Update();
}

//プレイヤーから円への方向を計算し、どの矢印を表示するか決める
void Arrow::SetDirection(const Vector3& playerPos, const Vector3& targetPos)
{
	//方向ベクトル
	Vector3 dir = targetPos - playerPos;
	dir.y = 0.0f;

	//円の中に入っていたら何もしない
	if (dir.LengthSq() < 0.0001f)
	{
		m_isActive = false;
		return;
	}
	//ベクトルの正規化
	dir.Normalize();

	//カメラ基準ベクトル
	Vector3 camForward = g_camera3D->GetForward();
	Vector3 camRight = g_camera3D->GetRight();

	camForward.y = 0.0f;
	camRight.y = 0.0f;

	camForward.Normalize();
	camRight.Normalize();

	//カメラ空間に変換
	float x = dir.Dot(camRight);
	float y = dir.Dot(camForward);

	//画面基準の角度
	float angle = atan2f(x, y);

	//度に変換
	float deg = Math::RadToDeg(angle);

	//0～360に正規化
	deg = fmodf(deg + 360.0f, 360.0f);

	//8方向に変換
	m_currentIndex = (int)((deg / 45.0f) + 0.5f) % 8;

	//正しく方向が計算できたら表示をONにする
	m_isActive = true;
}

void Arrow::Reset()
{
	m_isActive = false;
	m_currentIndex = -1;
}

void Arrow::Render(RenderContext& rc)
{
	//非表示、または方向が計算されていないなら何もしない
	if (!m_isActive || m_currentIndex == -1)return;

	//計算された方向に応じて該当する矢印を描画
	switch (m_currentIndex)
	{
	case 0:
		m_aboveArrow.Draw(rc);
		break;
	case 1:
		m_upperRightArrow.Draw(rc);
		break;
	case 2:
		m_rightArrow.Draw(rc);
		break;
	case 3:
		m_lowerRightArrow.Draw(rc);
		break;
	case 4:
		m_underArrow.Draw(rc);
		break;
	case 5:
		m_lowerLeftArrow.Draw(rc);
		break;
	case 6:
		m_leftArrow.Draw(rc);
		break;
	case 7:
		m_upperLeftArrow.Draw(rc);
		break;
	}
}