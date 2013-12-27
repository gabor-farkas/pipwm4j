package pipwm4j;

public class Pwm implements java.io.Closeable {

    protected long hControlStructure = 0;
    protected int pin;

    public Pwm(int pin) {
	this.pin = pin;
	hControlStructure = allocateControlStructure();
    }

    public void close() {
	if (hControlStructure != 0) {
	    freeControlStructure(hControlStructure);
	    hControlStructure = 0;
	}
    }

    protected native long allocateControlStructure();
    protected native void freeControlStructure(long hControlStructure);
}