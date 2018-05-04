#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
int score = 0;
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
	/*
	if (!Scene::initWithPhysics()) {
		return false;
	}
	*/

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




	auto label = Label::createWithTTF("Score", "fonts/CircularStd-Black.otf", 24);
	label->setTextColor(Color4B(255, 0 , 100,255 ));
	if (label == nullptr)
	{
		problemLoading("'fonts/CircularStd-Black.otf'");
	}
	else
	{
		// position the label on the center of the screen
		label->setPosition(Vec2(origin.x -50 + visibleSize.width / 2,
			origin.y - 20 + visibleSize.height - label->getContentSize().height));

		// add the label as a child to this layer
		this->addChild(label, 1);
	}

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	/////////////////////////////
	// 3. add your codes below...



	// add created tiled map
	auto map = TMXTiledMap::create("MapAttemptTrial.tmx");
	if (map == nullptr)
	{
		problemLoading("'MapAttemptTrial.tmx'");
	}
	else
	{
		// add map
		this->addChild(map, 0, 99);
	}
	//find spawn object
	TMXObjectGroup *objects = map->getObjectGroup("Object-Player");
	auto SpawnPoint = objects->getObject("SpawnPoint");
	int xC = SpawnPoint["x"].asInt();
	int yC = SpawnPoint["y"].asInt();
	//read in player sprite now that spawn location has been determined
	auto player = Sprite::create("GeorgiaTech.png");
	if (player == nullptr)
	{
		problemLoading("GeorgiaTech.png");
	}
	else {
		//wrong collision detection
		//auto EB = PhysicsBody::createBox(player->getContentSize(), PhysicsMaterial(0, 1, 0));
		//EB->setCollisionBitmask(1);
		//EB->setContactTestBitmask(true);
		//player->setPhysicsBody(EB);
		player->setPosition(Vec2(xC, yC));
		this->addChild(player, 5);
	}
	//map->setPosition(convertToWorldSpace(player->getPosition()));

	auto eventListener = EventListenerKeyboard::create();
	auto mouseListener = EventListenerMouse::create();
	CCPoint playerPos = player->getPosition();
	CCPoint mapPos = map->getPosition();
	CCPoint newMapPos = mapPos;
	CCPoint ws = convertToWorldSpace(player->getPosition());

	auto enemy1 = Sprite::create("UGAB_1.png");

	enemy1->setPosition(Vec2(100, 200));

	this->addChild(enemy1);

	//Health that was not implemented
	/*
	auto health = Sprite::create("health.png");

	health->setPosition(Vec2(100, 400));

	this->addChild(health);
	*/
	mouseListener->onMouseDown = [enemy1](Event* event)
	{
		if (enemy1->getBoundingBox().containsPoint(event->getCurrentTarget()->getPosition()))
		{
			enemy1->removeFromParentAndCleanup(true);
			//enemy1->setPosition(enemy1->getPosition() + Point(10, 0));
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseListener, enemy1);
	

	auto origion = MoveTo::create(2, Vec2(100, 200));
	auto moveleft = MoveTo::create(2, Vec2(50, 200));
	//enemy1->runAction(moveleft);

	auto moveright = MoveTo::create(2, Vec2(250, 200));
	//enemy1->runAction(moveright); 

	auto seq = RepeatForever::create (Sequence::create((DelayTime::create(2)), moveleft, origion, moveright, origion, nullptr));
	if(enemy1 != nullptr)
	{
		enemy1->runAction(seq);
	}
	
	
	int width_player = player->getContentSize().width;
	int width_enemy1 = enemy1->getContentSize().width;
	int height_player = player->getContentSize().height;
	int height_enemy1 = enemy1->getContentSize().height;
	if (player->getPositionX() < enemy1->getPositionX() + width_enemy1 &&
		player->getPositionX() + width_player > enemy1->getPositionX() &&
		player->getPositionY() < enemy1->getPositionY() + height_player &&
		player->getPositionY()+ height_enemy1 > enemy1->getPositionY())
	{
		CCLOG("COLLISION HAS OCCURED");
		enemy1->removeFromParentAndCleanup(true);
	}
	

	eventListener->onKeyPressed = [](EventKeyboard::KeyCode keyCode, Event* event) {

		Vec2 loc = event->getCurrentTarget()->getPosition();
		switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		case EventKeyboard::KeyCode::KEY_A:
			event->getCurrentTarget()->setPosition(loc.x -= 32, loc.y);


			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		case EventKeyboard::KeyCode::KEY_D:
			event->getCurrentTarget()->setPosition(loc.x += 32, loc.y);
			break;
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
		case EventKeyboard::KeyCode::KEY_W:
			event->getCurrentTarget()->setPosition(loc.x, loc.y += 32);
			break;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		case EventKeyboard::KeyCode::KEY_S:
			event->getCurrentTarget()->setPosition(loc.x, loc.y -= 32);
			break;
		}


	};

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, player);
	

	return true;
}
/*
void HelloWorld::score()
{

	float time = 0.03;
	char* name = "Michael";
	char text[256];
	sprintf(text, "name is %s, time is %.2f, score is %d", name, time, score);
	ttf->setString(CCString::createWithFormat("%d, %s", 123, "Hello")->getCString());
	auto label1 = LabelTTF::initWithString(text, "Arial", 20);
	this->addChild(label1);
}

*/

void HelloWorld::enemy()
{
	/*
	auto enemy1 = Sprite::create("UGAB_1.png");
	enemy1->setTag(111);
	if (enemy1 == nullptr)
	{
		problemLoading("UGAB_1.jpg");
	}
	else {
		enemy1->setPosition(Vec2(100, 200));
		//wrong collision detection
		//auto EB = PhysicsBody::createBox(enemy1->getContentSize(), PhysicsMaterial(0, 1, 0));
		//EB->setCollisionBitmask(2);
		//EB->setContactTestBitmask(true);
		//enemy1->setPhysicsBody(EB);
		this->addChild(enemy1);
	}
	
	auto origion = MoveTo::create(2, Vec2(100, 200));
	auto moveleft = MoveTo::create(2, Vec2(50, 200));
	//enemy1->runAction(moveleft);

	auto moveright = MoveTo::create(2, Vec2(250, 200));
	//enemy1->runAction(moveright); 
	//(enemy1->getTag() == 111)

		auto seq = Sequence::create(moveleft, origion, moveright, origion, nullptr);
		enemy1->runAction(seq);
		CCLOG("COLLISION HAS OCCURED");
	*/
	/*
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegin, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
	*/
}
/*
bool HelloWorld::onContactBegin(cocos2d::PhysicsContact &contact)
{
	PhysicsBody *a = contact.getShapeA()->getBody();
	PhysicsBody *b = contact.getShapeB()->getBody();
	//check
	if ((1 == a->getCategoryBitmask() && 2 == b->getCategoryBitmask()) || ( 2 == a->getCategoryBitmask() && 1 == b->getCategoryBitmask()) )
	{
		CCLOG("COLLISION HAS OCCURED");
	}
	return true; 
}
*/
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