package pipwm4j;

public class Pwm implements java.io.Closeable {

    protected long hControlStructure = 0;
    protected int pin;
    protected Runner runner = new Runner();

    /**
     * Assigns the pin as a digital output
     * @param pin The number of the pin. It should be reasonably between 1 and 27, but this class doesn't check anything
     */
    public Pwm(int pin) {
	this.pin = pin;
	hControlStructure = allocateControlStructure(pin);
	writeControlStructure(hControlStructure, 1000, 0, false);
    }

    public void start() {
	if (!runner.isAlive()) {
	    runner.start();
	}
    }

    /**
     * Sets the pin's state to high (true) or low (false)
     */
    public void setState(boolean state) {
	setPinState(pin, state);
    }

    public void close() {
	if (runner.isAlive()) {
	    writeControlStructure(hControlStructure, 0, 0, true);
	    try {
    		runner.join();
	    } catch (Exception e) {
		throw new RuntimeException(e);
	    }
	}
	if (hControlStructure != 0) {
	    freeControlStructure(hControlStructure);
	    hControlStructure = 0;
	}
    }

    protected native long allocateControlStructure(int pin);
    protected native void freeControlStructure(long hControlStructure);
    protected native void doPwm(long hControlStructure);
    protected native void writeControlStructure(long hControlStructure, long span, long fill, boolean stop);
    protected native void setPinState(int pin, boolean state);

    protected class Runner extends Thread {
	public void run() {
	    doPwm(hControlStructure);
	}
    }
}