package pfee.epita.fr.pfee_paramoteur;

import java.io.IOException;
import java.util.List;
import java.util.Locale;

import android.content.Context;
import android.location.Address;
import android.location.Geocoder;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Toast;

/**
 * Created by Iggy on 28/05/2014.
 */
public class h_gps implements LocationListener {
    private LocationManager lm;

    public h_gps()
    {
        lm = (LocationManager) MainActivity.activity.getSystemService(Context.LOCATION_SERVICE);
       // if (lm.isProviderEnabled(LocationManager.GPS_PROVIDER))
        lm.requestLocationUpdates(LocationManager.GPS_PROVIDER, 500, 0, this);
        lm.requestLocationUpdates(LocationManager.NETWORK_PROVIDER, 500, 0, this);
        lm.requestLocationUpdates(LocationManager.PASSIVE_PROVIDER, 500, 0, this);
        //LocationClient mLocationClient = new LocationClient(this, this, this);

        //Location mCurrentLocation;
        //mCurrentLocation = mLocationClient.getLastLocation();


    }

    @Override
    public void onLocationChanged(Location loc) {
        View_sensors.getT_gps_long().setText(String.valueOf(loc.getLongitude()));
        View_sensors.getT_gps_lat().setText(String.valueOf(loc.getLatitude()));
    }

    @Override
    public void onProviderDisabled(String provider) {}

    @Override
    public void onProviderEnabled(String provider) {}

    @Override
    public void onStatusChanged(String provider, int status, Bundle extras) {}
}