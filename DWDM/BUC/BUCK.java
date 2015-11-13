/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package buck;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.StringTokenizer;

/**
 *
 * @author c137104
 */
public class BUCK {
    String filepath ="transactions.txt";
    ArrayList<ArrayList<String>> allTransactions;
    int minimum_support = 2;
    static HashMap<ArrayList<String>,Integer> iceberg;
    public BUCK() throws FileNotFoundException, IOException{
        allTransactions = readFromFile();
        iceberg = new HashMap<ArrayList<String>,Integer>();
    }
    public ArrayList readFromFile() throws FileNotFoundException, IOException{
        ArrayList<ArrayList> a = new ArrayList<ArrayList>();
         BufferedReader r = new BufferedReader(new FileReader(filepath));
            while(r.ready()){
                ArrayList<String> t = new ArrayList<String>();
                StringTokenizer items = new StringTokenizer(r.readLine()," ");
                while(items.hasMoreTokens()){
                    String item = items.nextToken();
                    t.add(item);
                }
                a.add(t);
            }
//        System.out.println(""+ a);
        return a;
    }
    public int freq(ArrayList<String> tuple){
        int count = 0;
        for(ArrayList<String> trans:allTransactions){
            boolean diff = false;
            for( int i = 0; i<tuple.size(); i++){
                if(!tuple.get(i).equals("*")){
                    if(!tuple.get(i).equals(trans.get(i))){
                        diff = true;
                        break;
                    }
                }
            }
            if (!diff){
                count++;
            }
        }
//        System.out.println(""+ count);
        return count;
    }
    
    ArrayList<String> allUniqueValues(int d){
        ArrayList<String> ofAbhishek = new ArrayList<String>();
        for(ArrayList<String> trans:allTransactions){
            ofAbhishek.add(trans.get(d));
        }
        return ofAbhishek;
        
    }
    public void run(ArrayList<String> tuple, int d){
        if (d>=tuple.size()){
            return;
        }
        int count = freq(tuple);
        if (count<minimum_support){
            return;
        }
        
        iceberg.put(tuple, count);
        ArrayList<String> dimensions = allUniqueValues(d);
        for(String dim: dimensions){
            ArrayList<String> temptuple = new ArrayList(tuple);
            temptuple.set(d, dim);
            run(temptuple,d+1);
        }
        
        
    }
    
    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) throws FileNotFoundException, IOException {
        // TODO code application logic here
        BUCK obj = new BUCK();
        System.out.println(""+ obj.allTransactions);
        ArrayList<String> temp = new ArrayList<String>();
        temp.add("*");
        temp.add("*");
        temp.add("*");
        temp.add("*");
        obj.run(temp, 0);
        System.out.println(""+iceberg);
    }
}
