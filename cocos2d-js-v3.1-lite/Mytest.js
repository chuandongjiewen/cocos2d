/**
 * Created by wangjiewen on 14-10-29.
 */




var StartLayer = cc.Sprite.extend({
    /**
     * 构造函数
     */
    ctor : function(){
        this._super('liliad.png', cc.rect(50, 20, 200, 200));
        var size = cc.winSize;
        this.setPosition(size.width/2, size.height/2);
        this.setScale(0.8);

        var helloLabel = new cc.LabelTTF("Hello World", "", 38);
        helloLabel.x = size.width / 2;
        helloLabel.y = size.height / 2;
        this.addChild(helloLabel);
        return true;
    }
});

var StartScene = cc.Scene.extend({
    onEnter : function(){
        this._super();
        var startLayer = new StartLayer();
        this.addChild(startLayer);
    }
});


window.onload = function(){
    cc.game.onStart = function(){
        cc.LoaderScene.preload(['liliad.png'], function () {
            cc.director.runScene(new StartScene());
        }, this);
    }

    cc.game.run('gameCanvas');
}

/**
 * 测试文件
 * @param {String} name
 * @param {int} age
 * @returns {String}
 */
function test(name, age){

    return name + age;
}