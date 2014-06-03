package fr.epita.pfee.station_controle;

import java.awt.Color;

public class Settings {
	private boolean mode_telecomande;
	public boolean connecte;
	public static Settings get;
	
	public Settings()
	{
		mode_telecomande = false;
		connecte = false;
		Settings.get = this;
	}

	public void toggle_mode_telecomande()
	{
		MainWindow ui = MainWindow.get;
		
		if (!mode_telecomande && !connecte)
		{
			javax.swing.JOptionPane.showMessageDialog(null,"ERROR : Must connect first"); 
		}
		else if (!mode_telecomande)
		{
			mode_telecomande = true;
			ui.spn_power_l.setEnabled(true);
			ui.btn_power_l_inc.setEnabled(true);
			ui.btn_power_l_dec.setEnabled(true);
			ui.spn_power_r.setEnabled(true);
			ui.btn_power_r_dec.setEnabled(true);
			ui.btn_power_r_inc.setEnabled(true);
			ui.spn_power_p.setEnabled(true);
			ui.btn_power_p_inc.setEnabled(true);
			ui.btn_power_p_dec.setEnabled(true);
			ui.lbl_tele.setForeground(new Color(0,125,0));
		}
		else 
		{
			mode_telecomande = false;
			ui.spn_power_l.setEnabled(false);
			ui.btn_power_l_inc.setEnabled(false);
			ui.btn_power_l_dec.setEnabled(false);
			ui.spn_power_r.setEnabled(false);
			ui.btn_power_r_dec.setEnabled(false);
			ui.btn_power_r_inc.setEnabled(false);
			ui.spn_power_p.setEnabled(false);
			ui.btn_power_p_inc.setEnabled(false);
			ui.btn_power_p_dec.setEnabled(false);
			ui.lbl_tele.setText("OFF");
			ui.lbl_tele.setForeground(new Color(125,0,0));
		}
	}

}
