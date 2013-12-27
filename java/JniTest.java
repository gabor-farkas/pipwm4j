
import pipwm4j.Pwm;

public class JniTest {

    public static void main(String [] args) throws Exception {
	System.loadLibrary("pwm");
	Pwm row = new Pwm(18);
	row.setState(true);
	Pwm pwm = new Pwm(23);
	pwm.start();
	pwm.setParams(1000, 500);
	Thread.sleep(1000);
	pwm.setParams(1000, 100);
	Thread.sleep(1000);
	pwm.close();
	row.close();
    }

}