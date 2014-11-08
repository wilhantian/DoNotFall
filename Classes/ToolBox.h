#ifndef _TOOL_BOX_H_
#define _TOOL_BOX_H_

#include "cocos2d.h"

class ToolBox
{
public:
	static cocos2d::LabelTTF* rank_label;
	static const char* getStringWithJsonFile(const char* file, const char* name, const char* va);
	static const char* getStrTitle();
	static char* getRankJson();
	/*��ȡ��ǰ����*/
	static void updateRank(cocos2d::LabelTTF* label, int score);

	static bool saveJsonFile(std::string filename, const char* jsonContext);
	 /** ��ȡ�����ļ����������� */
	static const char* getFileByName(std::string pFileName);

	/*�������а�*/
	static void updateFillBoard();

	/*Ԥ���ز������*/
	static void loadSpotAds();
	/*��ʾ�������*/
	static void showSpotAds();

	/*���Ӧ�ø���*/
	static void autoUpdate();

};

#endif
