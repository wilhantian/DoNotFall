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
	/*获取当前排名*/
	static void updateRank(cocos2d::LabelTTF* label, int score);

	static bool saveJsonFile(std::string filename, const char* jsonContext);
	 /** 读取本地文件，返回数据 */
	static const char* getFileByName(std::string pFileName);

	/*更新排行榜*/
	static void updateFillBoard();

	/*预加载插屏广告*/
	static void loadSpotAds();
	/*显示插屏广告*/
	static void showSpotAds();

	/*检测应用更新*/
	static void autoUpdate();

};

#endif
