//Pine-entertainment
//Android Framework
//duy.nguyen 2014 - 2015

package pine.core.Actions;
import pine.core.*;
import pine.core.GameSparkServices.GameSparkManager;
import pine.core.GooglePlay.GooglePlayManager;

//#[001] import com.facebook.*;
//#[001] import com.facebook.model.*;

import 	android.util.Log;

public class ActionGameSparkLogin implements Action
{

	ActionGameSparkLoginArg Arg = null;
	public ActionGameSparkLogin( ActionGameSparkLoginArg arg)
	{
		Arg = arg;
	}
	
	public void act()
	{
		if(Arg == null )
		{
			Log.i("Debug", "action login game spark error");
			return;
		}
		Log.i("DEBUG","begin do action login game spark with facebook access token: " + FacebookManager.GetCurrentAccessToken());
		Arg.onBegin();
		if (GameSparkManager.isInit()) {
			GameSparkManager.LinkFacebookAccount(Arg, FacebookManager.GetCurrentAccessToken(), true);
		}
	}
}