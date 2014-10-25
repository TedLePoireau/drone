package pfee.epita.fr.pfee_paramoteur;

import android.app.Activity;
import android.content.DialogInterface;
import android.os.AsyncTask;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.Inet4Address;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;

/**
 * Created by Iggy on 06/06/2014.
 */
public final class Server extends AsyncTask implements DialogInterface.OnClickListener{
    Socket socket;
    private static BufferedReader in;
    private static PrintWriter out;

    public Server()
    {
    }

    public static void sendBatteryUpdate(int batt)
    {
        if (out == null)
            return;
        out.write("U:BAT:" + String.valueOf(batt) + "\n");
        out.flush();
    }

    public static void sendRotUpdate(int rot)
    {
        if (out == null)
            return;
        out.write("U:ROT:" + String.valueOf(rot) + "\n");
        out.flush();
    }

    public static void sendPitUpdate(int pit)
    {
        if (out == null)
            return;
        out.write("U:PIT:" + String.valueOf(pit) + "\n");
        out.flush();
    }

    public static void sendGPSUpdate(double lat, double log)
    {
        if (out == null)
            return;
        out.write("U:GPS:" + String.valueOf(lat) + ":" + String.valueOf(log) + "\n");
        out.flush();
    }

    public static void sendRolUpdate(int rol)
    {
        if (out == null)
            return;
        out.write("U:ROL:" + String.valueOf(rol) + "\n");
        out.flush();
    }

    @Override
    protected Object doInBackground(Object[] objects) {
        try {
            String ip = (String) objects[0];
            //socket = new Socket(InetAddress.getByName("10.0.2.2"), 7777);
            socket = new Socket(InetAddress.getByName(ip), 7777);
            MainActivity.activity.runOnUiThread(new Runnable() {
                @Override
                public void run() {View_sensors.t_conn.setText("Connected");}});

            in = new BufferedReader(new InputStreamReader(this.socket.getInputStream()));
            out =  new PrintWriter(new BufferedWriter(new OutputStreamWriter(socket.getOutputStream())), true);
            Server.sendGPSUpdate(48.815398, 2.362927);

            MainActivity.activity.runOnUiThread(new Runnable() {
                @Override
                public void run() {View_sensors.t_gps_lat.setText("48.815398");
                    View_sensors.t_gps_long.setText("2.362927");}});
                    float b =  Float.parseFloat(View_sensors.t_battery.getText().toString());
                    Server.sendBatteryUpdate((int)b);

            try {
                String line;
                while ((line = in.readLine()) != null) {
                    final String tab[] = line.split(":");
                    if (tab[1].compareTo("MOT") == 0)
                    {
                        switch (Integer.parseInt(tab[2]))
                        {
                            case 0: MainActivity.activity.runOnUiThread(new Runnable() {
                                @Override
                                public void run() {View_sensors.mot_p.setText(tab[3]);}});
                                break;
                            case 1: MainActivity.activity.runOnUiThread(new Runnable() {
                                @Override
                                public void run() {View_sensors.mot_l.setText(tab[3]);}});
                                break;
                            case 2: MainActivity.activity.runOnUiThread(new Runnable() {
                                @Override
                                public void run() {View_sensors.mot_r.setText(tab[3]);}});
                                break;
                        }
                    }
 }
            } catch (IOException e) {
                e.printStackTrace();
            }
        } catch (UnknownHostException e1) {
            e1.printStackTrace();
        } catch (IOException e1) {
            e1.printStackTrace();
        }
        return null;
    }

    @Override
    public void onClick(DialogInterface dialogInterface, int i) {
        this.execute();
    }
}