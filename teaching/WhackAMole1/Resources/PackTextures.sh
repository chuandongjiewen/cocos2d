#!/bin/sh
TP="/usr/local/bin/TexturePacker"
SRC="/Cocos/projects/WhackAMole/Art"

echo "building..."
${TP} --smart-update \
--format cocos2d \
--data resources/hd/background.plist \
--sheet resources/hd/background.pvr.ccz \
--dither-fs \
--opt RGB565 \
${SRC}/background/*.png
${TP} --smart-update \
--format cocos2d \
--data resources/sd/background.plist \
--sheet resources/sd/background.pvr.ccz \
--dither-fs \
--scale 0.5 \
--opt RGB565 \
${SRC}/background/*.png
${TP} --smart-update \
--format cocos2d \
--data resources/hd/foreground.plist \
--sheet resources/hd/foreground.pvr.ccz \
--dither-fs-alpha \
--opt RGBA4444 \
${SRC}/foreground/*.png
${TP} --smart-update \
--format cocos2d \
--data resources/sd/foreground.plist \
--sheet resources/sd/foreground.pvr.ccz \
--dither-fs-alpha \
--scale 0.5 \
--opt RGBA4444 \
${SRC}/foreground/*.png
${TP} --smart-update \
--format cocos2d \
--data resources/hd/sprites.plist \
--sheet resources/hd/sprites.pvr.ccz \
--dither-fs-alpha \
--opt RGBA4444 \
${SRC}/sprites/*.png
${TP} --smart-update \
--format cocos2d \
--data resources/sd/sprites.plist \
--sheet resources/sd/sprites.pvr.ccz \
--dither-fs-alpha \
--scale 0.5 \
--opt RGBA4444 \
${SRC}/sprites/*.png

exit 0