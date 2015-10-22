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
import java.util.*;

/**
 *
 * @author makala
 */
public class hashing {
    String filepath;
    int minimumSupport;
    HashMap<Integer,Integer> hm;
    
    
    public int getBucketCount(ArrayList<String>s){
        int hashvalue = hash(s);
        return hm.get(hashvalue);
    }
    
    public hashing(String filepath, int ms){
        this.filepath = filepath;
        minimumSupport = ms;
        hm = new HashMap<Integer,Integer>();
    }
    public int hash(ArrayList<String> s){
        int num1 = Integer.parseInt(s.get(0).substring(1));
        int num2 = Integer.parseInt(s.get(1).substring(1));
        int num = (num1*10) + num2;
        return (num%7);
    }
    public ArrayList<ArrayList> genFromHashing(ArrayList<ArrayList> L) throws FileNotFoundException, IOException{
        ArrayList<ArrayList> initialC = new ArrayList<ArrayList>();
        BufferedReader r = new BufferedReader(new FileReader(filepath));
        while(r.ready()){
            ArrayList<String> t = new ArrayList<String>();
            StringTokenizer items = new StringTokenizer(r.readLine()," ");
            while(items.hasMoreTokens()){
                String item = items.nextToken();
                t.add(item);
            }
            for( int i = 0;i<t.size()-1; i++){
                for(int j = i+1; j<t.size(); j++){
                    ArrayList<String> temp = new ArrayList<String>();
                    temp.add(t.get(i));
                    temp.add(t.get(j));
                    if(!initialC.contains(temp))
                        initialC.add(temp);
                    int hashvalue = hash(temp);
                    if (hm.containsKey(hashvalue)){
                        hm.put(hashvalue, hm.get(hashvalue)+1);
                    }
                    else{
                        hm.put(hashvalue, 1);
                    }
                    
                }
            }

        }
       r.close();
       
       ArrayList<ArrayList> C = new ArrayList<ArrayList>();
       for(ArrayList<String> candidate: initialC){
           if(hm.get(hash(candidate)) >= minimumSupport){
               C.add(candidate);
           }
       }
       return C;
    }
}
