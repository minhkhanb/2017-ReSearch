//Pine-entertainment
//Android Framework
//duy.nguyen 2014 - 2015

package pine.core.Actions;
import pine.core.*;

//#[001] import com.facebook.*;
//#[001] import com.facebook.model.*;

import 	android.util.Log;

public class ActionFacebookDownloadAvatar implements Action
{

	ActionArg Arg = null;
	public ActionFacebookDownloadAvatar( ActionArg arg)
	{
		Arg = arg;
	}
	
	public void act()
	{
		if(Arg == null )
		{
			Log.i("Debug", "action download facebook avatar error");
			return;
		}
		Log.i("DEBUG","begin do action download facebook avatar");
		Arg.onBegin();
		
		FacebookManager.downloadAvatar((ActionFacebookDownloadAvatarArg)Arg);
	}
}