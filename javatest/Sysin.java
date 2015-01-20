import java.io.*;

class Sysin {

    public static void main(String args[]){

        System.out.println (Thread.currentThread().getStackTrace());
        System.out.print(" type: ");

        char c=' ';
        try{
            c = (char)System.in.read();
        }catch (IOException e){
        }
          

        switch(c){
            case 'a': System.out.println("a: hello?");
            break;

            case 'b': System.out.println("b: hello?");
            break;
            
            default : System.out.println(c +  "hello?");
            break;
        }

    }

}
