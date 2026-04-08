#pragma once

enum class BallState
{
	Idle,			//出現待ち
	Flying,			//観客から飛んでくる
	OnUmbrella,		//傘に乗った
	Spinning,		//傘回し中
	FailFall,		//失敗時
	SuccessThrow,	//球を放る
};