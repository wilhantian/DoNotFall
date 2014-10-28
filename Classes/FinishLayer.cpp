#include "FinishLayer.h"
#include "GameScene.h"
#include "HelloWorldScene.h"
#include "BillBoardScene.h"
#include "ToolBox.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
// 引入相关的头文件
#include "Cocos2dx/Common/CCUMSocialSDK.h"
#include "Cocos2dx/ShareButton/UMShareButton.h"
// 使用友盟命令空间
USING_NS_UM_SOCIAL;
#endif

USING_NS_CC;

/*
 * 分享回调, 该回调不是某个类的成员函数， 而是一个普通的函数, 具体使用参考HelloWorldScene的例子
* @param platform 要分享到的目标平台
 * @param stCode 返回码, 200代表分享成功, 100代表开始分享
 * @param errorMsg 分享失败时的错误信息,android平台没有错误信息
 */
void shareCallback(int platform, int stCode, std::string& errorMsg)
{
    if ( stCode == 100 )
    {
        log("#### Start Share");
    }
    else if ( stCode == 200 )
    {
    	log("#### Share Done");
    }
    else
    {
    	log("#### Share Error");
    }

    log("platform num is : %d.", platform);
}

bool FinishLayer::init(int score)
{
	if (!Layer::init())
	{
		return false;
	}
	FinishLayer::score = score;
	Size size(640, 960);
	//反黑背景
// 	bk = Sprite::create("UI/FinishLayer/bk.png");
// 	bk->setAnchorPoint(Vec2(0, 0));
// 	bk->setPosition(0, 0);
// 	bk->setOpacity(0);
// 	this->addChild(bk);
	Menu* bk_menu = Menu::create();
	bk_menu->setAnchorPoint(Vec2(0, 0));
	bk_menu->setPosition(0, 0);
	this->addChild(bk_menu);

	bk = MenuItemImage::create("UI/FinishLayer/bk.png", "UI/FinishLayer/bk.png", "", CC_CALLBACK_1(FinishLayer::menuVoid, this));
	bk->setAnchorPoint(Vec2(0, 0));
	bk->setPosition(0, 0);
	bk->setOpacity(0);
	bk_menu->addChild(bk);
	//上端背景
	top_bk = Sprite::create("UI/FinishLayer/top_bk.png");
	top_bk->setPosition(size.width / 2, size.height + top_bk->getContentSize().height/2);
	this->addChild(top_bk);
	//分数
	char str_score[20];
	sprintf(str_score, "%d", score);
	
	LabelTTF* text_score = LabelTTF::create();
	text_score->setAnchorPoint(Vec2(0.5, 0.5));
	text_score->setColor(Color3B(26, 26, 26));
	text_score->setString(str_score);
	text_score->setFontSize(28);
	text_score->setPosition(375, 400 - 140);
	top_bk->addChild(text_score);
	//最高分数
	if (score > UserDefault::getInstance()->getIntegerForKey("high_score", 0))
	{
		UserDefault::getInstance()->setIntegerForKey("high_score", score);
	}

	char str_score_high[20];
	sprintf(str_score_high, "%d", UserDefault::getInstance()->getIntegerForKey("high_score", 0));

	LabelTTF* text_score_high = LabelTTF::create();
	text_score_high->setAnchorPoint(Vec2(0.5, 0.5));
	text_score_high->setColor(Color3B(26, 26, 26));
	text_score_high->setString(str_score_high);
	text_score_high->setFontSize(28);
	text_score_high->setPosition(375, 400 - 193);
	top_bk->addChild(text_score_high);
	//全球排名
	LabelTTF* text_rank = LabelTTF::create();
	text_rank->setAnchorPoint(Vec2(0.5, 0.5));
	text_rank->setColor(Color3B(26, 26, 26));
	text_rank->setString("wait...");
	text_rank->setFontSize(28);
	text_rank->setPosition(375, 400 - 242);
	top_bk->addChild(text_rank);
	//更新排名
	updateRank(text_rank, score);
	///////////按钮////////////
	//按钮容器
	Menu* menu = Menu::create();
	menu->setAnchorPoint(Vec2(0, 0));
	menu->setPosition(0, 0);
	top_bk->addChild(menu);

	Size top_bk_size = top_bk->getContentSize();
	int x = (top_bk_size.width - 50 * 2) / 4;
	//主页 243-132 = 111
	MenuItemImage* but_home = MenuItemImage::create("UI/FinishLayer/home.png", "UI/FinishLayer/home2.png", "", CC_CALLBACK_1(FinishLayer::menuCallBackHome, this));
	but_home->setPosition(132, 75);
	menu->addChild(but_home);
	//继续
	MenuItemImage* but_play = MenuItemImage::create("UI/FinishLayer/play.png", "UI/FinishLayer/play2.png", "", CC_CALLBACK_1(FinishLayer::menuCallBackPlay, this));
	but_play->setPosition(132 + 111, 75);
	menu->addChild(but_play);
//分享 544c58defd98c51e8b055ad7
	MenuItemImage* but_share = MenuItemImage::create("UI/FinishLayer/share.png", "UI/FinishLayer/share2.png", "", CC_CALLBACK_1(FinishLayer::menuCallBackShare, this));
	//位置
	but_share->setPosition(132 + 222, 75);
	menu->addChild(but_share);
	//排名
	MenuItemImage* but_rank = MenuItemImage::create("UI/FinishLayer/rank.png", "UI/FinishLayer/rank2.png", "", CC_CALLBACK_1(FinishLayer::menuCallBackRank, this));
	but_rank->setPosition(132 + 333, 75);
	menu->addChild(but_rank);
	///////////执行动画///////////
	//背景显示动画
	FadeTo* fadeTo = FadeTo::create(0.15, 255);
	bk->runAction(fadeTo);
	//上端背景移动动画
	EaseBackOut* action = EaseBackOut::create(MoveTo::create(0.3, Vec2(640 / 2, 960 / 2)));
	Sequence* actionSequence = Sequence::createWithTwoActions(action, CallFuncN::create(CC_CALLBACK_1(FinishLayer::saveScene, this)));
	top_bk->runAction(actionSequence);

	return true;
}

void FinishLayer::saveScene(Node* node)
{
	//截屏
	std::string filename = "screen.png";
	filename = FileUtils::getInstance()->getWritablePath() + filename;
	log("%s", filename.c_str());

	utils::captureScreen([](bool b, const std::string name){
		//成功截屏后
		//显示广告
		ToolBox::showSpotAds();
		log("captureScreen filename:%s", name.c_str());
	}, filename);
}

FinishLayer* FinishLayer::create(int score)
{
	FinishLayer* layer = new FinishLayer();
	if (layer->init(score))
	{
		layer->autorelease();
		return layer;
	}
	else
	{
		delete layer;
		layer = NULL;
		return NULL;
	}
}

//主页键
void FinishLayer::menuCallBackHome(Ref* sender)
{
	ToolBox::rank_label = NULL;
	Scene* scene = TransitionSlideInL::create(0.15, HelloWorld::createScene());
	Director::getInstance()->replaceScene(scene);
}
//重玩键
void FinishLayer::menuCallBackPlay(Ref* sender)
{
	ToolBox::rank_label = NULL;
	Scene* scene = TransitionSlideInL::create(0.15, GameScene::createScene());
	Director::getInstance()->replaceScene(scene);
}
//分享键
void FinishLayer::menuCallBackShare(Ref* sender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	//todo
	// 获取一个CCUMSocialSDK实例
	CCUMSocialSDK *sdk = CCUMSocialSDK::create("544c58defd98c51e8b055ad7");
	// 设置友盟appkey,如果create中设置了不用调用该函数
	// sdk->setAppKey("4eaee02c527015373b000003");
	// **********************   设置平台信息  ***************************
	sdk->setQQAppIdAndAppKey("1103405242", "6q0dZx9M6mTMeNhc");
	// sdk->setWeiXinAppId("设置微信和朋友圈的app id");
	// sdk->setYiXinAppKey("设置易信和易信朋友圈的app id");
	// sdk->setLaiwangAppInfo("设置来往和来往动态的app id",
	//              "设置来往和来往动态的app key", "我的应用名");
	// sdk->setFacebookAppId("你的facebook appid");
	//     // 打开或者关闭log
	// sdk->setLogEnable(true) ;
	// ********************** END ***************************

	// 设置用户点击一条图文分享时用户跳转到的目标页面, 一般为app主页或者下载页面
	sdk->setTargetUrl("http://donotfall.bmob.cn");
	// 设置友盟分享面板上显示的平台
	vector<int>* platforms = new vector<int>();
	platforms->push_back(SINA);
	platforms->push_back(RENREN);
	platforms->push_back(DOUBAN);
	platforms->push_back(QZONE);
	platforms->push_back(QQ);
	// 设置平台, 在调用分享、授权相关的函数前必须设置SDK支持的平台
	sdk->setPlatforms(platforms);

	const char* text;
	text = ToolBox::getStringWithJsonFile("share.json", "context", "not find share.json");
	/*//想要动态获取分享内容修改这里即可
	if(UserDefault::getInstance()->getStringForKey("share_text", "null") == std::string("null"))
	{
		text = ToolBox::getStringWithJsonFile("share.json", "context", "not find share.json");
	}
	else
	{
		text = UserDefault::getInstance()->getStringForKey("share_text", "null").c_str();
	}
	*/
	log("-------> %s", text);
	// 打开分享面板, 注册分享回调, 参数1为分享面板上的平台, 参数2为要分享的文字内容，
	// 参数3为要分享的图片路径(android和IOS的图片地址格式不一致，因此分平台设置), 参数4为分享回调.
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//		sdk->openShare("要分享的文字内容", "/sdcard/image.png", share_selector(shareCallback));
		std::string filename = "screen.png";
		filename = FileUtils::getInstance()->getWritablePath() + filename;
		sdk->openShare(text, filename.c_str(), share_selector(shareCallback));
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		std::string filename = "screen.png";
		filename = FileUtils::getInstance()->getWritablePath() + filename;
		sdk->openShare(text, filename.c_str(), share_selector(shareCallback));
    #endif
#endif
}
//排行键
void FinishLayer::menuCallBackRank(Ref* sender)
{
	//todo
	Scene* scene = TransitionSlideInL::create(0.15, BillBoardScene::createScene(1));
	Director::getInstance()->pushScene(scene);
}
//无响应void
void FinishLayer::menuVoid(Ref* sender)
{

}

void FinishLayer::updateRank(LabelTTF* label, int score)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	ToolBox::updateRank(label, score);
#endif
}

