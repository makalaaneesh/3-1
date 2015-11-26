/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package decision.tree;

import java.util.ArrayList;
import java.util.HashMap;

/**
 *
 * @author c137104
 */
public class gain {
    ArrayList<HashMap<String,String>> D; //table
    ArrayList<String> attributes;
    ArrayList<String> classes;
    public gain(ArrayList<HashMap<String,String>> table, ArrayList<String> attributes){
        D = table;
        this.attributes = attributes;
        classes = getAllUniqueValues("class");
    }
    
    public ArrayList<String> getAllUniqueValues(String attribute){
        ArrayList<String> ofTheMummy = new ArrayList<String>();
        for(HashMap<String,String> tuple: D){
            String value = tuple.get(attribute);
            if (!ofTheMummy.contains(value)){
                ofTheMummy.add(value);
            }
        }
        return ofTheMummy;
    }
    public ArrayList<HashMap<String,String>> getPartition(String attribute, String value){
        ArrayList<HashMap<String,String>> ofTheMummy = new ArrayList<HashMap<String,String>>();
        for(HashMap<String,String> tuple: D){
            if(tuple.get(attribute).equals(value)){
                ofTheMummy.add(tuple);
            }
        }
        return ofTheMummy;
    }
    public double getProbablity(String CLASS){
        int freq = 0;
        int total = D.size();
        for(HashMap<String,String> tuple: D){
            if (tuple.get("class").equals(CLASS)){
                freq++;
            }
        }
        double p = (double)freq/(double)total;
        return p;
    }
    
    public double getInfo(){
        double total = 0;
        for(String CLASS: classes){
            double p = getProbablity(CLASS);
            double p2 = Math.log(p)/Math.log(2);
            double p3 = -1 * p * p2;
            total = total + p3;
        }
        return total;
    }
    public double getInfo(String attribute){
        double total = 0;
        ArrayList<String> values = getAllUniqueValues(attribute);
        for(String value: values){
            ArrayList<HashMap<String,String>> partition = getPartition(attribute,value);
            int psize = partition.size();
            int totalsize = D.size();
            gain partitionobj = new gain(partition, attributes);
            double i = (partitionobj.getInfo()*(double)psize)/(double)totalsize;
            total = total + i;
        }
        return total;
    }
    public double getGain(String attribute){
        return (getInfo() - getInfo(attribute));
    }
    public String getBestSplittingAttribute(){
        double max = 0;
        String attr = "";
        for(String attribute: attributes){
            if (attribute.equals("class"))
                continue;
            if(getGain(attribute)>max){
                max = getGain(attribute);
                attr = attribute;
            }
        }
        return attr;
    }
}
