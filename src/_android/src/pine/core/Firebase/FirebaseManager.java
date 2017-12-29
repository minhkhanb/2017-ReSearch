package pine.core.Firebase;


import android.app.Activity;
import android.util.Log;

import pine.core.AppConfig;

public class FirebaseManager
{
	public static FirebaseServices currentFirebaseInstance;
	public static Activity currentActivity;

	public static void init(Activity activity)
	{
		currentActivity = activity;
		currentFirebaseInstance = new FirebaseServices(activity);

		if (AppConfig.UseFirebaseAnalystic)
		{
			currentFirebaseInstance.initAnalytics(currentActivity);
		}
	}

	public static boolean FirebaseAnalyticsLogEvent(String itemId, String itemName, String itemValue, String itemType)
	{
		if (AppConfig.UseFirebaseAnalystic)
		{
			return currentFirebaseInstance.logEvent(itemId, itemName, itemValue, itemType);
		}
		else
		{
			Log.i("DEBUG", "Firebase analytics is on turn off! see string.xml");
			return false;
		}
	}
}