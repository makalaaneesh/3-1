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
import java.util.Collections;
import java.util.ConcurrentModificationException;
import java.util.Iterator;
import java.util.StringTokenizer;

/**
 *
 * @author makala
 */
public class DIC {
    ArrayList<itemSet> CF = new ArrayList<itemSet>();
    ArrayList<itemSet> CIF = new ArrayList<itemSet>();
    ArrayList<itemSet> SF = new ArrayList<itemSet>();
    ArrayList<itemSet> SIF = new ArrayList<itemSet>();
    ArrayList<itemSet> toaddSIF = new ArrayList<itemSet>();
    int m = 3;
    int minimumSupport = 2;
    String filepath = "/home/makala/Desktop/transactions.txt";
    int currentCount = 0; // index of the transaction currently counting.
    ArrayList<ArrayList> allTransactions;
    
    
    
    //function to read all the transactions into memory
    public ArrayList<ArrayList> readFromFile() throws FileNotFoundException, IOException{
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
    
    //function to get all unique individal items to start off the algorithm
    public  ArrayList<itemSet> getUniqueItems() throws FileNotFoundException, IOException{
        ArrayList<itemSet> returnArr = new ArrayList<>();
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
            for(String item: arr){
                ArrayList<String> temp = new ArrayList<>();
                temp.add(item);
                itemSet tempItemSet = new itemSet(temp,0);
                returnArr.add(tempItemSet);
                
            }
            return returnArr;
    }
    
    
    
    
    //function to read m transactions and update the counts of all the suspected itemsets.
    public void updateCounts(){
        int index = currentCount;
        for(int i = 0; i< m; i++){
            ArrayList<String> trans = allTransactions.get(index);
            index = (index+1)%(allTransactions.size());
            
            for(itemSet iS: SIF){
                if (trans.containsAll(iS.items)){
                    iS.count++;
                }
            }
            for(itemSet iS: SF){
                if (trans.containsAll(iS.items)){
                    iS.count++;
                }
            }
        }
        currentCount = (currentCount+m)%(allTransactions.size());
    }
    public boolean canMerge(itemSet x, itemSet y, int k){
        ArrayList<String> a = x.items;
        ArrayList<String> b = y.items;
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
        return canBeMerged;
    }
    
    public boolean contains(ArrayList<itemSet> all, itemSet s){
        for(itemSet i : all){
            if(s.items.equals(i.items))
                return true;
        }
        return false;
    }
    public boolean isDuplicate(itemSet s){
        if(!contains(SIF,s) && !contains(SF,s) && !contains(CIF,s) && !contains(CF,s) && !contains(toaddSIF,s))
            return false;
        return true;
    }
    
    // function to generate immediate super sets of an itemset that has just been added to the suspected freq 
    // and add to the SIF 
    public void generateImmediateSuperSets(itemSet s){
        for(itemSet i : SF){
            if(i.items.size() == s.items.size() && (!i.equals(s))){
                if(canMerge(s, i, s.items.size()-1)){
                    //merging
//                    System.out.println("merging");
                    ArrayList<String> temp = new ArrayList<>(s.items);
                    temp.add(i.items.get(i.items.size()-1));
                    Collections.sort(temp);
//                    System.out.println("merged is "+ temp);
                    itemSet tempItemSet = new itemSet(temp, currentCount);
                    if(!isDuplicate(tempItemSet))
                        toaddSIF.add(tempItemSet);
                }
            }
        }
        for(itemSet i : CF){
            if(i.items.size() == s.items.size() && (!i.equals(s))){
                if(canMerge(s, i, s.items.size()-1)){
                    //merging
//                    System.out.println("merging");
                    ArrayList<String> temp = new ArrayList<>(s.items);
                    temp.add(i.items.get(i.items.size()-1));
                    Collections.sort(temp);
                    itemSet tempItemSet = new itemSet(temp, currentCount);
                    if(!isDuplicate(tempItemSet))
                        toaddSIF.add(tempItemSet);
                }
            }
        }
    }
    
    // function to promote an itemset to suspected frequent if it's count rises above minimum support
    public void infrequentToFrequent(){
        ArrayList<itemSet> toGen = new ArrayList<>();
        
        Iterator<itemSet> it = SIF.iterator();
        while (it.hasNext()) {
            itemSet iS = it.next();
            if(iS.count >= minimumSupport){
                it.remove();
                SF.add(iS);
                toGen.add(iS);
            }
           
        }
        for(itemSet iS : toGen){
            generateImmediateSuperSets(iS);
        }
    }
    public void suspectedToConfirmed(){
        Iterator<itemSet> it = SIF.iterator();
        while(it.hasNext()){
            itemSet iS = it.next();
            if(iS.startedAt == currentCount){
                it.remove();
                CIF.add(iS);
            }
        }
        it = SF.iterator();
        while(it.hasNext()){
            itemSet iS = it.next();
            if(iS.startedAt == currentCount){
                it.remove();
                CF.add(iS);
            }
        }
        
    }
    
    
    
    public void rectify(){
        for(itemSet i: toaddSIF){
            SIF.add(i);
        }
        toaddSIF.clear();
    }
    
    public void run() throws IOException, ConcurrentModificationException{
        allTransactions = readFromFile();
        SIF = getUniqueItems();
        System.out.println("crrent count is "+ currentCount);
            System.out.println("SIF is "+ SIF);
            System.out.println("SF is "+ SF);
            System.out.println("CIF is "+ CIF);
            System.out.println("CF is "+ CF);
            System.out.println("--------------------------");
        while((SIF.size() != 0)||(SF.size() != 0)){ //as long as there are suspected itemsets
            
            updateCounts(); 
            infrequentToFrequent();
            suspectedToConfirmed();
            rectify();
            
            
            System.out.println("crrent count is "+ currentCount);
            System.out.println("SIF is "+ SIF);
            System.out.println("SF is "+ SF);
            System.out.println("CIF is "+ CIF);
            System.out.println("CF is "+ CF);
            System.out.println("--------------------------");
        }
        
        System.out.println("Final list of Confirmed frequent itemsets are"+ CF);
    }
    
    public static void main(String[] args) throws IOException {
        DIC obj = new DIC();
//        ArrayList<itemSet> a = obj.getUniqueItems();
//        System.out.println(""+a);
        obj.run();
        
                
    }
}
