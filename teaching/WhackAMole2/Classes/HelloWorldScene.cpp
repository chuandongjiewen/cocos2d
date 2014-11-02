#include "HelloWorldScene.h"
#include"SimpleAudioEngine.h"

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
    if ( !Layer::init() )
    {
        return false;
    }

    auto winSize = Director::getInstance()->getWinSize();
	auto visibleSize = Director::getInstance()->getWinSize();
	auto visibleOrigin = Director::getInstance()->getVisibleOrigin();
	auto cache = SpriteFrameCache::getInstance();

    // Determine names of sprite sheets and plists to load
    std::string bgSheet = "background.pvr.ccz";
    std::string bgPlist = "background.plist";
    std::string fgSheet = "foreground.pvr.ccz";
    std::string fgPlist = "foreground.plist";
    std::string sSheet  = "sprites.pvr.ccz";
    std::string sPlist  = "sprites.plist";
    
    // Load background and foreground
    cache->addSpriteFramesWithFile(bgPlist);
    cache->addSpriteFramesWithFile(fgPlist);
    
    // Add background
    Sprite *dirt = Sprite::createWithSpriteFrame(
                        cache->getSpriteFrameByName("bg_dirt.png"));
    dirt->setScale(2.0);
    dirt->setPosition(winSize.width/2, winSize.height/2);
    this->addChild(dirt, -2);

    // Add foreground
    Sprite *lower = Sprite::createWithSpriteFrame(
                        cache->getSpriteFrameByName("grass_lower.png"));
    lower->setAnchorPoint(Point(0.5, 1));
    lower->setPosition(winSize.width/2, winSize.height/2 + 1);
    this->addChild(lower, 1);
    Sprite *upper = Sprite::createWithSpriteFrame(
                        cache->getSpriteFrameByName("grass_upper.png"));
    upper->setAnchorPoint(Point(0.5, 0));
    upper->setPosition(winSize.width/2, winSize.height/2 - 1);
    this->addChild(upper, -1);
    
	// Add more here later...
	SpriteBatchNode *spriteNode = SpriteBatchNode::create(sSheet);
	this->addChild(spriteNode, 0);
	cache->addSpriteFramesWithFile(sPlist);

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

	//add  update methond
	this->schedule(schedule_selector(HelloWorld::tryPopMoles), 0.5f);

	//create Animation
	int laughAnimOrder[6] = { 1, 2, 3, 2, 3, 1 };
	laughAnim = this->createAnimation("mole_laugh", laughAnimOrder, 6, 0.1f);
	int hitAnimOrder[6] = { 1, 2, 3, 4 };
	hitAnim = this->createAnimation("mole_thump", hitAnimOrder, 4, 0.02f);
	AnimationCache::getInstance()->addAnimation(laughAnim, "laughAnim");
	AnimationCache::getInstance()->addAnimation(hitAnim, "hitAnim");

	//touch event
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//add score label
	float margin = 10;
	label = LabelTTF::create("Score: 0", "fonts/Marker Felt.ttf", 14);
	label->setAnchorPoint(Point(1, 0));
	label->setPosition(visibleOrigin.x + visibleSize.width - margin, 
						visibleOrigin.y + margin);
	this->addChild(label, 10);

	//init game state
	gameOver = false;
	totalSpawns = 0;
	score = 0;

	//sound
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("laugh.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("ow.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("whack.mp3", true);
	
    return true;
}

bool HelloWorld::onTouchBegan(Touch *touch, Event *unused_event)
{
	Point touchLocation = this->convertTouchToNodeSpace(touch);
	for (Sprite *mole : molesVector) 
	{
		if (0 == mole->getTag()) continue;
		if ( mole->getBoundingBox().containsPoint(touchLocation) ) 
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("ow.mp3");

			mole->setTag(0);
			score += 10;

			mole->stopAllActions();
			Animate *hit = Animate::create(hitAnim);
			MoveBy *moveDown = MoveBy::create(0.2f, Point(0, -mole->getContentSize().height));
			EaseInOut *easeMoveDown = EaseInOut::create(moveDown, 3.0f);
			mole->runAction(Sequence::create(hit, easeMoveDown, NULL));
		}
	}

	return true;
}

Animation* HelloWorld::createAnimation(std::string prefixName, 
								int* pFramesOrder, 
								int framesNum, 
								float delay)
{
	Vector<SpriteFrame*> animFrames;	//1

	for (int i = 0; i < framesNum; i++)	//2
	{
		char buffer[20] = { 0 };	//3
		sprintf(buffer, "%d.png", pFramesOrder[i]);	//3
		std::string str = prefixName + buffer;	//3
		auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);	//4
		animFrames.pushBack(frame);		//4	
	}

	return Animation::createWithSpriteFrames(animFrames, delay);	//5
}

void HelloWorld::tryPopMoles(float dt)
{
	if (gameOver) return;

	char scoreStr[30] = { 0 };
	sprintf(scoreStr, "Score: %d", score);
	label->setString(scoreStr);

	if (totalSpawns >= 50) {
		Size winSize = Director::getInstance()->getWinSize();

		LabelTTF *goLabel = LabelTTF::create("Level Complete!", "fonts/Marker Felt.ttf", 48.0f);
		goLabel->setPosition(Point(winSize.width / 2, winSize.height / 2));
		goLabel->setScale(0.1f);
		this->addChild(goLabel, 10);
		goLabel->runAction(ScaleTo::create(0.5f, 1.0f));

		gameOver = true;
		return;
	}

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

void HelloWorld::setTappable(Object* pSender)
{
	Sprite *mole = (Sprite *)pSender;
	mole->setTag(1);
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("laugh.mp3");
}

void HelloWorld::unsetTappable(Object* pSender)
{
	Sprite *mole = (Sprite *)pSender;
	mole->setTag(0);
}

void HelloWorld::popMole(Sprite *mole)
{
	if (totalSpawns > 50) return;
	totalSpawns++;

	mole->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("mole_1.png"));

	// Pop mole
	auto moveUp = MoveBy::create(0.2f, Point(0, mole->getContentSize().height));  // 1
	auto easeMoveUp = EaseInOut::create(moveUp, 3.0f); // 2
	auto easeMoveDown = easeMoveUp->reverse(); // 3
	//auto delay = DelayTime::create(0.5f); // 4
	Animate *laugh = Animate::create(laughAnim);

	mole->runAction(Sequence::create(easeMoveUp, 
			CallFuncN::create(CC_CALLBACK_1(HelloWorld::setTappable, this)),
			laugh,
			CallFuncN::create(CC_CALLBACK_1(HelloWorld::unsetTappable, this)),
			easeMoveDown, 
			NULL)); // 5
}
