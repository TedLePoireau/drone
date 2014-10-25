package pfee.epita.fr.pfee_paramoteur;

import android.app.Activity;

import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.TextView;

import pfee.epita.fr.pfee_paramoteur.R;

/**
 * Created by Iggy on 27/05/2014.
 */
public class View_sensors {

    public static TextView t_gps_lat = null;
    public static TextView t_gps_long = null;
    public static TextView t_battery = null;
    public static TextView t_conn = null;
    public static TextView t_compass = null;
    public static TextView t_attitude0 = null;
    public static TextView t_attitude1 = null;
    public static EditText t_log = null;
    public static Button b_conn = null;
    public static EditText e_ip = null;
    public static TextView mot_p = null;
    public static TextView mot_r = null;
    public static TextView mot_l = null;


    public static void init(Activity a)
    {
        t_gps_lat = (TextView) a.findViewById(R.id.t_gps_lat);
        t_gps_long = (TextView) a.findViewById(R.id.t_gps_long);
        t_battery = (TextView) a.findViewById(R.id.t_batt);
        t_conn = (TextView) a.findViewById(R.id.t_conn);
        t_compass = (TextView) a.findViewById(R.id.t_compass);
        t_attitude0 = (TextView) a.findViewById(R.id.t_attitude0);
        t_attitude1 = (TextView) a.findViewById(R.id.t_attitude1);
        e_ip = (EditText) a.findViewById(R.id.e_ip);
        b_conn = (Button) a.findViewById(R.id.b_conn);
        mot_p = (TextView) a.findViewById(R.id.mot_p);
        mot_r = (TextView) a.findViewById(R.id.mot_r);
        mot_l = (TextView) a.findViewById(R.id.mot_l);

        //t_compass2 = (TextView) a.findViewById(R.id.t_compass2);
        //t_log = (EditText) a.findViewById(R.id.t_log);
    }

    public static TextView getT_gps_lat() {
        return t_gps_lat;
    }

    public static void setT_gps_lat(TextView t_gps) {
        View_sensors.t_gps_lat = t_gps;
    }
    public static TextView getT_gps_long() {
        return t_gps_long;
    }

    public static void setT_gps_long(TextView t_gps) {
        View_sensors.t_gps_long = t_gps;
    }

    public static TextView getT_battery() {
        return t_battery;
    }



    public static TextView getT_conn() {
        return t_conn;
    }



    public static TextView getT_attitude0() { return t_attitude0; }
    public static TextView getT_attitude1() { return t_attitude1; }
    public static TextView getT_compass() { return t_compass; }

}
