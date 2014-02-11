import jarduino.*;
import jssc.SerialPortException;

public class JSTest {
 
    public static void main(String[] args) {
	   try{
       JArduino ard = new JArduino ("COM22");
	   ard.pinMode (13, PORT.OUTPUT);
	   ard.digitalWrite (13, DIGITAL.HIGH);
	   ard.tone (8, 50);
	   try{Thread.sleep (2000);}catch( InterruptedException e){}
	   ard.noTone (8);
	   ard.digitalWrite (13, DIGITAL.LOW);
	   ard.digitalWrite (13, DIGITAL.HIGH);
	   System.out.println (ard.digitalRead (13));
	   System.out.println (ard.analogRead (4));
	   }
	   catch (SerialPortException e){}

    }
}