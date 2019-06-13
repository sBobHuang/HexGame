#include "LoginRegisterScene.h"
#include "network\HttpClient.h"
#include "json\document.h"
#include "Utils.h"
#include "SimpleAudioEngine.h"
#include "GameScene.h"

USING_NS_CC;
using namespace cocos2d::network;

cocos2d::Scene * LoginRegisterScene::createScene() {
  return LoginRegisterScene::create();
}

bool LoginRegisterScene::init() {
  if (!Scene::init()) {
    return false;
  }
  auto winSize = Director::getInstance()->getWinSize();
  auto bg = Scale9Sprite::create("dialog2.png");
  bg->setCapInsets(Rect(12, 12, 81, 78));

  bg->setPosition(winSize.width / 2, winSize.height / 2);
  addChild(bg);
  auto backButton = Button::create("youke1.png");
  backButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
	  auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	  audio->playBackgroundMusic("sound/bgmusictwo.mp3", true);
	  GameScene* parent = static_cast<GameScene*>(this->getParent());
	  if (this->getParent())
	  {
		  parent->replayGame(1);
		  parent->removeChild(this);
	  }
  });
  if (backButton) {
	  auto visibleSize = Director::getInstance()->getVisibleSize();
	  Vec2 origin = Director::getInstance()->getVisibleOrigin();
	  float x = origin.x + visibleSize.width / 2 + 240.f;
	  float y = origin.y + visibleSize.height / 2 - 300.f;
	  backButton->setPosition(Vec2(x, y));
  }
  this->addChild(backButton);
  auto visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();

 
  auto loginButton = Button::create("login.png");
  if (loginButton) {
    float x = origin.x + visibleSize.width / 2;
	float y = origin.y + visibleSize.height / 2 - 170.f;
    loginButton->setPosition(Vec2(x, y));
  }
  loginButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
	  // Your code here
	  //对应着服务端的username和password，传入json格式的数据
	  std::string username = usernameInput->getStringValue();
	  std::string password = passwordInput->getStringValue();
	  std::string postData = "{\"username\":\"" + username + "\"," + "\"password\":\"" + password + "\"}";

	  //用post方式传输,根据ppt里面的提示，用auth
	  HttpRequest* request = new HttpRequest();

	  request->setUrl("http://bobhuang.xyz:5001/auth");
	  request->setRequestType(HttpRequest::Type::POST);
	  request->setResponseCallback(CC_CALLBACK_2(LoginRegisterScene::onHttpRequestCompleted, this));
	  request->setRequestData(postData.c_str(), strlen(postData.c_str()));

	  //这里就是使用Cookies的地方，只要加这一句就好，可以在报头看到
	  //在登录的时候加上cookies就可以，这样服务端就可以记录正在使用的客户
	  //作为切换用户的标识
	  cocos2d::network::HttpClient::getInstance()->enableCookies(NULL);
	  cocos2d::network::HttpClient::getInstance()->send(request);
	  request->release();
  });
  this->addChild(loginButton);
  auto registerButton = Button::create("register.png");
  if (registerButton) {
    float x = origin.x + visibleSize.width / 2;
	float y = origin.y + visibleSize.height / 2 - 240.f;
    registerButton->setPosition(Vec2(x, y));
  }
  registerButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
	  // Your code here
	  //和登录函数差不多
	  std::string username = usernameInput->getStringValue();
	  std::string password = passwordInput->getStringValue();
	  std::string postData = "{\"username\":\"" + username + "\"," + "\"password\":\"" + password + "\"}";

	  //用Post传输方式，根据PPT里面的提示用users
	  HttpRequest* request = new HttpRequest();
	  request->setUrl("http://bobhuang.xyz:5001/users");
	  request->setRequestType(HttpRequest::Type::POST);
	  request->setResponseCallback(CC_CALLBACK_2(LoginRegisterScene::onHttpRequestCompleted1, this));
	  request->setRequestData(postData.c_str(), strlen(postData.c_str()));

	  cocos2d::network::HttpClient::getInstance()->send(request);
	  request->release();
  });
  this->addChild(registerButton);
  usernameInput = TextField::create("username", "arial", 50);
  if (usernameInput) {
    float x = origin.x + visibleSize.width / 2;
    float y = origin.y + visibleSize.height /2+100.f;
    usernameInput->setPosition(Vec2(x, y));
    this->addChild(usernameInput, 1);
  }

  passwordInput = TextField::create("password", "arial", 50);
  if (passwordInput) {
    float x = origin.x + visibleSize.width / 2;
    float y = origin.y + visibleSize.height /2+50.f;
    passwordInput->setPosition(Vec2(x, y));
    this->addChild(passwordInput, 1);
  }

  messageBox = Label::create("", "arial", 30);
  if (messageBox) {
    float x = origin.x + visibleSize.width / 2;
    float y = origin.y + visibleSize.height/2 - 100.0f;
    messageBox->setPosition(Vec2(x, y));
    this->addChild(messageBox, 1);
  }

  return true;
}

//登录的回调函数，主要看返回的状态
void LoginRegisterScene::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response)
{
	if (!response) {
		usernameInput->setString("not 200");
		return;
	}
	auto buffer = response->getResponseData();
	rapidjson::Document doc;
	doc.Parse(buffer->data(),buffer->size());
	if (doc["status"] == true) {
		Global::user = usernameInput->getString();
		this->messageBox->setString("Login OK,You Can Back");
		auto backButton = Button::create("back.png");
		backButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
			auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
			audio->playBackgroundMusic("sound/bgmusictwo.mp3", true);
			GameScene* parent = static_cast<GameScene*>(this->getParent());
			if (this->getParent())
			{
				parent->replayGame(1);
				parent->removeChild(this);
			}
		});
		if (backButton) {
			auto visibleSize = Director::getInstance()->getVisibleSize();
			Vec2 origin = Director::getInstance()->getVisibleOrigin();
			float x = origin.x + visibleSize.width / 2;
			float y = origin.y + visibleSize.height / 2 -50.0f;
			backButton->setPosition(Vec2(x, y));
		}
		this->addChild(backButton);
	}
	else {
		this->messageBox->setString(std::string("Login Failed\n") + doc["msg"].GetString());
	}
}

//注册的回调函数,主要显示信息
void LoginRegisterScene::onHttpRequestCompleted1(HttpClient *sender, HttpResponse *response)
{
	auto buffer = response->getResponseData();
	rapidjson::Document doc;
	doc.Parse(buffer->data(), buffer->size());
	if (doc["status"] == true) {
		this->messageBox->setString("Register OK");
	}
	else {
		this->messageBox->setString(std::string("Register Failed\n") + doc["msg"].GetString());
	}
}