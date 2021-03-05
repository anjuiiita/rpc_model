import java.net.*;
import java.io.*;
import java.util.*;
import java.nio.ByteBuffer;

public class GetLocalOS {
    c_char os = new c_char();
    c_char valid = new c_char();

    public static StringBuilder readintoString(byte[] bytes) 
	{ 
		if (bytes == null) 
			return null; 
		StringBuilder data = new StringBuilder(); 
		int i = 0; 
		while (bytes[i] > 0) 
		{ 
            char c = (char) bytes[i];
			data.append(c);
			i++; 
		} 
		return data;
	} 

    public static int readIntoInteger(byte[] bytes) {
        return ByteBuffer.wrap(bytes).getInt();
    }

    public void execute(String IP, int port) {
        try {
            int cmdlen = os.getSize() + valid.getSize();
            byte[] buf = new byte[100 + 4 + cmdlen];
            byte[] CmdID = "GetLocalOS".getBytes();
            System.arraycopy(CmdID, 0, buf, 0, CmdID.length);
            
            byte[] CmdLength = intToByte(cmdlen);
            System.arraycopy(CmdLength, 0, buf, 100, CmdLength.length);

            System.arraycopy(os.toByte(), 0, buf, 104, os.getSize());
            
            int offset = 104 + valid.getSize();
            System.arraycopy(valid.toByte(), 0, buf, offset, valid.getSize());
            

            Socket socket = new Socket(IP, port);
            OutputStream output = socket.getOutputStream();
            PrintWriter writer = new PrintWriter(output, true);

            byte[] ID_bytes = Arrays.copyOfRange(buf, 0, 99);
            StringBuilder value = readintoString(ID_bytes);
            System.out.println(value);

            ID_bytes = Arrays.copyOfRange(buf, 100, 104);
            int value1 = readIntoInteger(ID_bytes);
            System.out.println(value1);

            ID_bytes = Arrays.copyOfRange(buf, 104, 106);
            value = readintoString(ID_bytes);
            System.out.println(value);

            ID_bytes = Arrays.copyOfRange(buf, 106, 108);
            value = readintoString(ID_bytes);
            System.out.println(value);
            System.out.println("buffer size sent" + buf.length + buf);
            writer.println(buf);
            socket.close();
        } catch (UnknownHostException ex) {
            System.out.println("Server not found: " + ex.getMessage());
        } catch (IOException ex) {
            System.out.println("Error writing to server: " + ex.getMessage());
        }
    }

    static byte[] intToByte(int i)
    {
        byte[] result = new byte[4];

        result[0] = (byte) (i >> 24);
        result[1] = (byte) (i >> 16);
        result[2] = (byte) (i >> 8);
        result[3] = (byte) (i /*>> 0*/);

        return result;
    }
}
