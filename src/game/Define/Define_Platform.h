#define MAX_TOUCHES 					10

// hardware type

#define DEVICE_IPHONE				0
#define DEVICE_IPHONE_3G			1
#define DEVICE_IPOD					2
#define DEVICE_IPHONE_3GS			3
#define DEVICE_IPOD_4				4
#define DEVICE_IPHONE_4G			5
#define DEVICE_IPAD					6
#define DEVICE_SIMULATOR			7

#define UNIVERSAL_BUILD				0

#define DEFAULT_SCR_W				480
#define DEFAULT_SCR_H				320
#define DEFAULT_DIV_W				480
#define DEFAULT_DIV_H				320

#define D4_SCR_W					480
#define D4_SCR_H					320
#define D4_DIV_W					480
#define D4_DIV_H					320

#define IPAD_SCR_W					1024
#define IPAD_SCR_H					768
#define IPAD_DIV_W					1024
#define IPAD_DIV_H					768
#define ADD_ADVERT	0
#define FREE_GAME_OF_THEDAY         0
#define ADD_PICK_IMAGE              1

#define PICK_STATE_NONE             0
#define PICK_STATE_TAKE_PICTURE     1
#define PICK_STATE_CHOICE_PICTURE   2
#define ENABLE_SERVER_PUSH_NOTIFICATION 1
#define IS_DEVELOPMENT_VERSION 1

#define USE_GL_SCALE

#define APPLE_ID 983872088
#define ADVERT_APPLE_ID APPLE_ID

#define GAME_CENTER_FRIEND_NUM 1

#define CA_CERT_PATH        (GetBundlePath())
#if defined(IS_DEVELOPMENT_VERSION)

#define RSA_CLIENT_CERT     GetRealFilePath("apns-dev-cert.pem")
#define RSA_CLIENT_KEY      GetRealFilePath("apns-dev-key.pem")


#define APPLE_HOST          "gateway.sandbox.push.apple.com"
#define APPLE_PORT          2195

#define APPLE_FEEDBACK_HOST "feedback.sandbox.push.apple.com"
#define APPLE_FEEDBACK_PORT 2196
#else

#define RSA_CLIENT_CERT     GetRealFilePath("apns-production-cert.pem")
#define RSA_CLIENT_KEY      GetRealFilePath("apns-production-key.pem")


#define APPLE_HOST          "gateway.push.apple.com"
#define APPLE_PORT          2195

#define APPLE_FEEDBACK_HOST "feedback.push.apple.com"
#define APPLE_FEEDBACK_PORT 2196
#endif

#define USE_NEW_SYSTEM_VIEW_IOS_7_8 1
#define IOS8_MASTER_W (750.0f)
#define IOS8_MASTER_H (1334.0f)

#define ADD_VIDEO_REWARD 1
#define ADD_CHARTBOOST 1
#define ADD_ADVERT 1
#define ADD_IADVERT 1
#define USE_ADBOX 1
#define ADD_ADMOB   1
#define ADBOX_CONFIG_FILE "adbox.config"
#define CHARTBOOST_CONFIG_FILE "chartboost.config"
#define TAG_LINE "#ShakeDuckOff"

#define USE_PINESOCIAL_AT_ID_STRING 1
#define USE_PINESOCIAL  1
#define ADD_GAMECENTER  1
#define ADD_TWITTER     0
#define ADD_FACEBOOK    1
#define ADD_IAP         1

#define FACEBOOK_ID "344415252434208"
#define FACEBOOK_TRANSFER_IAP       (0)
#define FACEBOOK_API                (20)

#define FACEBOOK_PERMISSION [@"user_friends"]
#define FACEBOOK_PERMISSION_FOR_STREAM [@"user_friends",@"publish_actions"]
#define FACEBOOK_APP_LINK "https://fb.me/344416242434109"
#define FACEBOOOKAUDIENCE ACFacebookAudienceEveryone

#define GAME_NAME_REQUIRE_LOGIN "Shake Duck Off"
#define REQUIRES_FACEBOOOK_LOGIN "requires Facebook account to back up your data online. Check your device setting for logging in and re-launch the app to secure your progress."

#define PURCHASE_FUNC(x,y) {CGame::_this->PurchaseIAP(x,y);}

#define ADMOB_INTERSTITIAL_ID "ca-app-pub-6305464951596521/5853713491"
#if defined (ANDROID_OS)
#define ADMOB_INTERSTITIAL_ID "ca-app-pub-6305464951596521/5853713491"
#endif
#if defined (_WINDOWS_PHONE8) || defined(_WIN8)
#define ADMOB_INTERSTITIAL_ID "ca-app-pub-6305464951596521/4336941095"
#endif

#define CHARTBOOST_ID1_DEFAULT "552f22a7c909a61265f96080"
#define CHARTBOOST_ID2_DEFAULT "4cb850b9442130c5b07ef0c994c1ccd1e1989d08"

#define ADMOB_ID1_DEFAULT "6305464951596521"
#define ADMOB_ID2_DEFAULT "5613462690"


#define ADD_SCREEN_CAPTUREVIEW 0
#define FPS_VIDEO_RECORD 30
#define CAPTURE_VIDEO_W    1334
#define CAPTURE_VIDEO_H     750

#define DO_SHAKEKING()          {MAIN_GAME()->_gamePlay.DoShaking();}




#define GC_SUBMIT_SCORE_ID				 "SDO_TOPHUNTER"

#define SENSITIVITY_ROUND (100)
#define SENSITIVITY_MULTIX (8)
#define SENSITIVITY_MULTIY (12)
#define SENSITIVITY_SPEED_BASE (1000)
#define SENSITIVITY_SPEED_MIN (3.0)

#define SENSITIVITY_ACCELL (0.1)
#define SENSITIVITY_SPEED (0.4)

