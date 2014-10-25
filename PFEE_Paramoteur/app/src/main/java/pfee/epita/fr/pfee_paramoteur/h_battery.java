package pfee.epita.fr.pfee_paramoteur;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.BatteryManager;

import java.util.Set;

/**
 * Created by Iggy on 27/05/2014.
 */
public class h_battery extends BroadcastReceiver{
    Intent batteryStatus;

    public h_battery(Context c)
    {
        IntentFilter ifilter = new IntentFilter(Intent.ACTION_BATTERY_CHANGED);
        batteryStatus = c.registerReceiver(this, ifilter);
        //batteryStatus = c.registerReceiver(null, ifilter);
        int level = batteryStatus.getIntExtra(BatteryManager.EXTRA_LEVEL, -1);
        int scale = batteryStatus.getIntExtra(BatteryManager.EXTRA_SCALE, -1);
        float batteryPct = level / (float)scale;

        View_sensors.getT_battery().setText(String.valueOf(batteryPct));
    }
    @Override
    public void onReceive(Context context, Intent intent) {
        int level = intent.getIntExtra(BatteryManager.EXTRA_LEVEL, -1);
        int scale = intent.getIntExtra(BatteryManager.EXTRA_SCALE, -1);
        float batteryPct = (level / (float)scale) * 100;


        View_sensors.getT_battery().setText(String.valueOf(batteryPct));
        Server.sendBatteryUpdate((int)batteryPct);
    }
}
