package fr.epita.pfee.station_controle;

import java.awt.Color;
import java.awt.Font;
import java.awt.EventQueue;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JFormattedTextField;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.JSpinner;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.swing.text.DefaultFormatter;


public class MainWindow {

	public JFrame frame;
	public JTextField txt_port;
	public JTextArea txt_log;
	public JLabel lbl_status;
	public JLabel lbl_batt;
	public JLabel lbl_gps_lat;
	public JLabel lbl_gps_long;
	public JLabel lbl_tele;
	
	public JButton btn_listen;
	public JButton btn_disconnect;
	public JButton btn_toggle_tele;
	
	public static MainWindow get;
	
	private Server server;
	private Settings settings;

	public JButton btn_power_p_inc;
	public JButton btn_power_p_dec;
	public JButton btn_power_l_dec;
	public JButton btn_power_l_inc;
	public JButton btn_power_r_dec;
	public JButton btn_power_r_inc;
	public JSpinner spn_power_p;
	public JSpinner spn_power_l;
	public JSpinner spn_power_r;


	/**
	 * Create the application.
	 */
	private MainWindow() {
		initialize();
		reset_ui();
		reset_listeners();
		
		settings = new Settings();
		MainWindow.get = this;
	}
	
	
	public void reset_ui()
	{
		lbl_status.setText("Not listening");
		lbl_status.setForeground(new Color(125,0,0));
			
		txt_log.setText("Empty...\n");
	
		btn_listen.setText("Listen");
		btn_listen.setEnabled(true);
		
		btn_disconnect.setEnabled(false);

		lbl_tele.setForeground(new Color(125,0,0));
		
		spn_power_l.setEnabled(false);
		btn_power_l_inc.setEnabled(false);
		btn_power_l_dec.setEnabled(false);
		spn_power_r.setEnabled(false);
		btn_power_r_dec.setEnabled(false);
		btn_power_r_inc.setEnabled(false);
		spn_power_p.setEnabled(false);
		btn_power_p_inc.setEnabled(false);
		btn_power_p_dec.setEnabled(false);
	}
	
	public void reset_listeners()
	{
		btn_listen.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				if (!valid_input())
					return;
				btn_listen.setEnabled(false);
				lbl_status.setText("Listening on port : " + txt_port.getText());
				lbl_status.setForeground(new Color(255,128,64));
				
				btn_listen.setEnabled(false);
					
				server = new Server(Integer.valueOf(txt_port.getText()));
				
				server.start();
		}});
		
		btn_disconnect.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				server.close();
			}
		});
		
		btn_toggle_tele.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				settings.toggle_mode_telecomande();
			}
		});
		
	    spn_power_p.addChangeListener(new ChangeListener() {
	        @Override
	        public void stateChanged(ChangeEvent e) {
	        	int n = (int)spn_power_p.getValue();
	        	if (n < 0) n = 0;
	        	if (n > 100) n = 100;
	        	
	        	server.sendUpdateMotor(Motor.PROPULTION, n);
	        }
	    });
	    
	    spn_power_l.addChangeListener(new ChangeListener() {
	        @Override
	        public void stateChanged(ChangeEvent e) {
	        	int n = (int)spn_power_l.getValue();
	        	if (n < 0) n = 0;
	        	if (n > 100) n = 100;
	        	
	        	server.sendUpdateMotor(Motor.LEFT, n);
	        }
	    });
	    
	    spn_power_r.addChangeListener(new ChangeListener() {
	        @Override
	        public void stateChanged(ChangeEvent e) {
	        	int n = (int)spn_power_r.getValue();
	        	if (n < 0) n = 0;
	        	if (n > 100) n = 100;
	        	
	        	server.sendUpdateMotor(Motor.RIGHT, n);
	        }
	    });
	}
	
	private boolean valid_input()
	{
		if (txt_port.getText().isEmpty())
		{
			javax.swing.JOptionPane.showMessageDialog(null,"ERROR : Port is empty"); 
			return false;
		}
		
		if (Integer.valueOf(txt_port.getText()) <= 1024 || Integer.valueOf(txt_port.getText()) > 9999)
		{
			javax.swing.JOptionPane.showMessageDialog(null,"ERROR : Port must be > 1024 and < 9999"); 
			return false;
		}
		return true;
	}
	
	/**
	 * Launch the application.
	 */
	public static void main(String[] args) {
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try {
					MainWindow window = new MainWindow();
					window.frame.setVisible(true);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}
	
	/**
	 * All the dirty UI stuff.
	 * Nothing to see here (see init_ui_connect())
	 */
	private void initialize() {
		frame = new JFrame();
		frame.setBounds(100, 100, 830, 544);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.getContentPane().setLayout(null);
		
		//labels that wont change
		JLabel ui_lbl_status = new JLabel("Status :");
		ui_lbl_status.setBounds(10, 11, 46, 14);
		frame.getContentPane().add(ui_lbl_status);
		
		JLabel ui_lbl_port = new JLabel("Port :");
		ui_lbl_port.setBounds(10, 36, 46, 14);
		frame.getContentPane().add(ui_lbl_port);

		JLabel ui_lbl_Gps = new JLabel("GPS :");
		ui_lbl_Gps.setBounds(622, 11, 51, 14);
		frame.getContentPane().add(ui_lbl_Gps);
		
		JLabel ui_lbl_Batterie = new JLabel("Batterie :");
		ui_lbl_Batterie.setBounds(622, 61, 51, 14);
		frame.getContentPane().add(ui_lbl_Batterie);
		
		JLabel ui_lbl_MoteurPropultion = new JLabel("Moteur Propultion - (Z)");
		ui_lbl_MoteurPropultion.setBounds(10, 221, 133, 14);
		frame.getContentPane().add(ui_lbl_MoteurPropultion);
		
		JLabel ui_lbl_MoteurGauche = new JLabel("Moteur Gauche - (Q)");
		ui_lbl_MoteurGauche.setBounds(10, 277, 145, 14);
		frame.getContentPane().add(ui_lbl_MoteurGauche);
		
		JLabel ui_lbl_MoteurDroit = new JLabel("Moteur Droit - (D)");
		ui_lbl_MoteurDroit.setBounds(10, 336, 145, 14);
		frame.getContentPane().add(ui_lbl_MoteurDroit);
		
		JLabel ui_lbl_ModeTlcomand = new JLabel("Mode t\u00E9l\u00E9comand\u00E9");
		ui_lbl_ModeTlcomand.setBounds(10, 189, 115, 14);
		frame.getContentPane().add(ui_lbl_ModeTlcomand);
		
		//menu bar	
		JMenuBar menuBar = new JMenuBar();
		frame.setJMenuBar(menuBar);
		
		JMenu mnHelp = new JMenu("Help");
		menuBar.add(mnHelp);
		
		JMenuItem mntmAbout = new JMenuItem("About");
		mntmAbout.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				javax.swing.JOptionPane.showMessageDialog(null,"duran_n - guitte_r - ravaji_b\nSCIA 2015\nPFEE Paramoteur"); 
			}
		});
		mnHelp.add(mntmAbout);
		
		btn_listen = new JButton("Listen");
		btn_listen.setBounds(10, 61, 89, 23);
		frame.getContentPane().add(btn_listen);
		
		lbl_status = new JLabel("Not listening");
		lbl_status.setBounds(66, 11, 202, 14);
		frame.getContentPane().add(lbl_status);
		
		txt_port = new JTextField();
		txt_port.setBounds(66, 33, 46, 20);
		txt_port.setColumns(10);	
		frame.getContentPane().add(txt_port);
	
		txt_log = new JTextArea();
		txt_log.setBounds(253, 6, 359, 471);
		frame.getContentPane().add(txt_log);
		
		lbl_gps_lat = new JLabel("-1");
		lbl_gps_lat.setBounds(697, 11, 131, 14);
		frame.getContentPane().add(lbl_gps_lat);
		
		lbl_batt = new JLabel("-1");
		lbl_batt.setBounds(697, 61, 46, 14);
		frame.getContentPane().add(lbl_batt);

		lbl_gps_long = new JLabel("-1");
		lbl_gps_long.setBounds(697, 36, 131, 14);
		frame.getContentPane().add(lbl_gps_long);
		
		btn_disconnect = new JButton("Disconnect");
		btn_disconnect.setBounds(109, 61, 107, 23);
		frame.getContentPane().add(btn_disconnect);
		
		lbl_tele = new JLabel("OFF");
		lbl_tele.setFont(new Font("Tahoma", Font.BOLD, 11));
		lbl_tele.setBounds(134, 189, 31, 14);
		frame.getContentPane().add(lbl_tele);
		
		btn_toggle_tele = new JButton("Toggle");
		btn_toggle_tele.setBounds(175, 185, 72, 23);
		frame.getContentPane().add(btn_toggle_tele);
		
		btn_power_p_inc = new JButton("+");
		btn_power_p_inc.setBounds(88, 246, 41, 23);
		frame.getContentPane().add(btn_power_p_inc);
		
		btn_power_p_dec = new JButton("-");
		btn_power_p_dec.setBounds(136, 246, 41, 23);
		frame.getContentPane().add(btn_power_p_dec);
		
		btn_power_l_dec = new JButton("-");
		btn_power_l_dec.setBounds(136, 302, 41, 23);
		frame.getContentPane().add(btn_power_l_dec);
		
		btn_power_l_inc = new JButton("+");
		btn_power_l_inc.setBounds(88, 302, 41, 23);
		frame.getContentPane().add(btn_power_l_inc);
		
		btn_power_r_dec = new JButton("-");
		btn_power_r_dec.setBounds(136, 361, 41, 23);
		frame.getContentPane().add(btn_power_r_dec);
		
		btn_power_r_inc = new JButton("+");
		btn_power_r_inc.setBounds(88, 361, 41, 23);
		frame.getContentPane().add(btn_power_r_inc);
		
		spn_power_p = new JSpinner();
		spn_power_p.setBounds(20, 247, 51, 20);
		frame.getContentPane().add(spn_power_p);
	    JComponent comp_p = spn_power_p.getEditor();
	    JFormattedTextField field_p = (JFormattedTextField) comp_p.getComponent(0);
	    DefaultFormatter formatter_p = (DefaultFormatter) field_p.getFormatter();
	    formatter_p.setCommitsOnValidEdit(true);
	    
		spn_power_r = new JSpinner();
		spn_power_r.setBounds(20, 362, 51, 20);
		frame.getContentPane().add(spn_power_r);
	    JComponent comp_r = spn_power_r.getEditor();
	    JFormattedTextField field_r = (JFormattedTextField) comp_r.getComponent(0);
	    DefaultFormatter formatter_r = (DefaultFormatter) field_r.getFormatter();
	    formatter_r.setCommitsOnValidEdit(true);
	    
		spn_power_l = new JSpinner();
		spn_power_l.setBounds(20, 303, 51, 20);
		frame.getContentPane().add(spn_power_l);
	    JComponent comp_l = spn_power_l.getEditor();
	    JFormattedTextField field_l = (JFormattedTextField) comp_l.getComponent(0);
	    DefaultFormatter formatter_l = (DefaultFormatter) field_l.getFormatter();
	    formatter_l.setCommitsOnValidEdit(true);
	}
}
