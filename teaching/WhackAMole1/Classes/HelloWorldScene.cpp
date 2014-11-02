#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    Size winSize = Director::getInstance()->getWinSize();

    // Determine names of sprite sheets and plists to load
    std::string bgSheet = "background.pvr.ccz";
    std::string bgPlist = "background.plist";
    std::string fgSheet = "foreground.pvr.ccz";
    std::string fgPlist = "foreground.plist";
    std::string sSheet  = "sprites.pvr.ccz";
    std::string sPlist  = "sprites.plist";
    
    // Load background and foreground
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(bgPlist);
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(fgPlist);
    
    // Add background
    Sprite *dirt = Sprite::createWithSpriteFrame(
                        SpriteFrameCache::getInstance()->getSpriteFrameByName("bg_dirt.png"));
    dirt->setScale(2.0);
    dirt->setPosition(winSize.width/2, winSize.height/2);
    this->addChild(dirt, -2);

    // Add foreground
    Sprite *lower = Sprite::createWithSpriteFrame(
                        SpriteFrameCache::getInstance()->getSpriteFrameByName("grass_lower.png"));
    lower->setAnchorPoint(Point(0.5, 1));
    lower->setPosition(winSize.width/2, winSize.height/2 + 1);
    this->addChild(lower, 1);
    Sprite *upper = Sprite::createWithSpriteFrame(
                        SpriteFrameCache::getInstance()->getSpriteFrameByName("grass_upper.png"));
    upper->setAnchorPoint(Point(0.5, 0));
    upper->setPosition(winSize.width/2, winSize.height/2 - 1);
    this->addChild(upper, -1);
    
    // Add more here later...
	SpriteBatchNode *spriteNode = SpriteBatchNode::create(sSheet);
	this->addChild(spriteNode, 0);
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(sPlist);

	// Load sprites
	Sprite *mole1 = Sprite::createWithSpriteFrameName("mole_1.png");
	mole1->setPosition(99, winSize.height / 2 - 75);
	spriteNode->addChild(mole1);
	molesVector.pushBack(mole1);

	Sprite *mole2 = Sprite::createWithSpriteFrameName("mole_1.png");
	mole2->setPosition(winSize.width / 2, winSize.height / 2 - 75);
	spriteNode->addChild(mole2);
	molesVector.pushBack(mole2);

	Sprite *mole3 = Sprite::createWithSpriteFrameName("mole_1.png");
	mole3->setPosition(winSize.width - 102, winSize.height / 2 - 75);
	spriteNode->addChild(mole3);
	molesVector.pushBack(mole3);

	this->schedule(schedule_selector(HelloWorld::tryPopMoles), 0.5);
    
    return true;
}

void HelloWorld::tryPopMoles(float dt)
{
	for (Sprite *mole : molesVector)
	{
		int temp = CCRANDOM_0_1()*10000;
		if ( temp % 3 == 0) 
		{
			if (mole->getNumberOfRunningActions() == 0) 
			{
				this->popMole(mole);
			}
		}
	}
}

void HelloWorld::popMole(Sprite *mole)
{
	auto moveUp = MoveBy::create(0.2f, Point(0, mole->getContentSize().height));  // 1
	auto easeMoveUp = EaseInOut::create(moveUp, 3.0f); // 2
	auto easeMoveDown = easeMoveUp->reverse(); // 3
	auto delay = DelayTime::create(0.5f); // 4

	mole->runAction(Sequence::create(easeMoveUp, delay, easeMoveDown, NULL)); // 5
}

void HelloWorld::menuCloseCallback(Object* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
