/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package fp;

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
public class FP {
    String filepath = "./transactions.txt";
    int minimumSupport = 2;
    ArrayList<ArrayList<String>> allTransactions;
    tree t;
    String prefix;
    /**
     * @param args the command line arguments
     */
    public FP(String prefix) throws IOException{
        this.prefix = prefix;
        allTransactions = new ArrayList<>();
        readFromFile();
        
        t = new tree(allTransactions, minimumSupport,prefix);
        
        t.printTree();
        t.algo();
    }
    public FP(ArrayList<ArrayList<String>> allTransactions, String prefix){
        this.prefix = prefix;
        this.allTransactions = new ArrayList(allTransactions);
        
        t = new tree(allTransactions, minimumSupport,prefix);
        t.printTree();
        t.algo();
    }
    public void readFromFile() throws FileNotFoundException, IOException{
//        ArrayList<ArrayList<String>> a = new ArrayList<>();
         BufferedReader r = new BufferedReader(new FileReader(filepath));
            while(r.ready()){
                ArrayList<String> t = new ArrayList<String>();
                StringTokenizer items = new StringTokenizer(r.readLine()," ");
                while(items.hasMoreTokens()){
                    String item = items.nextToken();
                    t.add(item);
                }
                allTransactions.add(t);
            }
            
    }
    
//    public void rec(String item){
//        ArrayList<ArrayList<String>> p = t.allPathsEndingIn(item);
//        FP ending_e = new FP(p);
////            ending_e.genTree(false);
////        ending_e.t.printTree();
//        ending_e.rec("d");
//        
//        
//    }
    public ArrayList<ArrayList<String>> getFIS(){
        ArrayList<ArrayList<String>> f = t.getFreqItemSets();
//        System.out.println("Frequent itemsets are "+f);
        return f;
    }
    public static void main(String[] args) throws IOException {
        // TODO code application logic here
        
        FP mainFP = new FP("");
        System.out.println(""+mainFP.getFIS());
        
        
        
        
        
//        System.out.println(""+t.root.children.get(0).item);
//        System.out.println(""+t.root.children.get(0).ItemEquals("a"));
        
    }
    
    
}
