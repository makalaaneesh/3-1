/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package completedapriori;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.StringTokenizer;

/**
 *
 * @author makala
 */
public class Apriori {
    static String filepath = "/home/makala/Desktop/transactions.txt";
    static int minimum_support = 2;
    static final int HASHING = 1;
    static final int NORMAL = 0;
    static final int PARTITION = 2;
    static final int DIC = 3;
    static int method = NORMAL; //CHANGE THIS TO CHANGE THE METHOD
    /**
     * 
     * @param args the command line arguments
     */
    public static ArrayList readFromFile() throws FileNotFoundException, IOException{
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
        return a;
    }
    public static void main(String[] args) throws IOException {
        // TODO code application logic here
        
        newalgo normal = new newalgo(minimum_support, method, filepath, readFromFile());
        normal.run_algo();
//        normal.getAssociationRules();
//        partitioning P = new partitioning(minimum_support,readFromFile(), 3,filepath);
//        P.run_algo();
        
    }
    
    
}
