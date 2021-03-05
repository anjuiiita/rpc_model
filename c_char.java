import java.nio.ByteBuffer;

public class c_char {
    byte[] buf = new byte[2];
    
    public int getSize() {
        return buf.length;
    }
    public char getValue() {
        return ByteBuffer.wrap(buf).order(java.nio.ByteOrder.LITTLE_ENDIAN).getChar();
    }
    public void setValue(byte[] b) {
        System.arraycopy(b, 0, buf, 0, 2);
    }
    public void setValue(char v) {
        byte temp[] = new byte[]{(byte)v};
        System.out.println(temp.length);
        System.arraycopy(temp, 0, buf, 0, temp.length);
    }

    public byte[] toByte() {
        return this.buf;
    }
}
