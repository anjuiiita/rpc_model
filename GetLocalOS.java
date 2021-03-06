import java.net.*;
import java.io.*;
import java.util.*;
import java.nio.ByteBuffer;

public class GetLocalOS {
    c_char os = new c_char();
    c_char valid = new c_char();

    public void execute(String IP, int port) {
        try {

            Socket clientSocket = new Socket(IP, port); 

            // create text reader and writer
            DataInputStream inStream  = new DataInputStream(clientSocket.getInputStream());
            DataOutputStream outStream = new DataOutputStream(clientSocket.getOutputStream());

            int cmdlen = os.getSize() + valid.getSize();
            byte[] buf = new byte[100 + 4 + cmdlen];
            byte[] CmdID = "GetLocalOS".getBytes();
            System.arraycopy(CmdID, 0, buf, 0, CmdID.length);
            
            byte[] CmdLength = intToByte(cmdlen);
            System.arraycopy(CmdLength, 0, buf, 100, CmdLength.length);

            System.arraycopy(os.toByte(), 0, buf, 104, os.getSize());
            
            int offset = 104 + valid.getSize();
            System.arraycopy(valid.toByte(), 0, buf, offset, valid.getSize());

            outStream.write(buf, 0, buf.length);
            outStream.flush();

            byte[] buff_in = new byte[cmdlen];

            inStream.readFully(buff_in);

            byte[] incoming_os = Arrays.copyOfRange(buff_in, 0, 16);
            byte[] incoming_valid = Arrays.copyOfRange(buff_in, 16, 32);

            this.os.setValue(incoming_os);
            this.valid.setValue(incoming_valid);

            clientSocket.close();
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
