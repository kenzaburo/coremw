import java.sql.*;

public class JavaNoJni
{
  public static void main( String args[] )
  {
    Connection c = null;
    Statement stmt = null;
    try {
       for(int i = 0; i < 100000; i++){
        Class.forName("org.sqlite.JDBC");
        c = DriverManager.getConnection("jdbc:sqlite:/home/trunghuynh/c_plus_plus/coremw/db/system_load.db");
        c.setAutoCommit(false);
        System.out.println("Opened database successfully");

       
          stmt = c.createStatement();
          String sql = "INSERT INTO system_load_tb (CPU_LOAD,RAM_LOAD) VALUES(5000,5000)";
          stmt.executeUpdate(sql);
          System.out.println("insert data successfully");
        

        stmt.close();
        c.commit();
        c.close();
      }
    } catch ( Exception e ) {
      System.err.println( e.getClass().getName() + ": " + e.getMessage() );
      System.exit(0);
    }
    System.out.println("Records created successfully");
  }
}