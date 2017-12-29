//Pine-entertainment
//Android Framework
//duy.nguyen 2014 - 2015

package pine.core.Actions;

import pine.core.AppContext;
import pine.core.AppRegistry;

public class ActionTakeScreenShot implements Action{
	ActionArg Arg = null;
	public ActionTakeScreenShot(ActionArg arg){
		Arg = arg;
	}
	public void act(){
		if(Arg == null){
			return;
		}
		Arg.onBegin();
		final ActionArg arg = Arg;
		AppContext.CurrentRegistry.regTakeScreenShot(new AppRegistry.TakeScreenShotCallBack(){
			public void onDone(){
				arg.onDone();
			}
		});
	}
}