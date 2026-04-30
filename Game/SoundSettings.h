//後々追加
#pragma once
class SoundSettings
{
public:
	static float Master;	//全体音量
	static float BGM;		//BGM音量
	static float SE;		//SE音量

	//現在の音量設定をファイルに保存
	static void Save();

	//保存されている音量設定を読み込む
	static void Load();
};

