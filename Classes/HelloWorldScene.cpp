#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
	return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

	if (closeItem == nullptr ||
		closeItem->getContentSize().width <= 0 ||
		closeItem->getContentSize().height <= 0)
	{
		problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
	}
	else
	{
		float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
		float y = origin.y + closeItem->getContentSize().height / 2;
		closeItem->setPosition(Vec2(x, y));
	}

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	/////////////////////////////
	// 3. add your codes below...

	// add a label shows "Hello World"
	// create and initialize a label

	auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
	if (label == nullptr)
	{
		problemLoading("'fonts/Marker Felt.ttf'");
	}
	else
	{
		// position the label on the center of the screen
		label->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height - label->getContentSize().height));

		// add the label as a child to this layer
		this->addChild(label, 1);
	}

	// add "HelloWorld" splash screen"
	auto map = TMXTiledMap::create("MapAttemptTrial.tmx");

	if (map == nullptr)
	{
		problemLoading("'MapAttemptTrial.tmx'");
	}
	else
	{

		// add the sprite as a child to this layer
		this->addChild(map, 0, 99);
	}
	TMXObjectGroup *objects = map->getObjectGroup("Object-Player");
	auto SpawnPoint = objects->getObject("SpawnPoint");
	int xC = SpawnPoint["x"].asInt();
	int yC = SpawnPoint["y"].asInt();
	auto player = Sprite::create("Player.png");
	player->setPosition(Vec2(xC, yC));
	this->addChild(player, 1);
	auto eventListener = EventListenerKeyboard::create();
	CCPoint playerPos = player->getPosition();
	CCPoint mapPos = map->getPosition();
	CCPoint newMapPos = mapPos;
	auto mapSize = map->getMapSize();
	float kMarginHorizontalPercent = .8;
	float kMarginVerticalPercent = .8;
	float limitLeft = mapPos.x + mapSize.width*(1 - kMarginHorizontalPercent);
	float limitRight = mapPos.x + mapSize.width*kMarginHorizontalPercent;

	if (limitLeft > playerPos.x)
		newMapPos.x = (mapPos.x - (limitLeft - playerPos.x))*-1;
	else if (limitRight < playerPos.x)
		newMapPos.x = (mapPos.x + (playerPos.x - limitRight))*-1;

	float limitBottom = mapPos.y + mapSize.height*(1 - kMarginVerticalPercent);
	float limitUpper = mapPos.y + mapSize.height*kMarginVerticalPercent;

	if (limitBottom > playerPos.y)
		newMapPos.y = (mapPos.y - (limitBottom - playerPos.y))*-1;
	else if (limitUpper < playerPos.y)
		newMapPos.y = (mapPos.y + (playerPos.y - limitUpper))*-1;

	map->setPosition(newMapPos);



	eventListener->onKeyPressed = [](EventKeyboard::KeyCode keyCode, Event* event) {

		Vec2 loc = event->getCurrentTarget()->getPosition();
		switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		case EventKeyboard::KeyCode::KEY_A:
			event->getCurrentTarget()->setPosition(loc.x -= 10, loc.y);

			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		case EventKeyboard::KeyCode::KEY_D:
			event->getCurrentTarget()->setPosition(loc.x += 10, loc.y);
			break;
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
		case EventKeyboard::KeyCode::KEY_W:
			event->getCurrentTarget()->setPosition(loc.x, loc.y += 10);
			break;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		case EventKeyboard::KeyCode::KEY_S:
			event->getCurrentTarget()->setPosition(loc.x, loc.y -= 10);
			break;
		}


	};

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, player);

	return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);


}
