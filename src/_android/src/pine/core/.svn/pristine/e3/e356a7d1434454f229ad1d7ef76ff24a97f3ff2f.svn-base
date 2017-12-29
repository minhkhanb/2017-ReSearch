package pine.core.Firebase;


import android.app.Activity;
import android.os.Bundle;
import android.util.Log;

import com.google.firebase.analytics.FirebaseAnalytics;

import pine.core.AppConfig;

class FirebaseServices
{
	private Activity CurrentActivity;
	private FirebaseAnalytics mFirebaseAnalytics;

	public FirebaseServices(Activity activity){
		CurrentActivity = activity;
	}

	public void initAnalytics(Activity activity)
	{
		if (AppConfig.UseFirebaseAnalystic) {
			mFirebaseAnalytics = FirebaseAnalytics.getInstance(CurrentActivity.getBaseContext());
		}
	}

	public boolean logEvent (String itemId, String itemName, String itemValue, String itemType) {
		if (AppConfig.UseFirebaseAnalystic) {
			if (mFirebaseAnalytics == null) {
				Log.i("DEBUG", "Firebase analytics not instance is null!");
				return false;
			}
			try {
				Bundle bundle = new Bundle();
				bundle.putString(FirebaseAnalytics.Param.ITEM_ID, itemId);
				bundle.putString(FirebaseAnalytics.Param.ITEM_NAME, itemName);
				bundle.putString(FirebaseAnalytics.Param.VALUE, itemValue);
				bundle.putString(FirebaseAnalytics.Param.CONTENT_TYPE, itemType);
				mFirebaseAnalytics.logEvent(FirebaseAnalytics.Event.SELECT_CONTENT, bundle);
				return true;
			} catch (Exception ex) {
				Log.i("DEBUG", "Firebase logevent fail with exception: " + ex.getMessage());
				return false;
			}
		}
		return false;
	}
}