//Pine-entertainment
//Android Framework
//duy.nguyen 2014 - 2015

package pine.core;
import 	android.content.Context;
import android.net.Network;
import 	android.util.Log;
import android.content.res.AssetFileDescriptor;
import java.io.FileDescriptor;
import java.lang.reflect.Field;
import android.R;
//network
import android.net.NetworkInfo;
import android.net.ConnectivityManager;
import android.content.IntentFilter;

public class OsFunctions{
	public static boolean _is_init = false;
	static boolean _is_nerwork_available = false;
	//static android.R Resource;
	static public void Init(){
		//if(!_is_init){
			//check network available
			_is_nerwork_available = IsNetworkAvailable();
			Log.i("PLATFORM",_is_nerwork_available?"network available":"network not available");
			//init native
			nativeInit();
			_is_init = true;
		//}
	}

	static public int GetDeviceOsVersion()
	{
		return android.os.Build.VERSION.SDK_INT;
	}

	static public boolean IsNetworkAvailable(){
		try{
			if(AppContext.CurrentContext != null){
				ConnectivityManager conxMgr = (ConnectivityManager)AppContext.CurrentContext.getSystemService(AppContext.CurrentContext.CONNECTIVITY_SERVICE);
				NetworkInfo netInfo = conxMgr.getActiveNetworkInfo();
				if (netInfo != null && netInfo.isConnectedOrConnecting()) {
					return true;
				}
			}
		}catch(Exception ex){
		}
		return false;
	}
	static public String GetResourcePath(){
		//Log.i("SGLOG",AppContext.CurrentContext.getApplicationInfo().sourceDir);
		//Log.i("SGLOG",AppContext.CurrentContext.getApplicationInfo().publicSourceDir);
		//Log.i("SGLOG",AppContext.CurrentContext.getApplicationInfo().nativeLibraryDir);
		return AppContext.CurrentContext.getApplicationInfo().sourceDir;
	}
	static public String GetDataPath(){
		//Log.i("SGLOG",AppContext.CurrentContext.getApplicationInfo().sourceDir);
		//Log.i("SGLOG",AppContext.CurrentContext.getApplicationInfo().publicSourceDir);
		//Log.i("SGLOG",AppContext.CurrentContext.getApplicationInfo().nativeLibraryDir);
		if(AppContext.CurrentContext == null) Log.i("DEBUG","error getPackageName");
		Log.i("DEBUG", AppContext.CurrentContext.getPackageName());
		return AppContext.CurrentContext.getPackageName();
	}
	static public void ProcessServiceStream(byte[] service_data){
		//receive stream to process
		ServiceStream stream = new ServiceStream(service_data);
		Game.processService(stream);
	}
	static public void ReportServiceStream(byte[] service_data){
		//response after process
		nativeReportService(service_data);
	}
	static public int GetAssetOffset(String filename) {
		/*int filesize=0;
		try {
			android.content.res.AssetFileDescriptor fd=AppContext.CurrentContext.getAssets().openFd(filename);
			filesize=(int)fd.getStartOffset();
		} catch(java.io.IOException e) {
			Log.i("ERROR",e.toString());
		}
		return filesize;
		*/
		
		int resourceID = AppContext.CurrentContext.getResources().getIdentifier(filename, "raw",AppContext.CurrentContext.getPackageName());
		//Log.i("ERROR","resid:" + resourceID);
		try{
			AssetFileDescriptor RawAssetsDescriptor = AppContext.CurrentContext.getResources().openRawResourceFd(resourceID);
			if (RawAssetsDescriptor != null){
				//FileDescriptor fd = RawAssetsDescriptor.getFileDescriptor();
				long off = RawAssetsDescriptor.getStartOffset();
				RawAssetsDescriptor.close();
				return (int)off;
			}
		}catch(Exception ex){
			Log.i("ERROR",filename + " "+ex.toString());
		}
		
		/*
		try {
			Class res = R.raw.class;
			Field field = res.getField(filename);
			int rawid = field.getInt(null);
			return rawid;
		}
		catch (Exception e) {
			Log.e("MyTag", "Failure to get drawable id.", e);
		}
		*/
		return 0;
	}
	
	static public boolean ScaleView(float scalex, float scaley)
	{
		Log.i("DEBUG", "change scale view:" + scalex + " sy:" + scaley);
		pine.core.AppContext.CurrentContentView.setScale(scalex, scaley);
		return true;
	}
	
	static public void setFacebookInfo(String id, String user_name)
	{
		nativeSetFacebookInfo(id, user_name);
	}
	
	private static native void nativeInit();
	private static native void nativeReportService(byte[] data);
	private static native void nativeSetFacebookInfo(String id, String user_name);
}