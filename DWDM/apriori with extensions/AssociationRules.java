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
public class AssociationRules {
    
    newalgo a;
    public AssociationRules(newalgo a){
        this.a = a;
        
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
    public void genAllSubsets(ArrayList<String> a,int index, ArrayList<String> partial, int pindex, ArrayList<ArrayList> f){
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
    public void genAssociateRules(ArrayList<ArrayList> L) throws FileNotFoundException, IOException{
        for(ArrayList freq: L){
            ArrayList<String> partial = new ArrayList<String>(10);
            ArrayList<ArrayList> f = new ArrayList<ArrayList>();
            genAllSubsets(freq,0,partial, 0,f);
//            System.out.println("anirudh");
//            printAL(f);
            int supCountFreq = a.freq2(freq); 
            System.out.println("support count "+ supCountFreq);
            for(ArrayList x: f){
                if(x.size()!=freq.size()){
                    a.printA(x);
                    int confidence = (supCountFreq*100/a.freq2(x));
                    System.out.println("\t"+ confidence);
                }
            }
            System.out.println("next");
        }
    }
}
