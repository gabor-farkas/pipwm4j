
import pipwm4j.Pwm;

public class JniTest {

    public static void main(String [] args) {
	System.loadLibrary("pwm");
	Pwm pwm = new Pwm(1);
	pwm.close();
    }

}