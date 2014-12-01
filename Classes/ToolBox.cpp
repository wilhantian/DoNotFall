#include "ToolBox.h"
#include "editor-support/spine/Json.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#endif

USING_NS_CC;

LabelTTF* ToolBox::rank_label = NULL;

const char* ToolBox::getStringWithJsonFile(const char* file, const char* name, const char* va)
{
	//����json�ļ�
	ssize_t len;
	std::string fileName = FileUtils::getInstance()->fullPathForFilename(file);
	unsigned char* jsonData = FileUtils::getInstance()->getFileData(fileName, "r", &len);
	//����json��������
	Json* json = Json_create((char*)jsonData);
	return Json_getString(json, name, va);
}

const char* ToolBox::getStrTitle()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	//������Ϣ�ṹ��
	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo,/*JniMethodInfo������*/
												 "org/cocos2dx/cpp/AppActivity",/*���·��*/
												 "getStrTitle",/*������*/
												 "()Ljava/lang/String;");/*�������ͼ�д*/
	jobject activityObj;
	if (isHave)
	{
		//CallStaticObjectMethod����java���������ѷ���ֵ��ֵ��activityObj
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
	//������Ϣ�ṹ��
	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo,/*JniMethodInfo������*/
												 "org/cocos2dx/cpp/AppActivity",/*���·��*/
												 "getRankJson",/*������*/
												 "()Ljava/lang/String;");/*�������ͼ�д*/
	jobject activityObj;
	if (isHave)
	{
		//CallStaticObjectMethod����java���������ѷ���ֵ��ֵ��activityObj
		activityObj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);

		jstring jstr = (jstring)activityObj;
		std::string text = JniHelper::jstring2string(jstr);
//		return text.c_str();
		//todo
		//����json��������
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
	//��һ��ȡ������ļ�·��
	    std::string path = CCFileUtils::sharedFileUtils()->getWritablePath() + filename;
	    log("wanna save file path = %s",path.c_str());

	    //����һ���ļ�ָ��
	    //·����ģʽ
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
	//��һ�Ȼ�ȡ�ļ���·��
		std::string path = CCFileUtils::sharedFileUtils()->getWritablePath() + pFileName;
	    CCLOG("path = %s",path.c_str());

	    //����һ���ļ�ָ��
	    FILE* file = fopen(path.c_str(), "r");

	    if (file) {
	        char* buf;  //Ҫ��ȡ���ַ���
	        int len;    //��ȡ�ĳ���
	        /*��ȡ����*/
	        fseek(file, 0, SEEK_END);   //�Ƶ�β��
	        len = ftell(file);          //��ȡ����
	        rewind(file);               //�ع�ԭλ
	        log("count the file content len = %d",len);
	        //����buf�ռ�
	        buf = (char*)malloc(sizeof(char) * len + 1);
	        if (!buf) {
	        	log("malloc space is not enough.");
	            return NULL;
	        }

	        //��ȡ�ļ�
	        //��ȡ����buf����λ��С�����ȣ��ļ�ָ��
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
	//������Ϣ�ṹ��
	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo,/*JniMethodInfo������*/
												 "org/cocos2dx/cpp/AppActivity",/*���·��*/
												 "updateRank",/*������*/
												 "(I)V");/*�������ͼ�д*/
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
	//������Ϣ�ṹ��
	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo,/*JniMethodInfo������*/
												 "org/cocos2dx/cpp/AppActivity",/*���·��*/
												 "updateFillBoard",/*������*/
												 "()V");/*�������ͼ�д*/
	if (isHave)
	{
		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
	}
#endif
}

/*Ԥ���ز������*/
void ToolBox::loadSpotAds()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	//������Ϣ�ṹ��
	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo,/*JniMethodInfo������*/
												 "org/cocos2dx/cpp/AppActivity",/*���·��*/
												 "loadSpotAds",/*������*/
												 "()V");/*�������ͼ�д*/
	if (isHave)
	{
		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
	}
#endif
}

/*��ʾ�������*/
void ToolBox::showSpotAds()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	//������Ϣ�ṹ��
	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo,/*JniMethodInfo������*/
												 "org/cocos2dx/cpp/AppActivity",/*���·��*/
												 "showSpotAds",/*������*/
												 "()V");/*�������ͼ�д*/
	if (isHave)
	{
		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
	}
#endif
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//javaͨѶ
extern "C"
{
	//��ȡmail�ɹ�
    void Java_org_cocos2dx_cpp_AppActivity_onMailSuccess(JNIEnv*  env, jobject thiz, jstring title, jstring context)
    {
    	std::string str_title = JniHelper::jstring2string(title);
    	std::string str_context = JniHelper::jstring2string(context);

    	log("%s", str_title.c_str());
    	log("%s", str_context.c_str());

    	UserDefault::getInstance()->setStringForKey("title", str_title);
    	UserDefault::getInstance()->setStringForKey("context", str_context);
    }
    //��ȡmailʧ��
    void Java_org_cocos2dx_cpp_AppActivity_onMailFailure(JNIEnv*  env, jobject thiz)
    {
    	UserDefault::getInstance()->setStringForKey("title", "title null");
    	UserDefault::getInstance()->setStringForKey("context", "context null");
    }

    //��ȡ���а�ɹ�
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
	//��ȡ���а�ʧ��
	void Java_org_cocos2dx_cpp_AppActivity_onFillBoardFailure(JNIEnv*  env, jobject thiz)
	{

	}

    //��ȡ���������ɹ�
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
	//��ȡ��������ʧ��
	void Java_org_cocos2dx_cpp_AppActivity_onRankFailure(JNIEnv*  env, jobject thiz)
	{
		if(ToolBox::rank_label)
		{
			ToolBox::rank_label->setString("network error");
		}
	}

	//��ȡshare context�ɹ�
    void Java_org_cocos2dx_cpp_AppActivity_onShareContextSuccess(JNIEnv*  env, jobject thiz, jstring text)
    {
    	std::string str_text = JniHelper::jstring2string(text);

    	UserDefault::getInstance()->setStringForKey("share_text", str_text);
    }
}
#endif
