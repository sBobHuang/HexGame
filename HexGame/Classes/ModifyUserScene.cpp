#include "ModifyUserScene.h"
#include "Utils.h"
#include "network\HttpClient.h"
#include "json\document.h"

using namespace cocos2d::network;

cocos2d::Scene * ModifyUserScene::createScene() {
  return ModifyUserScene::create();
}

bool ModifyUserScene::init() {
  if (!Scene::init()) return false;
  
  auto visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();

  auto postDeckButton = MenuItemFont::create("Post Deck", CC_CALLBACK_1(ModifyUserScene::putDeckButtonCallback, this));
  if (postDeckButton) {
    float x = origin.x + visibleSize.width / 2;
    float y = origin.y + postDeckButton->getContentSize().height / 2;
    postDeckButton->setPosition(Vec2(x, y));
  }

  auto backButton = MenuItemFont::create("Back", [](Ref* pSender) {
    Director::getInstance()->popScene();
  });
  if (backButton) {
    float x = origin.x + visibleSize.width / 2;
    float y = origin.y + visibleSize.height - backButton->getContentSize().height / 2;
    backButton->setPosition(Vec2(x, y));
  }

  auto menu = Menu::create(postDeckButton, backButton, NULL);
  menu->setPosition(Vec2::ZERO);
  this->addChild(menu, 1);

  deckInput = TextField::create("Deck json here", "arial", 24);
  if (deckInput) {
    float x = origin.x + visibleSize.width / 2;
    float y = origin.y + visibleSize.height - 100.0f;
    deckInput->setPosition(Vec2(x, y));
    this->addChild(deckInput, 1);
  }

  messageBox = Label::create("", "arial", 30);
  if (messageBox) {
    float x = origin.x + visibleSize.width / 2;
    float y = origin.y + visibleSize.height / 2;
    messageBox->setPosition(Vec2(x, y));
    this->addChild(messageBox, 1);
  }

  return true;
}

//�ı俨��ĺ���
void ModifyUserScene::putDeckButtonCallback(Ref * pSender) {
  // Your code here
	//��֮ǰ��ע���¼����
	std::string deck = deckInput->getStringValue();
	std::string postData = "{\"deck\":" + deck + "}";

	//����PPT�����ݣ���put��ʽ��users
	HttpRequest* request = new HttpRequest();
	request->setUrl("http://bobhuang.xyz:5001/users");
	request->setRequestType(HttpRequest::Type::PUT);

	request->setResponseCallback(CC_CALLBACK_2(ModifyUserScene::onHttpRequestCompleted, this));
	request->setRequestData(postData.c_str(), strlen(postData .c_str()));
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}
//�ı俨��Ļص���������Ҫ��ʾ��Ϣ
void ModifyUserScene::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response) {
	auto buffer = response->getResponseData();
	rapidjson::Document doc;
	doc.Parse(buffer->data(), buffer->size());
	if (doc["status"] == true) {
		this->messageBox->setString("PUT OK");
	}
	else {
		this->messageBox->setString(std::string("PUT Failed\n") + doc["msg"].GetString());
	}
}