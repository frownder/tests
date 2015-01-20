
import java.util.*;
import java.util.regex.*;

public class StrTok{
    
    public static void main(String args[]){
        
        String test="abc def ghi k d e f a d d e f eaf dfe afd d aa f ad f a d f e a fd e f a e d a d e fa d e fa d e fa d fe a d fe fda  e  da d ee ad fe ad fe ad de a de a de fd e a d e a d e ad e f ad e f a e d fa e d  fae   f";


        long before = System.currentTimeMillis();

        /*StringTokenizer strTok = new StringTokenizer(test," ");

        while( strTok.hasMoreTokens()){
            System.out.println( strTok.nextElement() );
        }
        */

        /*
        String splitted[] = test.split("\\s");

        for (String s: splitted)
            System.out.println(s);
        */
       
        String test2 = "OK!OK!OK!OK!";
        Scanner sc = new Scanner(test2);
        String temp = sc.findInLine("(\\w+)!");
        System.out.println(temp);

        MatchResult r = sc.match();
        System.out.println( r.groupCount() );


        long after = System.currentTimeMillis();
        
        System.out.println( after-before);
    }
}
