package pine.core;
import java.util.*;

import pine.core.Actions.ActionArg;
import pine.core.Actions.ActionFacebookLoginArg;
import pine.core.Actions.ActionUnityVideoAdsAllowSkipShow;
import pine.core.Actions.ActionUnityVideoAdsAllowSkipShowArg;
import pine.core.Actions.ActionUnityVideoAdsShow;
import pine.core.Actions.ActionUnityVideoAdsShowArg;
import pine.core.Actions.ActionVungleVideoAdsShow;
import pine.core.Actions.ActionVungleVideoAdsShowArg;

import android.app.Activity;
import android.util.Log;
import com.vungle.publisher.EventListener;

public class VungleAdsManager{
	public static final EventListener vungleListener = new EventListener(){

		@Deprecated
		@Override
		public void onVideoView(boolean isCompletedView, int watchedMillis, int videoDurationMillis) {
			// This method is deprecated and will be removed. Please do not use it.
			// Please use onAdEnd instead.
		}

		@Override
		public void onAdStart() {
			// Called before playing an ad
		}

		@Override
		public void onAdEnd(boolean wasSuccessfulView, boolean wasCallToActionClicked) {
			// Called when the user leaves the ad and control is returned to your application
			// if wasSuccessfulView is true, the user watched the ad and should be rewarded
			// (if this was a rewarded ad).
			// if wasCallToActionClicked is true, the user clicked the call to action
			// button in the ad.
			Log.i("DEBUG", "Show vungle ads end success = " + wasSuccessfulView);

			onVideoCompleted(wasSuccessfulView);
		}

		@Override
		public void onAdPlayableChanged(boolean isAdPlayable) {
			// Called when the playability state changes. if isAdPlayable is true, you can now
			// play an ad.
			// If false, you cannot yet play an ad.
		}

		@Override
		public void onAdUnavailable(String reason) {
			// Called when VunglePub.playAd() was called, but no ad was available to play
		}

	};

	static private ArrayList<ActionArg> _lst_waiting_args = new ArrayList<ActionArg>();
	static private Activity CurrentActivity;
	
	static public void init(Activity activity){
		CurrentActivity = activity;
	}

	static public void showVideoAds(ActionVungleVideoAdsShowArg arg)
	{
		_lst_waiting_args.add(arg);
		ActionVungleVideoAdsShow act_vungle_show_video = new ActionVungleVideoAdsShow(arg, CurrentActivity);
		act_vungle_show_video.act();
	}


	static public void onVideoCompleted(boolean success)
	{
		ArrayList<ActionArg> tmp = new ArrayList<ActionArg>();

		for(int s = 0; s< _lst_waiting_args.size(); s++)
		{
			ActionArg arg = _lst_waiting_args.get(s);
			if(arg instanceof ActionVungleVideoAdsShowArg)
			{
				tmp.add(arg);
			}
		}

		if(tmp.size() > 0) Log.i("DEBUG", "Has pending ActionVungleVideoAdsShowArg");

		for(int i = 0; i< tmp.size(); i++){
			ActionArg arg = tmp.get(i);
			_lst_waiting_args.remove(arg);
			if (success) {
				arg.onDone();
			}
			else
			{
				arg.onCancel();
			}
		}
	}
}