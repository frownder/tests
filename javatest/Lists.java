
import java.util.*;

public class Lists{

    public static void main(String[] args){
        List<Integer> list = new ArrayList<Integer>();

        list.add(1);
        list.add(2);
        list.add(3);

        for( int i : list)
            System.out.println(i);


        Comparator<Integer> compare = new Comparator<Integer>(){
            public int compare( Integer a, Integer b){
                if (a==2) return -1;
                else
                    return Integer.compare(a,b);
            }

            public boolean eqauals(Integer a, Integer b){
                if (a==b) return true;
                else return false;
            }
        };


        Collections.sort(list,compare);

        for( int i : list)
            System.out.println(i);
    }
}
