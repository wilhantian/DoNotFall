#include "ToolBox.h"
#include "editor-support/spine/Json.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#endif

USING_NS_CC;

LabelTTF* ToolBox::rank_label = NULL;

const char* ToolBox::getStringWithJsonFile(const char* file, const char* name, const char* va)
{
	//加载json文件
	ssize_t len;
	std::string fileName = FileUtils::getInstance()->fullPathForFilename(file);
	unsigned char* jsonData = FileUtils::getInstance()->getFileData(fileName, "r", &len);
	//创建json解析对象
	Json* json = Json_create((char*)jsonData);
	return Json_getString(json, name, va);
}

const char* ToolBox::getStrTitle()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	//函数信息结构体
	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo,/*JniMethodInfo的引用*/
												 "org/cocos2dx/cpp/AppActivity",/*类的路径*/
												 "getStrTitle",/*函数名*/
												 "()Ljava/lang/String;");/*函数类型简写*/
	jobject activityObj;
	if (isHave)
	{
		//CallStaticObjectMethod调用java函数，并把返回值赋值给activityObj
		activityObj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);

		jstring jstr = (jstring)activityObj;
		std::string text = JniHelper::jstring2string(jstr);
		log("%s", text.c_str());
		return text.c_str();
	}
#endif
	return "";
}

char* ToolBox::getRankJson()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	//函数信息结构体
	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo,/*JniMethodInfo的引用*/
												 "org/cocos2dx/cpp/AppActivity",/*类的路径*/
												 "getRankJson",/*函数名*/
												 "()Ljava/lang/String;");/*函数类型简写*/
	jobject activityObj;
	if (isHave)
	{
		//CallStaticObjectMethod调用java函数，并把返回值赋值给activityObj
		activityObj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);

		jstring jstr = (jstring)activityObj;
		std::string text = JniHelper::jstring2string(jstr);
//		return text.c_str();
		//todo
		//创建json解析对象
		Json* json = Json_create(text.c_str());
		Json* items = Json_getItem(json, "results");
		int posSize = items->size;

		Json* j = items->child;
		for (int i = 0; i < posSize; i++)
		{
			std::string name = Json_getString(j, "name", "NLL");
			int score = Json_getInt(j, "score", 1);

			log("%s %d", name.c_str(), score);

			j = j->next;
		}
	}

#endif
	return "";
}

bool ToolBox::saveJsonFile(std::string filename, const char* jsonContext)
{
	//第一获取储存的文件路径
	    std::string path = CCFileUtils::sharedFileUtils()->getWritablePath() + filename;
	    log("wanna save file path = %s",path.c_str());

	    //创建一个文件指针
	    //路径、模式
	    FILE* file = fopen(path.c_str(), "w");
	    if (file) {
	        fputs(jsonContext, file);
	        fclose(file);
			return true;
	    }
	    else
	        log("save file error.");

			return false;
}

const char* ToolBox::getFileByName(std::string pFileName)
{
	//第一先获取文件的路径
		std::string path = CCFileUtils::sharedFileUtils()->getWritablePath() + pFileName;
	    CCLOG("path = %s",path.c_str());

	    //创建一个文件指针
	    FILE* file = fopen(path.c_str(), "r");

	    if (file) {
	        char* buf;  //要获取的字符串
	        int len;    //获取的长度
	        /*获取长度*/
	        fseek(file, 0, SEEK_END);   //移到尾部
	        len = ftell(file);          //提取长度
	        rewind(file);               //回归原位
	        log("count the file content len = %d",len);
	        //分配buf空间
	        buf = (char*)malloc(sizeof(char) * len + 1);
	        if (!buf) {
	        	log("malloc space is not enough.");
	            return NULL;
	        }

	        //读取文件
	        //读取进的buf，单位大小，长度，文件指针
	        int rLen = fread(buf, sizeof(char), len, file);
	        buf[rLen] = '\0';
	        log("has read Length = %d",rLen);
	        log("has read content = %s",buf);

	        std::string result = buf;
	        fclose(file);
	        free(buf);
	        return result.c_str();
	    }
	    else
	        CCLOG("open file error.");
	    return NULL;
}

void ToolBox::updateRank(LabelTTF* label, int score)
{
	rank_label = label;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	//函数信息结构体
	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo,/*JniMethodInfo的引用*/
												 "org/cocos2dx/cpp/AppActivity",/*类的路径*/
												 "updateRank",/*函数名*/
												 "(I)V");/*函数类型简写*/
	if (isHave)
	{
		jint j_score = score;
		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, j_score);
	}
#endif
}

void ToolBox::updateFillBoard()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	//函数信息结构体
	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo,/*JniMethodInfo的引用*/
												 "org/cocos2dx/cpp/AppActivity",/*类的路径*/
												 "updateFillBoard",/*函数名*/
												 "()V");/*函数类型简写*/
	if (isHave)
	{
		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
	}
#endif
}

/*预加载插屏广告*/
void ToolBox::loadSpotAds()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	//函数信息结构体
	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo,/*JniMethodInfo的引用*/
												 "org/cocos2dx/cpp/AppActivity",/*类的路径*/
												 "loadSpotAds",/*函数名*/
												 "()V");/*函数类型简写*/
	if (isHave)
	{
		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
	}
#endif
}

/*显示插屏广告*/
void ToolBox::showSpotAds()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	//函数信息结构体
	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo,/*JniMethodInfo的引用*/
												 "org/cocos2dx/cpp/AppActivity",/*类的路径*/
												 "showSpotAds",/*函数名*/
												 "()V");/*函数类型简写*/
	if (isHave)
	{
		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
	}
#endif
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//java通讯
extern "C"
{
	//获取mail成功
    void Java_org_cocos2dx_cpp_AppActivity_onMailSuccess(JNIEnv*  env, jobject thiz, jstring title, jstring context)
    {
    	std::string str_title = JniHelper::jstring2string(title);
    	std::string str_context = JniHelper::jstring2string(context);

    	log("%s", str_title.c_str());
    	log("%s", str_context.c_str());

    	UserDefault::getInstance()->setStringForKey("title", str_title);
    	UserDefault::getInstance()->setStringForKey("context", str_context);
    }
    //获取mail失败
    void Java_org_cocos2dx_cpp_AppActivity_onMailFailure(JNIEnv*  env, jobject thiz)
    {
    	UserDefault::getInstance()->setStringForKey("title", "title null");
    	UserDefault::getInstance()->setStringForKey("context", "context null");
    }

    //获取排行榜成功
	void Java_org_cocos2dx_cpp_AppActivity_onFillBoardSuccess(JNIEnv*  env, jobject thiz, jstring rankJson)
	{
		std::string str_rankJson = JniHelper::jstring2string(rankJson);
		UserDefault::getInstance()->setStringForKey("str_rankJson", str_rankJson);
/*
		Json* json = Json_create(str_rankJson.c_str());
		Json* items = Json_getItem(json, "results");
		int posSize = items->size;

		Json* j = items->child;
		for (int i = 0; i < posSize; i++)
		{
			std::string name = Json_getString(j, "name", "NLL");
			int score = Json_getInt(j, "score", 1);
			//todo
			log("FillBoard : %s %d", name.c_str(), score);

			j = j->next;
		}
*/
	}
	//获取排行榜失败
	void Java_org_cocos2dx_cpp_AppActivity_onFillBoardFailure(JNIEnv*  env, jobject thiz)
	{

	}

    //获取独立排名成功
	void Java_org_cocos2dx_cpp_AppActivity_onRankSuccess(JNIEnv*  env, jobject thiz, jint rank)
	{
		int c_rank = rank;

		char str_rank[20];
		sprintf(str_rank, "%d", c_rank);
		if(ToolBox::rank_label)
		{
			ToolBox::rank_label->setString(str_rank);
		}
	}
	//获取独立排名失败
	void Java_org_cocos2dx_cpp_AppActivity_onRankFailure(JNIEnv*  env, jobject thiz)
	{
		if(ToolBox::rank_label)
		{
			ToolBox::rank_label->setString("network error");
		}
	}

	//获取share context成功
    void Java_org_cocos2dx_cpp_AppActivity_onShareContextSuccess(JNIEnv*  env, jobject thiz, jstring text)
    {
    	std::string str_text = JniHelper::jstring2string(text);

    	UserDefault::getInstance()->setStringForKey("share_text", str_text);
    }
}
#endif
