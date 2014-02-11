/*
	JArduino
	Remotely automate arduino boards using JAVA	
	Author : Raja Jamwal <linux1@zoho.com>
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
	
*/

package jarduino;

import jssc.SerialPort;
import jssc.SerialPortException;

/*
	TODO: TODO: add support for SHIFT_OUT SHIFT_IN PULSE_IN
*/

class OPERATION
{
	public static byte PIN_MODE 			= 0;
	public static byte DIGITAL_WRITE 		= 1; 
	public static byte DIGITAL_READ 		= 2;
	public static byte ANALOG_REFERENCE 	= 3;
	public static byte ANALOG_READ 			= 4;
	public static byte ANALOG_WRITE 		= 5;
	public static byte TONE 				= 6;
	public static byte TONE_DURATION 		= 7;
	public static byte NO_TONE 				= 8;
	public static byte SHIFT_OUT 			= 9;
	public static byte SHIFT_IN 			= 10;
	public static byte PULSE_IN 			= 11;
};

public class JArduino
{
	SerialPort serialPort;
	public JArduino (String portName) throws SerialPortException
	{
		serialPort = new SerialPort (portName); 
		
        serialPort.openPort();
        serialPort.setParams	(SerialPort.BAUDRATE_9600, 
                                 SerialPort.DATABITS_8,
                                 SerialPort.STOPBITS_1,
                                 SerialPort.PARITY_NONE);
								 
		byte init[] = new byte [3];
		init = read (3);
		if (init[0] == 0x00 && init[2] == 0x01)
		{
			System.out.println ("Arduino board connected, supports version 1");
		}
	}
	
	public void pinMode (int pin, byte mode) throws SerialPortException
	{
		byte control[] = {OPERATION.PIN_MODE, 0x02, (byte)pin, mode};
		write (control);
		read (2);
	}
	
	public int analogRead (int pin) throws SerialPortException
	{
		byte control[] = {OPERATION.ANALOG_READ, 0x01, (byte)pin};
		write (control);
		read (2);
		byte response[] = new byte [4];
		response = read (4);
		if (response[0] == OPERATION.ANALOG_READ && response[1] == 2)
		{
			return (int) (response[3]);
		}
		return 0;
	}
	
	public void analogWrite (int pin, int value) throws SerialPortException
	{
		byte control[] = {OPERATION.ANALOG_WRITE, 0x02, (byte)pin, (byte)value};
		write (control);
		read (2);
	}
	
	public byte digitalRead (int pin) throws SerialPortException
	{
		byte control[] = {OPERATION.DIGITAL_READ, 0x01, (byte)pin};
		write (control);
		read (2);
		byte response[] = new byte [4];
		response = read (4);
		if (response[0] == OPERATION.DIGITAL_READ && response[1] == 2)
		{
			return (response[3]);
		}
		return DIGITAL.LOW;
	}
	
	public void digitalWrite (int pin, byte mode) throws SerialPortException
	{
		byte control[] = {OPERATION.DIGITAL_WRITE, 0x02, (byte)pin, mode};
		write (control);
		read (2);
	}
	
	public void noTone (int pin) throws SerialPortException
	{
		byte control[] = {OPERATION.NO_TONE, 0x01, (byte)pin};
		write (control);
		read (2);
	}
	
	public void tone (int pin, int freq) throws SerialPortException
	{
		byte control[] = {OPERATION.TONE, 0x02, (byte)pin, (byte)freq};
		write (control);
		read (2);
	}
	
	public void tone (int pin, int freq, int duration) throws SerialPortException
	{
		byte control[] = {OPERATION.TONE_DURATION, 0x02, (byte)pin, (byte)freq, (byte) (duration>255?255:0)};
		write (control);
		read (2);
	}
	
	public void close () throws SerialPortException
	{
		serialPort.closePort();
		serialPort = null;
	}
	
	public byte[] read (int len) throws SerialPortException
	{
		return serialPort.readBytes (len);
	}
	
	public void write (byte data[]) throws SerialPortException
	{
		serialPort.writeBytes (data);
	}
}; 

