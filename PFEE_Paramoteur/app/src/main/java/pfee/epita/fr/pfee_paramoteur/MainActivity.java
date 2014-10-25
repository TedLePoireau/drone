package pfee.epita.fr.pfee_paramoteur;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;


public class MainActivity extends Activity {

    public static Activity activity = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {


        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        activity = this;
        View_sensors.init(this);

        h_battery hb = new h_battery(this.getApplicationContext());
        h_compass hc = new h_compass();
        //h_assiette ha = new h_assiette();
        h_gps hg = new h_gps();
        final Server serv = new Server();
        View_sensors.b_conn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                serv.execute(View_sensors.e_ip.getText().toString());
            }
        });

    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.my, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();
        if (id == R.id.action_settings) {
            return true;
        }
        return super.onOptionsItemSelected(item);
    }
}
