LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm

LOCAL_MODULE := pine_framework

LOCAL_C_INCLUDES += $(LOCAL_PATH) \
					$(STLPORT_BASE)/stlport

LOCAL_CFLAGS := -DANDROID_NDK \
				-DANDROID_OS \
				-fexceptions \
				-std=c++0x \
				-w
APP_CPPFLAGS += -std=c++11
LOCAL_SRC_FILES := \
	core/jni_OsFunction.cpp \
	core/jni_game.cpp \
	core/jni_environment.cpp \
	core/jni_log.cpp \
	core/jni_service_manager.cpp \
	../../game/Lib/DataInputStream/DataInputStream.cpp \
	../../game/Lib/DataType/BufferStream.cpp \
	../../game/Lib/DataType/ServiceStream.cpp \
	../../game/Lib/Sprite/CSprite.cpp \
	../../game/Lib/OpenGLES/Graphic.cpp \
	../../game/Lib/OpenGLES/GLESLib1.cpp \
	../../game/Lib/png/png.cpp \
	../../game/Lib/PineImage.cpp \
	../../game/Lib/Sound/OpenSLES/Sound.cpp \
	../../game/Lib/Sound/OpenSLES/Music.cpp \
	../../game/Lib/Sound/OpenSLES/SoundOpenSLES.cpp \
	../../game/Lib/PineFacebookController.cpp \
	../../game/Lib/PineGameCenterController.cpp \
	../../game/Lib/PineTwitterController.cpp \
	../../game/Lib/PineSocial.cpp \
	../../game/Lib/PineSocialAsync.cpp \
	../../game/GameCore/CAnimObject.cpp \
	../../game/GameCore/CGame_Core.cpp \
	../../game/GameCore/CTouch.cpp \
	../../game/GameCore/Device.cpp \
	../../game/GameCore/GameContext.cpp \
	../../game/Lib/Math/CMath.cpp \
	../../game/Lib/Math/SMatrix4x4.cpp \
	../../game/Lib/Math/PinePolygon.cpp \
	../../game/GamePlay/Game_ExternFunctions.cpp \
	../../game/GamePlay/CGame_State.cpp \
	../../game/GamePlay/GameObject/CScrollBar.cpp \
	../../game/GamePlay/GameObject/MenuSystem.cpp \
	../../game/GamePlay/GameObject/SpriteList.cpp \
	../../game/GamePlay/GameObject/ExampleAnchor.cpp \
	../../game/GamePlay/GameObject/ExampleBufferStream.cpp \
	../../game/GamePlay/GameObject/ExampleSetting.cpp \
	../../game/GamePlay/GameState/CGame_State_Logo.cpp \
	../../game/GamePlay/GameState/CGame_State_Example.cpp

APP_STL := stlport_static
	
LOCAL_LDLIBS := -ldl -llog -lstdc++ -lc -lGLESv1_CM -lm -lOpenSLES -pthread -landroid
#-lGLESv1_CM  -lGLESv2
include $(BUILD_SHARED_LIBRARY)
