//後々追加
#include "stdafx.h"
#include "SoundSettings.h"
#include <fstream>

//初期値
float SoundSettings::Master = 1.0f;
float SoundSettings::BGM = 1.0f;
float SoundSettings::SE = 1.0f;

//設定を保存する
void SoundSettings::Save()
{
	//バイナリ形式でファイルを開く
	//↑テキスト形式と対比される用語で、テキストデータ以外のデータを意図した表現
	//ofstream→ファイルに書き込む
	std::ofstream ofs("SoundSettings.dat", std::ios::binary);

	//各音量を順番に書き込む
	ofs.write((char*)&Master, sizeof(float));
	ofs.write((char*)&BGM, sizeof(float));
	ofs.write((char*)&SE, sizeof(float));
}

//設定を読み込む
void SoundSettings::Load()
{
	//ファイルを開く
	//ifstream→ファイルから読み込む
	std::ifstream ifs("SoundSettings.dat", std::ios::binary);

	//ファイルが存在しなければ何もしない
	if (!ifs)return;

	//保存された音量を読み込む
	ifs.read((char*)&Master, sizeof(float));
	ifs.read((char*)&BGM, sizeof(float));
	ifs.read((char*)&SE, sizeof(float));
}