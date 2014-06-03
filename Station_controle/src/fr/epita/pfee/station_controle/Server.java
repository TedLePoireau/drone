package fr.epita.pfee.station_controle;

import java.awt.Color;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;


public class Server extends Thread{
	
	private MainWindow mf;
	int port;
	private BufferedReader in;
	private PrintWriter out;
	private ServerSocket server_socket;
	
	public Server(int port)
	{
		this.mf = MainWindow.get;
		this.port = port;
	}
	
	private int ParseLine(String s)
	{
		if (s.isEmpty())
			return -1;
		if (s.contains("QUIT"))
			return 0;
		switch (s.charAt(0))
		{
		case 'U':
			return ParseUpdate(s);
		default :
			return -1;
		}

	}
	
	private int ParseUpdate(String s)
	{
		switch (s.substring(2, 5))
		{
			case "GPS":	
				String coord[] = s.substring(6).split(":");
				mf.lbl_gps_lat.setText(coord[0] + " (latitute) ");
				mf.lbl_gps_long.setText(coord[1] + " (longitude) ");
				break;
			case "BAT":
				mf.lbl_batt.setText(s.substring(6) + "%"); 
				break;
			default :
				return -1;
		}
		System.out.println(s.substring(2, 5));
		return 1;

	}
	
	public void sendUpdateMotor(Motor m, int value)
	{
		String q = "U:MOT:" + m + ":" + String.valueOf(value);
		out.write(q + "\n");
		out.flush();
	}
	
	public void close()
	{

        try {
        	if (server_socket != null)
        		server_socket.close();
        	if (out != null)
        		out.close();
        	if (in != null)
        		in.close();
        	Settings.get.connecte = false;

		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
        mf.reset_ui();
	}
	
    @Override
    public void run() {
        try {
        	server_socket = new ServerSocket(this.port);
            mf.btn_disconnect.setEnabled(true);
            Socket client = server_socket.accept();
            mf.btn_listen.setEnabled(false);
            mf.lbl_status.setText("Connected");
			mf.lbl_status.setForeground(new Color(0,105,0));
            in = new BufferedReader(new InputStreamReader(client.getInputStream()));
            out = new PrintWriter(client.getOutputStream(), true);

            Settings.get.connecte = true;
            
            String line;
            int ret;
            while ((line = in.readLine()) != null) {
                System.out.println(line);
                mf.txt_log.append(line+ "\n");
                ret = ParseLine(line);
                if (ret < 0)
                	mf.txt_log.append("Error parsing this line, ignoring it"+ "\n");
                if (ret == 0)
                	break;
            }
            close();

        } catch (Exception e) {
            close();
        }
    }


}
