package fr.epita.pfee.station_controle;

public enum Motor {
	RIGHT('R'),
	LEFT('L'),
	PROPULTION('P');
	
	private char value;
	
	 private Motor(char value) {
         this.value = value;
 }
}
