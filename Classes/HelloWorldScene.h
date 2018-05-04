#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Scene
{
private:
	cocos2d::CCTMXTiledMap* map;
public:

	static cocos2d::Scene* createScene();

	virtual bool init();
	void enemy();
	bool onContactBegin(cocos2d::PhysicsContact &contact);
	void setViewPointCenter(cocos2d::Point position);
	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(HelloWorld);

};

#endif // __HELLOWORLD_SCENE_H__