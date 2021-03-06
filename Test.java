import java.net.*;
import java.io.*;
 
/**
 * This is the chat client program.
 * Type 'bye' to terminte the program.
 *
 * @author www.codejava.net
 */
public class Test {
    private String hostname;
    private int port;
 
    public Test(String hostname, int port) {
        this.hostname = hostname;
        this.port = port;
    }
 
    public void execute() {
        System.out.println("Enter the command");
        Console console = System.console();
        String text;
        do {
            text = console.readLine();
            if (text.toLowerCase().equals("getlocaltime")) {
                GetLocalTime time_obj = new GetLocalTime();
                time_obj.valid.setValue('1');
                time_obj.execute(hostname, port);
                int timeStamp = time_obj.time.getValue();
                java.util.Date dateTime=new java.util.Date((long)timeStamp*1000);
                System.out.println(dateTime);
            } else if (text.toLowerCase().equals("getlocalos")) {
                GetLocalOS os_obj = new GetLocalOS();
                os_obj.valid.setValue('1');
                os_obj.os.setValue('1');
                os_obj.execute(hostname, port);
                StringBuilder os_value = os_obj.os.getValue();
                System.out.println(os_value);
            } else {
                System.out.println("wrong command");
            }

        } while (!text.equals("bye"));
    }
 
    public static void main(String[] args) {
        Test client = new Test("127.0.0.1", 5000);
        client.execute();
    }
}
