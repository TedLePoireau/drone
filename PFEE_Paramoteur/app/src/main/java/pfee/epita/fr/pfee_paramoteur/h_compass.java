package pfee.epita.fr.pfee_paramoteur;

import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;

public class h_compass implements SensorEventListener {
    private SensorManager mSensorManager;
    private Sensor mAccelerometer, mMagnetic;
    private float[] valuesAccelerometer;
    private float[] valuesMagneticField;

    private float[] matrixR;
    private float[] matrixI;
    private float[] matrixValues;

    public h_compass() {
        mSensorManager = (SensorManager)MainActivity.activity.getSystemService(Context.SENSOR_SERVICE);
        mMagnetic = mSensorManager.getDefaultSensor(Sensor.TYPE_MAGNETIC_FIELD );
        mAccelerometer = mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER );
        mSensorManager.registerListener(this, mAccelerometer, SensorManager.SENSOR_DELAY_NORMAL);
        mSensorManager.registerListener(this, mMagnetic, SensorManager.SENSOR_DELAY_NORMAL);
        valuesAccelerometer = new float[3];
        valuesMagneticField = new float[3];
        matrixR = new float[9];
        matrixI = new float[9];
        matrixValues = new float[3];
    }


    public void onAccuracyChanged(Sensor sensor, int accuracy) {
        //View_sensors.getT_compass().setText(String.valueOf(sensor.getName()));
    }

    public void onSensorChanged(SensorEvent event) {
        switch(event.sensor.getType()){
            case Sensor.TYPE_ACCELEROMETER:
                for(int i =0; i < 3; i++){
                    valuesAccelerometer[i] = event.values[i];
                }
                break;
            case Sensor.TYPE_MAGNETIC_FIELD:
                for(int i =0; i < 3; i++){
                    valuesMagneticField[i] = event.values[i];
                }
                break;
        }

        boolean success = SensorManager.getRotationMatrix(
                matrixR,
                matrixI,
                valuesAccelerometer,
                valuesMagneticField);

        if(success){
            SensorManager.getOrientation(matrixR, matrixValues);

            double azimuth = Math.toDegrees(matrixValues[0]);
            azimuth = (double)Math.round((azimuth * 10) / 10);
            double pitch = Math.toDegrees(matrixValues[1]);
            pitch = (double)Math.round((pitch * 10) / 10);
            double roll = Math.toDegrees(matrixValues[2]);
            roll = (double)Math.round((roll * 10) / 10);

            View_sensors.getT_compass().setText(String.valueOf(azimuth));
            Server.sendRotUpdate((int)azimuth);
            View_sensors.getT_attitude0().setText(String.valueOf(pitch));
            Server.sendPitUpdate((int)pitch);
            View_sensors.getT_attitude1().setText(String.valueOf(roll));
            Server.sendRolUpdate((int)roll);

            //myCompass.update(matrixValues[0]);
            //View_sensors.getT_compass().setText(String.valueOf((double)Math.round(event.values[0] * 10) / 10));
        }


    }
}
