import java.net.*;
import java.io.*;

public class GetLocalTime {
    c_int time = new c_int();
    c_char valid = new c_char();

    public void execute(String IP, int port) {
        try {

            int cmdlen = time.getSize() + valid.getSize();
            System.out.println(cmdlen);
            byte[] buf = new byte[100 + 4 + cmdlen];
            byte[] CmdID = "GetLocalTime".getBytes();
            System.arraycopy(CmdID, 0, buf, 0, CmdID.length);
            byte[] CmdLength = intToByte(cmdlen);
            System.arraycopy(CmdLength, 0, buf, 100, CmdLength.length);
            System.arraycopy(time.toByte(), 0, buf, 104, time.getSize());
            int offset = 104 + time.getSize();
            System.arraycopy(valid.toByte(), 0, buf, offset, valid.getSize());
            Socket socket = new Socket(IP, port);
            OutputStream output = socket.getOutputStream();
            PrintWriter writer = new PrintWriter(output, true);
            System.out.println("buffer size sent" + buf.length + buf);
            //writer.println(buf);
            writer.println(buf);
            socket.close();
        } catch (UnknownHostException ex) {
            System.out.println("Server not found: " + ex.getMessage());
        } catch (IOException ex) {
            System.out.println("I/O Error: " + ex.getMessage());
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
