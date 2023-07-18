// VideoPlay.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include<Windows.h>
#include <iostream>
#include"vlc.h"
#include<conio.h>
std::string Unicode2Utf8(const std::wstring& strIn) {

	std::string str;
	int length = ::WideCharToMultiByte(CP_UTF8, 0, strIn.c_str(), strIn.size(), NULL, 0, NULL, NULL);
	str.resize(length + 1);
	::WideCharToMultiByte(CP_UTF8, 0, strIn.c_str(), strIn.size(), (LPSTR)str.c_str(), length, NULL, NULL);
	return str;
}

int main()
{
	int argc = 1;
	char* argv[2];
	argv[0] = (char*)"--ignore-config";//多字节转Unicode,再转utf-8
	libvlc_instance_t* vlc_ins = libvlc_new(argc,argv);
	//std::string path = Unicode2Utf8(L"不甘.mp4");
	//libvlc_media_t* media = libvlc_media_new_path(vlc_ins, path.c_str());
	std::string loca = Unicode2Utf8(L"file:///D:\\VSdm\\VideoPlay\\VideoPlay\\不甘.mp4");
	libvlc_media_t* media = libvlc_media_new_location(vlc_ins,loca.c_str());
	libvlc_media_player_t* player = libvlc_media_player_new_from_media(media);
	int c = 1;
	do {
		int ret = libvlc_media_player_play(player);
		if (ret == -1) {
			printf("error found!\r\n");
			break;
		}
		int vol = -1;
		//给media加载的时间,只有media解析完成 才会有下面的参数
		while (vol == -1) {
			Sleep(10);
			vol = libvlc_audio_get_volume(player);
		}
		libvlc_time_t tm= libvlc_media_player_get_length(player);//获取视频的播放总时长
		printf("%02d:%02d:%02d\r\n",int(tm/3600000),int((tm/60000)%60),int(tm/1000%60));
		int width = libvlc_video_get_width(player);//获取视频的宽度
		int height = libvlc_video_get_height(player);//获取视频的高度
		printf("width = %d  height = %d\r\n", width, height);

		while (c) {
			int plan{};
			
			while (!_kbhit()) {
				plan = int(100.0 * libvlc_media_player_get_position(player));
				printf("%d%%\r",plan);
				Sleep(500);
				if (libvlc_media_player_get_position(player) >= 1.0) {
					c = 0;
					break;
				}
			}

			char i = getchar();
			switch (i) {
			case 49://1 暂停播放
				libvlc_media_player_pause(player);
				break;
			case 50://2 开始播放
				libvlc_media_player_play(player);
				break;
			case 51://3 推出
				libvlc_media_player_stop(player);
				c = 0;
				break;
			case 52://4 设置进度
				libvlc_media_player_set_position(player, 0.96);
				break;
			case 53://5 设置音量+
				libvlc_audio_set_volume(player, vol + 10);
				vol = libvlc_audio_get_volume(player);
				break;
			case 54://6 设置音量-
				libvlc_audio_set_volume(player, vol - 10);
				vol = libvlc_audio_get_volume(player);
				break;
			}

		}
	} while (0);
	libvlc_media_player_release(player);
	libvlc_media_release(media);
	libvlc_release(vlc_ins);
	return 0;
}

