package pine.framework;
import pine.core.*;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.util.DisplayMetrics;
import android.content.Context;
import 	android.widget.Toast;
import 	android.content.Intent;
import java.util.*;

import org.odata4j.consumer.ODataConsumer;
import org.odata4j.jersey.consumer.ODataJerseyConsumer;
import org.odata4j.jersey.consumer.ODataJerseyConsumer.Builder;
import org.odata4j.core.OEntity;
import org.odata4j.core.OFuncs;
import org.odata4j.core.OProperties;
import com.sun.jersey.api.client.filter.HTTPBasicAuthFilter;
import org.odata4j.consumer.behaviors.BasicAuthenticationBehavior;
import org.odata4j.consumer.ODataClientRequest;

import org.odata4j.consumer.behaviors.*;
import org.odata4j.jersey.consumer.behaviors.*;

import android.content.pm.PackageInfo;
import 	android.util.Base64;

//chartboost
import com.chartboost.sdk.*;

//facebook 
import com.facebook.*;
import com.facebook.android.*;
import com.facebook.model.*;
import com.facebook.android.Facebook.DialogListener;
import org.json.JSONObject;
import java.io.IOException;
import java.security.NoSuchAlgorithmException;
import java.security.MessageDigest;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.pm.Signature;

//
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.RelativeLayout.*;

public class MainActivity extends Activity{
	class TestOption{
		static final boolean testChartBoost = false;
		static final boolean testOdata = false;
		static final boolean testFacebook = false;
	}
	///GL2JNIView mView;
    /** Called when the activity is first created. */
	static{
        System.loadLibrary("pine_framework");
    }
	
	pine.core.GLJNISurfaceView GLView;
	pine.core.GLJNIRenderer GLRenderer;
	
	private Chartboost cb;
	
	
	//
	FacebookManager facebook;
	
	/*Facebook facebook;
	AsyncFacebookRunner asyncfbrunner;
	String FACEBOOK_APP_ID = "241150389408711";
	*/
    @Override
    public void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
		// Init Pine Core Application Context
		pine.core.AppContext.Init(this, savedInstanceState);
		
		// Create GLView
		GLView = new pine.core.GLJNISurfaceView(getApplication());
		GLRenderer = new pine.core.GLJNIRenderer();
		GLView.setRenderer(GLRenderer);
		//setContentView(GLView);
		
		RelativeLayout rl = new RelativeLayout(this);
		rl.addView(GLView);        
		TextView tv = new TextView(this);
		RelativeLayout.LayoutParams lp = new RelativeLayout.LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
		lp.addRule(RelativeLayout.ALIGN_TOP);
		tv.setLayoutParams(lp);
		tv.setText("FPS: 0");
		tv.setBackgroundColor(0x4060ff70);
		rl.addView(tv);

		setContentView(rl);
	
		//DisplayMetrics display_matric = new DisplayMetrics();
		//getWindowManager().getDefaultDisplay().getMetrics(display_matric);
		//int device_width = display_matric.widthPixels;
		//int device_height = display_matric.heightPixels;
		
		// Test odata4j
		if(TestOption.testOdata)
		{
			OClientBehavior cbehavior = new BasicAuthenticationBehavior("trieunk", "FPT0312S1");
			ODataJerseyConsumer.Builder builder = ODataJerseyConsumer.newBuilder("http://odata.ya.pine-entertainment.com/api/pocketarmy/");
			builder = builder.setClientBehaviors(cbehavior);
			ODataConsumer c = builder.build();
			try{
				BasicAuthenticationBehavior auth = new BasicAuthenticationBehavior("trieunk", "FPT0312S1");
				ODataClientRequest request = auth.transform(new ODataClientRequest("GET", "http://odata.ya.pine-entertainment.com/api/pocketarmy/", null, null, null));
				OEntity havinaCola = c.getEntity("SaveGames", 1790683886).execute();
			}catch(Exception ex){
				Log.i("PINE",ex.toString());
			}
			try{
				OEntity havinaCola = c.getEntity("SaveGames", 1790683886).execute();
			}catch(Exception ex){
				Log.i("PINE",ex.toString());
			}
		}
		
		// Configure Chartboost
		if(TestOption.testChartBoost)
		{
			this.cb = Chartboost.sharedChartboost();
			String appId = "53be07f11873da0cb140ce07";
			String appSignature = "75a3200e7af9cc93ecd7b15ac0c53998a5233cde";
			this.cb.onCreate(this, appId, appSignature, null);
		}
		
		// Configure Facebook sdk
		if(TestOption.testFacebook){
			facebook = new FacebookManager(this, savedInstanceState);
			facebook.login();
		}
		/*facebook = new Facebook(FACEBOOK_APP_ID);
		asyncfbrunner = new AsyncFacebookRunner(facebook);
		loginFacebook();
		*/
		
		//test Stream Buffer
		test.testStreamBuffer();
    }
	@Override
	protected void onStart() {
		super.onStart();
		if(TestOption.testChartBoost)
		{
			this.cb.onStart(this);
			// Show an interstitial. This and other interstital/MoreApps cache/show calls should be used after onStart().
			this.cb.showInterstitial(); 
		}
	}  

	@Override protected void onPause() {
        super.onPause();
        GLView.onPause();
    }

    @Override protected void onResume() {
        super.onResume();
        GLView.onResume();
    }
	
	// In onStop()
	@Override
	protected void onStop() { 
		super.onStop(); 
		if(TestOption.testChartBoost)
		{
			this.cb.onStop(this); 
		}
	}
	
	// In onDestroy()
	@Override
	protected void onDestroy() 
	{ 
		if(TestOption.testChartBoost)
		{
			this.cb.onDestroy(this); 
		}
		super.onDestroy(); 
	}
	// In onBackPressed()
	@Override
	public void onBackPressed() 
	{ 
		// If an interstitial is on screen, close it. Otherwise continue as normal. 
		if(TestOption.testChartBoost)
		{
			if (this.cb.onBackPressed()) 
				return; 
			else 
				super.onBackPressed(); 
		}else{
			super.onBackPressed();
		}
	}
	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);
		Log.i("DEBUG","on activity result:" + requestCode + ":" + resultCode + " :");
		Session.getActiveSession().onActivityResult(this, requestCode, resultCode, data);
	}
	/*
	private void loginFacebook() {

		if (!facebook.isSessionValid()) {

			facebook.authorize(this, new String[] { "email", "publish_stream","read_stream" }, new LoginDialogListener());

		} else {

			getProfileInformation();

		}

	}
	public void getProfileInformation() {
		try {
			Log.e("PINE", "loading facebook user infomation");
			/*JSONObject profile = Util.parseJson(facebook.request("me"));
			Log.e("Profile", "" + profile);

			String mUserId = profile.getString("id");
			String mUserToken = facebook.getAccessToken();
			String mUserName = profile.getString("name");
			String mUserEmail = profile.getString("email");
			Log.i("PINE",""+mUserId+"\n"+mUserToken+"\n"+mUserName+"\n"+mUserEmail);
			/*runOnUiThread(new Runnable() {

				public void run() {

					Log.e("FaceBook_Profile",""+mUserId+"\n"+mUserToken+"\n"+mUserName+"\n"+mUserEmail);

					/*Toast.makeText(getApplicationContext(),
							"Name: " + mUserName + "\nEmail: " + mUserEmail,
							Toast.LENGTH_LONG).show();*/



			//	}

			//});*/
			/*

		} /*catch (FacebookError e) {

			e.printStackTrace();
		} catch (MalformedURLException e) {

			e.printStackTrace();
		} catch (JSONException e) {

			e.printStackTrace();
		} catch (IOException e) {

			e.printStackTrace();
		}*//*
		catch (Exception ex){
			ex.printStackTrace();
		}

	}
	class LoginDialogListener implements DialogListener {

		public void onComplete(Bundle values) {
			try {

				getProfileInformation();

			} catch (Exception error) {
				/*Toast.makeText(LoginActivity.this, error.toString(),
						Toast.LENGTH_SHORT).show();*//*
						error.printStackTrace();
			}
		}

		public void onFacebookError(FacebookError error) {
			/*Toast.makeText(LoginActivity.this,
					"Something went wrong. Please try again.",
					Toast.LENGTH_LONG).show();*//*
		}

		public void onError(DialogError error) {
			/*Toast.makeText(LoginActivity.this,
					"Something went wrong. Please try again.",
					Toast.LENGTH_LONG).show();*//*
		}

		public void onCancel() {
			/*Toast.makeText(LoginActivity.this,
					"Something went wrong. Please try again.",
					Toast.LENGTH_LONG).show();*//*
		}
	}
	*/
}
