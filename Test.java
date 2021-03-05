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
            System.out.println(text);
            if (text.toLowerCase().equals("getlocaltime")) {
                System.out.println("inside here time");
                GetLocalTime time_obj = new GetLocalTime();
                time_obj.valid.setValue('1');
                time_obj.execute(hostname, port);
                int t = time_obj.time.getValue();
                System.out.println(t);
            } else if (text.toLowerCase().equals("getlocalos")) {
                System.out.println("inside here");
                GetLocalOS os_obj = new GetLocalOS();
                os_obj.valid.setValue('1');
                os_obj.execute(hostname, port);
                char os_value = os_obj.os.getValue();
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
