/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package completedapriori;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.*;

/**
 *
 * @author c137104
 */




public class newalgo {
    int minimum_support;
    int k = 0;
    int hashing = 1;
    int partitioning = 2;
    int dic = 3;
    int method;
    String filepath;
    ArrayList<ArrayList> allTransactions;
    ArrayList<ArrayList> finalL = new ArrayList<ArrayList>();
    
    public newalgo(int minimumSupport, int method, String filepath){
        this.method = method;
        minimum_support = minimumSupport;
        this.filepath = filepath;
    }
    
    public newalgo(int minimumSupport, int method, String filepath, ArrayList<ArrayList> transactions){
        this.method = method;
        minimum_support = minimumSupport;
        this.filepath = filepath;
        allTransactions = transactions;
    }
    
    public ArrayList readAllUniqueItems() throws FileNotFoundException, IOException{
        ArrayList<String> arr = new ArrayList();
        BufferedReader r = new BufferedReader(new FileReader(filepath));
            while(r.ready()){
                StringTokenizer items = new StringTokenizer(r.readLine()," ");
                while(items.hasMoreTokens()){
                    String item = items.nextToken();
                    if(!arr.contains(item)){
                        arr.add(item);
                    }
                }
            }
            return arr;
    }
    
    
   public ArrayList<ArrayList> genNextLevel(ArrayList<ArrayList> L, int k){
       ArrayList<ArrayList> C = new ArrayList<ArrayList>();
       for(int i = 0; i< L.size()-1;i++){
           for(int j = i+1; j< L.size(); j++){
               ArrayList<String> a = L.get(i);
               ArrayList<String> b = L.get(j);
               boolean canBeMerged = true;
               int c = 0;
               while(c<=k){
                   if(!a.get(c).equals(b.get(c)))
                       break;
                   c++;
               }
               if (c != k){
                   canBeMerged = false;
               }
               if(canBeMerged){
                   ArrayList t = new ArrayList(a);
                   t.add(b.get(b.size()-1));
                   C.add(t);
               }
               
           }
       }
       return C;
   } 
   public static void printA(ArrayList<String> a){
        for(String x: a){
            System.out.print(x + " ");
        }
    }
   public void printAL(ArrayList<ArrayList> A){
        for(ArrayList t: A){
            for(Object s: t){
                System.out.print(""+ s+" ");
            }
            System.out.println("");
        }
   }
   public boolean existsIn(ArrayList c, ArrayList<ArrayList> L){
       boolean exits = false;
       for(ArrayList x: L){
           if(x.equals(c)){
               exits = true;
               break;
           }
       }
       return exits;
   }
    
    public void prune(ArrayList<ArrayList> C, ArrayList<ArrayList> prevL){
        
        for(ArrayList a: C){
            for(int i = 0; i< a.size() ; i++){
                ArrayList temp = new ArrayList(a);
                temp.remove(i);
                if (!existsIn(temp, prevL)){
                    a.clear();
                    break;
                }
            }
            
        }
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
    public int freq(ArrayList a, ArrayList<ArrayList> at){
//        for(Object x: a){
//            System.out.print(""+ x + " "); 
//        }
        int c = 0;
        for(ArrayList temp: at){
            if(temp.containsAll(a))
                c++;
        }
//        System.out.print(""+ c+"\n");
        return c;
    }
    
    public ArrayList<ArrayList> run_algo() throws IOException{
        ArrayList<String> firstL = readAllUniqueItems();
        Collections.sort(firstL);
       
        System.out.println("");
        ArrayList<ArrayList> L = new ArrayList();
        for(String s: firstL){
            ArrayList<String> temp = new ArrayList();
            temp.add(s);
            L.add(temp);
        }
        
        
        while(true){
            if(L.size() > 0){
                for(ArrayList x: L){
                    finalL.add(x);
                }
            }
            if(L.size() == 0)
                break;
            ArrayList<ArrayList> C = new ArrayList<ArrayList>();
            if(k == 0){
                if(method == 1){//hashing generating the second level via the hash buckets method
                    hashing h = new hashing(filepath,minimum_support);
                    C = h.genFromHashing(L);
                }
                else{//normal
                    System.out.println("normal method bro");
                    C = genNextLevel(L,k);
                }
                
            }
            else{
                C = genNextLevel(L,k);
            }
            
            if(C.size() == 0)
                break;
            prune(C,L);
            L = new ArrayList<ArrayList>();
            System.out.println("---------------C----------------");
            printAL(C);
            System.out.println("--------------------------------\n");
            for(ArrayList a :C){
                if(a.size()>0){
                    if(method == 2){// if method is partitioning, transactions will be supplied. so read from data structure instead of from file
                        if(freq(a,allTransactions) >= minimum_support){
                            L.add(a);
                        }
                    }
                    else{
                        if(freq2(a) >= minimum_support){
                            L.add(a);
                        }
                    }
                }
            }
            System.out.println("-----------------L--------------");
            printAL(L);
            System.out.println("--------------------------------\n");
            k++;
        }
        printAL(L);
//        System.out.println("final l is");
//        printAL(finalL);
        return finalL;
    }
    public void getAssociationRules() throws FileNotFoundException, IOException{
        AssociationRules a = new AssociationRules(this);
        a.genAssociateRules(finalL);
    }
    
    
}
