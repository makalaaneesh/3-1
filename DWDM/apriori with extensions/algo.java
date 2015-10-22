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




public class algo {
    public static ArrayList readFromFile2() throws FileNotFoundException, IOException{
        ArrayList<String> arr = new ArrayList();
        BufferedReader r = new BufferedReader(new FileReader("C:\\Users\\c137104\\Documents\\NetBeansProjects\\transaction\\build\\classes\\transactions.txt"));
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
    public static ArrayList readFromFile() throws FileNotFoundException, IOException{
        ArrayList<ArrayList> a = new ArrayList<ArrayList>();
         BufferedReader r = new BufferedReader(new FileReader("C:\\Users\\c137104\\Documents\\NetBeansProjects\\transaction\\build\\classes\\transactions.txt"));
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
    
   public static ArrayList<ArrayList> genNextLevel(ArrayList<ArrayList> L, int k){
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
   
   public static void printAL(ArrayList<ArrayList> A){
        for(ArrayList t: A){
            for(Object s: t){
                System.out.print(""+ s+" ");
            }
            System.out.println("");
        }
   }
   public static boolean existsIn(ArrayList c, ArrayList<ArrayList> L){
       boolean exits = false;
       for(ArrayList x: L){
           if(x.equals(c)){
               exits = true;
               break;
           }
       }
       return exits;
   }
    
    public static void prune(ArrayList<ArrayList> C, ArrayList<ArrayList> prevL){
        
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
    public static int freq2(ArrayList a) throws FileNotFoundException, IOException{
        int c = 0;
        BufferedReader r = new BufferedReader(new FileReader("C:\\Users\\c137104\\Documents\\NetBeansProjects\\transaction\\build\\classes\\transactions.txt"));
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
//    public static int freq(ArrayList a, ArrayList<ArrayList> at){
////        for(Object x: a){
////            System.out.print(""+ x + " "); 
////        }
//        int c = 0;
//        for(ArrayList temp: at){
//            if(temp.containsAll(a))
//                c++;
//        }
////        System.out.print(""+ c+"\n");
//        return c;
//    }
    
    
    public static boolean isEmpty(ArrayList<ArrayList> a){
        for(ArrayList x:a){
            if (x.size()>0)
                return false;
        }
        return true;
    }
    
    
    public static void genAllSubsets(ArrayList<String> a,int index, ArrayList<String> partial, int pindex, ArrayList<ArrayList> f){
//        ArrayList<ArrayList> f = new ArrayList<ArrayList>();
        if (index >= a.size())
            return ;
        for(int i = index; i< a.size(); i++){
            if(i == index)
                partial.add(a.get(i));
            else
                partial.set(pindex, a.get(i));
            ArrayList<String> temp = new ArrayList(partial.subList(0, pindex+1));
            if(!existsIn(temp,f))
                f.add(temp);
//            System.out.print("index = "+ index+" pindex = "+ pindex +"|| ");
//            for(String x : temp){
//                System.out.print(x+",");
//            }
//            System.out.println("");
            genAllSubsets(a,i+1,partial, pindex+1, f);
        }
        
    }
    public static void printA(ArrayList<String> a){
        for(String x: a){
            System.out.print(x + " ");
        }
    }
    
    
    public static void genAssociateRules(ArrayList<ArrayList> L) throws FileNotFoundException, IOException{
        for(ArrayList freq: L){
            ArrayList<String> partial = new ArrayList<String>(10);
            ArrayList<ArrayList> f = new ArrayList<ArrayList>();
            genAllSubsets(freq,0,partial, 0,f);
//            System.out.println("anirudh");
//            printAL(f);
            int supCountFreq = freq2(freq); 
            System.out.println("support count "+ supCountFreq);
            for(ArrayList x: f){
                if(x.size()!=freq.size()){
                    printA(x);
                    int confidence = (supCountFreq*100/freq2(x));
                    System.out.println("\t"+ confidence);
                }
            }
            System.out.println("next");
        }
    }
    
    
    public static int hashBuckets(ArrayList<ArrayList> L, int minimumSupport) throws FileNotFoundException, IOException{
        ArrayList<ArrayList> counts = new ArrayList<ArrayList>();
        
      
        for(ArrayList a: L){
            int c = 0;
            BufferedReader r = new BufferedReader(new FileReader("C:\\Users\\c137104\\Documents\\NetBeansProjects\\transaction\\build\\classes\\transactions.txt"));
                while(r.ready()){
                    ArrayList<String> t = new ArrayList<String>();
                    StringTokenizer items = new StringTokenizer(r.readLine()," ");
                    while(items.hasMoreTokens()){
                        String item = items.nextToken();
                        t.add(item);
                    }
                    if(t.containsAll(a))
                        c++;
                    //generating all possible two length subsets and adding to a hashmap
                    for(int i = 0; i<t.size();i++){
                        for(int j = i+1; j<t.size();j++){
                            ArrayList<String> temp = new ArrayList<String>();
                            temp.add(t.get(i));
                            temp.add(t.get(j));
                            
                        }
                    }
                }
           r.close();
           if(c < minimumSupport)
               a.clear();
           
        }
        return 0;
        
    }
    
    public static void main(String[] args) throws FileNotFoundException, IOException {
        int minimum_support = 2;
        int k = 0;
        ArrayList<ArrayList> allTransactions = readFromFile();
        
        ArrayList<String> firstL = readFromFile2();
        Collections.sort(firstL);
       
        System.out.println("");
        ArrayList<ArrayList> L = new ArrayList();
        for(String s: firstL){
            ArrayList<String> temp = new ArrayList();
            temp.add(s);
            L.add(temp);
        }
        
//        ArrayList<String> t = new ArrayList();
//        for(int i =0 ;i<5;i++)
//            L.add(new ArrayList());
//        L.get(0).add("I1");
//        L.get(1).add("I2");
//        L.get(2).add("I3");
//        L.get(3).add("I4");
//        L.get(4).add("I5");
        System.out.println("first l is "+L);
        while(true){
            if(L.size() == 0)
                break;
            ArrayList<ArrayList> C = genNextLevel(L,k);
            prune(C,L);
            if(isEmpty(C))
                break;
            L = new ArrayList<ArrayList>();
            System.out.println("k is "+k);
            System.out.println("---------------C----------------");
            printAL(C);
            System.out.println("--------------------------------\n");
            for(ArrayList a :C){
                if(a.size()>0){
                    
//                    if(freq(a,allTransactions) >= minimum_support){
//                        L.add(a);
//                    }
                    if(freq2(a) >= minimum_support){
                        L.add(a);
                    }
                }
            }
//            printAL(C);
            System.out.println("-----------------L--------------");
            printAL(L);
            System.out.println("--------------------------------\n");
            k++;
        }
        
        genAssociateRules(L);
        
   
        
    }
}
