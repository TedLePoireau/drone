package pfee.epita.fr.pfee_paramoteur;

import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;

public class h_assiette implements SensorEventListener {
    private SensorManager mSensorManager;
    private Sensor mAccelerometer;

    public h_assiette() {
        mSensorManager = (SensorManager)MainActivity.activity.getSystemService(Context.SENSOR_SERVICE);
        mAccelerometer = mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER );
        mSensorManager.registerListener(this, mAccelerometer, SensorManager.SENSOR_DELAY_GAME);
    }

    public void onAccuracyChanged(Sensor sensor, int accuracy) {
        //View_sensors.getT_compass().setText(String.valueOf(sensor.getName()));
    }

    public void onSensorChanged(SensorEvent event) {
        //View_sensors.getT_compass().setText("LOL");
        View_sensors.getT_attitude0().setText(String.valueOf((double)Math.round(event.values[0] * 10) / 10));
        View_sensors.getT_attitude1().setText(String.valueOf((double)Math.round(event.values[1] * 10) / 10));
    }
}
