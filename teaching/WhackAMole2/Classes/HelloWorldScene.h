#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

	cocos2d::Animation* createAnimation(std::string prefixName, int* pFramesOrder, int framesNum, float delay);
	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	void tryPopMoles(float dt);
	void popMole(cocos2d::Sprite *mole);
	void unsetTappable(Object* pSender);
	void setTappable(Object* pSender);
	
	cocos2d::Vector<cocos2d::Sprite*> molesVector;
	cocos2d::Animation *laughAnim;
	cocos2d::Animation *hitAnim;

	cocos2d::LabelTTF *label;
	int score;
	int totalSpawns;
	bool gameOver;
};

#endif // __HELLOWORLD_SCENE_H__
