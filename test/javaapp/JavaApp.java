public class JavaApp{
	private native boolean writeDataReport(String cpuLoad, String ramLoad);
	private native boolean readDataReport();
	static{
		System.loadLibrary("javaJNI");
	}
	
	static public void main(String[] args){
		System.out.println("hello trung we are trying to use c/c++ friends");
		System.out.println("readDataReport");
		JavaApp javaApp = new JavaApp();
		// javaApp.readDataReport();
		for(int i = 0; i <100000; i++){
			System.out.println("writeDataReport()");
			javaApp.writeDataReport("50","20");
		}
	}
}
