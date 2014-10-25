package pfee.epita.fr.pfee_paramoteur;

import android.content.Context;
import android.hardware.usb.UsbManager;
import android.util.Log;

import com.hoho.android.usbserial.driver.UsbSerialDriver;
import com.hoho.android.usbserial.driver.UsbSerialProber;

import java.io.IOException;

/**
 * Created by IggY on 22/09/2014.
 */
public class SerialCom {

    public SerialCom()
    {
        // Get UsbManager from Android.
        UsbManager manager = (UsbManager) MainActivity.activity.getSystemService(Context.USB_SERVICE);
        // Find the first available driver.
        UsbSerialDriver driver = UsbSerialProber.acquire(manager);

        if (driver != null) {
            try {
                driver.open();
                driver.setBaudRate(115200);

                byte buffer[] = new byte[16];
                int numBytesRead = driver.read(buffer, 1000);
                //Log.d(TAG, "Read " + numBytesRead + " bytes.");
            } catch (IOException e) {
                // Deal with error.
            } finally {
                try {
                    driver.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }
}
