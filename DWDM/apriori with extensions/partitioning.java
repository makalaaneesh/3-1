/*
 * To change this template, choose Tools | Templates
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
 * @author c137104
 */
public class partitioning {
    ArrayList<ArrayList> allTransactions;
    int minimumSupport;
    int localSupport;
    int total;
    int partitionSize;
    String filepath;
    
    public partitioning(int minimumSupport, ArrayList<ArrayList> allTransactions, int partitionSize, String filepath){
        this.allTransactions = allTransactions;
        this.minimumSupport = minimumSupport;
        this.partitionSize = partitionSize;
        this.filepath = filepath;
        total = allTransactions.size();
        int noOfPartitions = allTransactions.size()/partitionSize;
        localSupport = (int) Math.ceil((double)minimumSupport/(double)noOfPartitions);
        System.out.println("local support is "+ localSupport);
        
    }
    public int freq2(ArrayList a) throws FileNotFoundException, IOException{
        int c = 0;
        BufferedReader r = new BufferedReader(new FileReader(filepath));
            while(r.ready()){
                ArrayList<String> t = new ArrayList<String>();
                StringTokenizer items = new StringTokenizer(r.readLine()," ");
                while(items.hasMoreTokens()){
                    String item = items.nextToken();
                    t.add(item);
                }
                if(t.containsAll(a))
                    c++;
            }
       r.close();
       return c;
    }
    
    public void run_algo() throws IOException{
        ArrayList<ArrayList> C = new ArrayList<ArrayList>();
        ArrayList<ArrayList> finalL = new ArrayList<ArrayList>();
        for(int i = 0; i<total; i = i +partitionSize){
            ArrayList<ArrayList> temp = new ArrayList<ArrayList>(allTransactions.subList(i, Math.min(i+partitionSize, total)));
            System.out.println("partitoin is "+ temp);
            newalgo apriori = new newalgo(localSupport, 2,filepath,temp);
            ArrayList<ArrayList> localL = apriori.run_algo();
            System.out.println("L for partition is "+ localL);
            System.out.println("---------------------");
            for(ArrayList a: localL){
                if (!C.contains(a))
                    C.add(a);
            }
        }
        for(ArrayList a: C){
            if(freq2(a) >= minimumSupport){
                finalL.add(a);
            }
            System.out.println(""+freq2(a));
        }
        
        System.out.println("final L is "+ finalL);
        
        
    }
}
