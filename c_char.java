

public class c_char {
    byte[] buf = new byte[16];
    
    public int getSize() {
        return buf.length;
    }
    public StringBuilder getValue() {
		if (buf == null) 
			return null; 
		StringBuilder data = new StringBuilder(); 
		int i = 0; 
		while (buf[i] > 0) 
		{ 
            char c = (char) buf[i];
			data.append(c);
			i++; 
		} 
		return data;
    }
    public void setValue(byte[] b) {
        System.arraycopy(b, 0, buf, 0, b.length);
    }
    public void setValue(char v) {
        byte temp[] = new byte[]{(byte)v};
        System.arraycopy(temp, 0, buf, 0, temp.length);
    }

    public byte[] toByte() {
        return this.buf;
    }
}
