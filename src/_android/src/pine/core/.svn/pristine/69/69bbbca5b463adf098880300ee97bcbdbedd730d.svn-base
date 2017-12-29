package pine.core;
import java.util.*;
import 	android.util.Log;

import pine.core.Actions.ActionArg;
import pine.core.Admob.*;

import android.app.Activity;
import android.view.View;
import android.widget.LinearLayout;
//admob
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.AdView;
import com.google.android.gms.ads.InterstitialAd;
import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.MobileAds;
import com.google.android.gms.ads.reward.RewardItem;
import com.google.android.gms.ads.reward.RewardedVideoAd;
import com.google.android.gms.ads.reward.RewardedVideoAdListener;

public class AdmobManager{
	static private ArrayList<AdmobWaitingLoadCallback> _waiting = new ArrayList<AdmobWaitingLoadCallback>();
	static private ArrayList<InterstitialAd> _lst_ads = new ArrayList<InterstitialAd>();
	static private ArrayList<ActionArg> _lst_waiting_args = new ArrayList<ActionArg>();
	static private InterstitialAd _interstitialAd;
	static private AdView _adView;
	static private ActionArg _waiting_banner_arg;
	static private boolean _adBannerLoaded = false;
	//static private RewardedVideoAd _videoAds;
	static private Activity CurrentActivity;

	static public void addWaitingInterstitialCallback(AdmobWaitingLoadCallback callback){
		_waiting.add(callback);
	}
	static public void removeWaitingInterstitialCallback(AdmobWaitingLoadCallback callback){
		_waiting.remove(callback);
	}
	
	static public void init(Activity activity){
		CurrentActivity = activity;
		MobileAds.initialize(CurrentActivity, AppConfig.StringAdmobUnitId);
	}

	static public void initAdmobInterstitial ()
	{
		_interstitialAd = new InterstitialAd(CurrentActivity);
		_interstitialAd.setAdUnitId(AppConfig.StringAdmobUnitId);
		_interstitialAd.setAdListener(new AdListener(){
			@Override
			public void onAdLoaded() {
				//for(int i = 0; i< AdmobManager._waiting.size(); i++){
				//AdmobManager._waiting.get(i).onDone(true);
				//}
				for (int i = 0; i < _lst_waiting_args.size(); i++)
				{
					_lst_waiting_args.get(i).onDone();
					_lst_waiting_args.remove(i);
				}
			}

			@Override
			public void onAdFailedToLoad (int errorCode) {
				//for(int i = 0; i< AdmobManager._waiting.size(); i++){
				//AdmobManager._waiting.get(i).onDone(false);
				//}
				for (int i = 0; i < _lst_waiting_args.size(); i++)
				{
					_lst_waiting_args.get(i).onCancel();
					_lst_waiting_args.remove(i);
				}
			}

			@Override
			public void onAdClosed(){
				Log.i("DEBUG","Ads Closed");
				for (int i = 0; i < _lst_waiting_args.size(); i++)
				{
					_lst_waiting_args.get(i).onDone();
					_lst_waiting_args.remove(i);
				}
				for(int i = 0; i< _lst_ads.size(); i++){
					InterstitialAd tmp = _lst_ads.get(i);
					if(tmp.getAdListener () == this){
						_lst_ads.remove(tmp);
						Log.i("DEBUG","Ads Removed: remain:" + _lst_ads.size());
						break;
					}
				}
			}
		});
	}

	static public boolean isPrepareInterstitialCompleted(){
		return _interstitialAd.isLoaded();
	}

	static public void prepareInterstitial(ActionArg arg){
		AdRequest adRequest = new AdRequest.Builder().addTestDevice("88E5FBEB490FEB54DE0B654913F0ADE9").build();
        _interstitialAd.loadAd(adRequest);
		_lst_waiting_args.add(arg);
		//_lst_ads.add(_interstitialAd);
	}
	
	static public boolean showInterstitial(ActionArg arg){
		/*for(int i = 0; i< _lst_ads.size(); i++){
			InterstitialAd tmp = _lst_ads.get(i);
			if (tmp != null && tmp.isLoaded()) {
				tmp.show();
				_lst_waiting_args.add(arg);
				return true;
			}
		}*/
		if (_interstitialAd != null && _interstitialAd.isLoaded())
		{
			_interstitialAd.show();
			_lst_waiting_args.add(arg);
			return true;
		}
		else {
			arg.onCancel();
			return false;
		}
	}

	static public void initAdmobBanner (AdView ad_view)
	{
		Log.i("DEBUG", "Init admob banner!");
		_adView = ad_view;
		_adView.setAdUnitId(AppConfig.StringAdmobBannerUnitId);
		_adView.setAdSize(AdSize.BANNER);
		_adView.setVisibility(View.GONE);
	}

	static public void prepareBannerAds(ActionArg arg)
	{
		if (_adView != null) {
			_waiting_banner_arg = arg;
			AdRequest adRequest = new AdRequest.Builder().addTestDevice("88E5FBEB490FEB54DE0B654913F0ADE9").build();
			_adView.setAdListener(new AdListener() {
				@Override
				public void onAdClosed() {
					if (_waiting_banner_arg != null)
					{
						_waiting_banner_arg.onCancel();
						_waiting_banner_arg = null;
					}
					_adBannerLoaded = false;
					Log.i("DEBUG", "admob banner onAdClosed");
					super.onAdClosed();
				}

				@Override
				public void onAdFailedToLoad(int i) {
					_adBannerLoaded = false;
					if (_waiting_banner_arg != null)
					{
						_waiting_banner_arg.onCancel();
						_waiting_banner_arg = null;
					}
					Log.i("DEBUG", "admob banner onAdFailedToLoad");
					super.onAdFailedToLoad(i);
				}

				@Override
				public void onAdOpened() {
					Log.i("DEBUG", "admob banner onAdOpened");
					super.onAdOpened();
				}

				@Override
				public void onAdLeftApplication() {
					Log.i("DEBUG", "admob banner onAdLeftApplication");
					super.onAdLeftApplication();
				}

				@Override
				public void onAdLoaded() {
					Log.i("DEBUG", "admob banner onAdLoaded");
					if (_waiting_banner_arg != null)
					{
						_waiting_banner_arg.onDone();
						_waiting_banner_arg = null;
					}
					_adBannerLoaded = true;
					super.onAdLoaded();
				}
			});
			_adView.loadAd(adRequest);
		}
		else
		{
			Log.i("DEBUG", "AdView for admob banner is null");
			arg.onCancel();
		}
	}

	static public boolean showBannerAds(ActionArg arg){
		if (_adView != null && _adBannerLoaded)
		{
			_adView.setVisibility(View.VISIBLE);
			arg.onDone();
			return true;
		}
		else {
			arg.onCancel();
			return false;
		}
	}

	static public boolean hideBannerAds(ActionArg arg){
		if (_adView != null)
		{
			_adView.setVisibility(View.GONE);
			arg.onDone();
			return true;
		}
		else {
			arg.onCancel();
			return false;
		}
	}


	static public void onPause()
	{
		if (_adView != null) {
			_adView.pause();
		}
	}

	static public void onResume()
	{
		if (_adView != null) {
			_adView.resume();
		}
	}

	static public void onDestroy()
	{
		if (_adView != null) {
			_adView.destroy();
		}
	}
	/*
	static public RewardedVideoAd getVideoAdsInstance () {
		if (_videoAds == null)
		{
			prepareVideo ();
		}
		return _videoAds;
	}

	static public boolean isPrepareVideoCompleted(){
		return _videoAds.isLoaded();
	}

	static public void prepareVideo ()
	{
		_videoAds = MobileAds.getRewardedVideoAdInstance(CurrentActivity);
		_videoAds.setRewardedVideoAdListener(new RewardedVideoAdListener() {
			@Override
			public void onRewardedVideoAdLoaded() {

			}

			@Override
			public void onRewardedVideoAdOpened() {

			}

			@Override
			public void onRewardedVideoStarted() {

			}

			@Override
			public void onRewardedVideoAdClosed() {

			}

			@Override
			public void onRewarded(RewardItem rewardItem) {

			}

			@Override
			public void onRewardedVideoAdLeftApplication() {

			}

			@Override
			public void onRewardedVideoAdFailedToLoad(int i) {

			}
		});
		_videoAds.loadAd(AppConfig.StringAdmobUnitId, new AdRequest.Builder().build());
	}*/
}