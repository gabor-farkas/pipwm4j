
import pipwm4j.Pwm;

public class JniTest {

    public static void main(String [] args) throws Exception {
	System.loadLibrary("pwm");
	Pwm pwm = new Pwm(1);
	pwm.start();
	Thread.sleep(1000);
	pwm.close();
    }

}