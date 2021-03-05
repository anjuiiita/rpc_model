import java.nio.ByteBuffer;

public class c_int {
    byte[] buf = new byte[4];
    
    public int getSize() {
        return buf.length;
    }
    public int getValue() {
        return ByteBuffer.wrap(buf).order(java.nio.ByteOrder.LITTLE_ENDIAN).getInt();
    }
    public void setValue(byte[] b) {
        System.arraycopy(b, 0, buf, 0, 4);
    }
    public void setValue(int v) {
        byte temp[] = intToByte(v);
        System.arraycopy(temp, 0, buf, 0, 4);
    }

    public byte[] toByte() {
        return this.buf;
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
