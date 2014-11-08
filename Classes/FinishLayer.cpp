#include "FinishLayer.h"
#include "GameScene.h"
#include "HelloWorldScene.h"
#include "BillBoardScene.h"
#include "ToolBox.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
// ������ص�ͷ�ļ�
#include "Cocos2dx/Common/CCUMSocialSDK.h"
#include "Cocos2dx/ShareButton/UMShareButton.h"
// ʹ����������ռ�
USING_NS_UM_SOCIAL;
#endif

USING_NS_CC;

/*
 * ����ص�, �ûص�����ĳ����ĳ�Ա������ ����һ����ͨ�ĺ���, ����ʹ�òο�HelloWorldScene������
* @param platform Ҫ������Ŀ��ƽ̨
 * @param stCode ������, 200�������ɹ�, 100����ʼ����
 * @param errorMsg ����ʧ��ʱ�Ĵ�����Ϣ,androidƽ̨û�д�����Ϣ
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
	//���ڱ���
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
	//�϶˱���
	top_bk = Sprite::create("UI/FinishLayer/top_bk.png");
	top_bk->setPosition(size.width / 2, size.height + top_bk->getContentSize().height/2);
	this->addChild(top_bk);
	//����
	char str_score[20];
	sprintf(str_score, "%d", score);
	
	LabelTTF* text_score = LabelTTF::create();
	text_score->setAnchorPoint(Vec2(0.5, 0.5));
	text_score->setColor(Color3B(26, 26, 26));
	text_score->setString(str_score);
	text_score->setFontSize(28);
	text_score->setPosition(375, 400 - 140);
	top_bk->addChild(text_score);
	//��߷���
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
	//ȫ������
	LabelTTF* text_rank = LabelTTF::create();
	text_rank->setAnchorPoint(Vec2(0.5, 0.5));
	text_rank->setColor(Color3B(26, 26, 26));
	text_rank->setString("wait...");
	text_rank->setFontSize(28);
	text_rank->setPosition(375, 400 - 242);
	top_bk->addChild(text_rank);
	//��������
	updateRank(text_rank, score);
	///////////��ť////////////
	//��ť����
	Menu* menu = Menu::create();
	menu->setAnchorPoint(Vec2(0, 0));
	menu->setPosition(0, 0);
	top_bk->addChild(menu);

	Size top_bk_size = top_bk->getContentSize();
	int x = (top_bk_size.width - 50 * 2) / 4;
	//��ҳ 243-132 = 111
	MenuItemImage* but_home = MenuItemImage::create("UI/FinishLayer/home.png", "UI/FinishLayer/home2.png", "", CC_CALLBACK_1(FinishLayer::menuCallBackHome, this));
	but_home->setPosition(132, 75);
	menu->addChild(but_home);
	//����
	MenuItemImage* but_play = MenuItemImage::create("UI/FinishLayer/play.png", "UI/FinishLayer/play2.png", "", CC_CALLBACK_1(FinishLayer::menuCallBackPlay, this));
	but_play->setPosition(132 + 111, 75);
	menu->addChild(but_play);
//���� 544c58defd98c51e8b055ad7
	MenuItemImage* but_share = MenuItemImage::create("UI/FinishLayer/share.png", "UI/FinishLayer/share2.png", "", CC_CALLBACK_1(FinishLayer::menuCallBackShare, this));
	//λ��
	but_share->setPosition(132 + 222, 75);
	menu->addChild(but_share);
	//����
	MenuItemImage* but_rank = MenuItemImage::create("UI/FinishLayer/rank.png", "UI/FinishLayer/rank2.png", "", CC_CALLBACK_1(FinishLayer::menuCallBackRank, this));
	but_rank->setPosition(132 + 333, 75);
	menu->addChild(but_rank);
	///////////ִ�ж���///////////
	//������ʾ����
	FadeTo* fadeTo = FadeTo::create(0.15, 255);
	bk->runAction(fadeTo);
	//�϶˱����ƶ�����
	EaseBackOut* action = EaseBackOut::create(MoveTo::create(0.3, Vec2(640 / 2, 960 / 2)));
	Sequence* actionSequence = Sequence::createWithTwoActions(action, CallFuncN::create(CC_CALLBACK_1(FinishLayer::saveScene, this)));
	top_bk->runAction(actionSequence);

	return true;
}

void FinishLayer::saveScene(Node* node)
{
	//����
	std::string filename = "screen.png";
	filename = FileUtils::getInstance()->getWritablePath() + filename;
	log("%s", filename.c_str());

	utils::captureScreen([](bool b, const std::string name){
		//�ɹ�������
		//��ʾ���
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

//��ҳ��
void FinishLayer::menuCallBackHome(Ref* sender)
{
	ToolBox::rank_label = NULL;
	Scene* scene = TransitionSlideInL::create(0.15, HelloWorld::createScene());
	Director::getInstance()->replaceScene(scene);
}
//�����
void FinishLayer::menuCallBackPlay(Ref* sender)
{
	ToolBox::rank_label = NULL;
	Scene* scene = TransitionSlideInL::create(0.15, GameScene::createScene());
	Director::getInstance()->replaceScene(scene);
}
//�����
void FinishLayer::menuCallBackShare(Ref* sender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	//todo
	// ��ȡһ��CCUMSocialSDKʵ��
	CCUMSocialSDK *sdk = CCUMSocialSDK::create("544c58defd98c51e8b055ad7");
	// ��������appkey,���create�������˲��õ��øú���
	// sdk->setAppKey("4eaee02c527015373b000003");
	// **********************   ����ƽ̨��Ϣ  ***************************
	sdk->setQQAppIdAndAppKey("1103405242", "6q0dZx9M6mTMeNhc");
	sdk->setWeiXinAppId("wx756e474a6153c652");
	// sdk->setYiXinAppKey("�������ź���������Ȧ��app id");
	// sdk->setLaiwangAppInfo("����������������̬��app id",
	//              "����������������̬��app key", "�ҵ�Ӧ����");
	// sdk->setFacebookAppId("���facebook appid");
	//     // �򿪻��߹ر�log
	sdk->setLogEnable(false) ;
	// ********************** END ***************************

	// �����û����һ��ͼ�ķ���ʱ�û���ת����Ŀ��ҳ��, һ��Ϊapp��ҳ��������ҳ��
	sdk->setTargetUrl("http://donotfall.bmob.cn");
	// �������˷����������ʾ��ƽ̨
	vector<int>* platforms = new vector<int>();
	platforms->push_back(SINA);
	platforms->push_back(RENREN);
	platforms->push_back(DOUBAN);
	platforms->push_back(QZONE);
	platforms->push_back(QQ);
	// ����ƽ̨, �ڵ��÷�����Ȩ��صĺ���ǰ��������SDK֧�ֵ�ƽ̨
	sdk->setPlatforms(platforms);

	const char* text;
	text = ToolBox::getStringWithJsonFile("share.json", "context", "not find share.json");
	/*//��Ҫ��̬��ȡ���������޸����Ｔ��
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
	// �򿪷������, ע�����ص�, ����1Ϊ��������ϵ�ƽ̨, ����2ΪҪ������������ݣ�
	// ����3ΪҪ�����ͼƬ·��(android��IOS��ͼƬ��ַ��ʽ��һ�£���˷�ƽ̨����), ����4Ϊ����ص�.
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//		sdk->openShare("Ҫ�������������", "/sdcard/image.png", share_selector(shareCallback));
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
//���м�
void FinishLayer::menuCallBackRank(Ref* sender)
{
	//todo
	Scene* scene = TransitionSlideInL::create(0.15, BillBoardScene::createScene(1));
	Director::getInstance()->pushScene(scene);
}
//����Ӧvoid
void FinishLayer::menuVoid(Ref* sender)
{

}

void FinishLayer::updateRank(LabelTTF* label, int score)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	ToolBox::updateRank(label, score);
#endif
}

