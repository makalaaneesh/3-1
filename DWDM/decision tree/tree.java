/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package decision.tree;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;

/**
 *
 * @author c137104
 */
public class tree {
    node root;
    ArrayList<HashMap<String,String>> D;
    public tree(ArrayList<HashMap<String,String>> D, ArrayList<String> attributes){
//        System.out.println(""+majortiyClass(D));
        this.D = D;
        root = new node("");
        genNode(D, attributes, root);
//        System.out.println(root);
    }
    public boolean allTuplesOneClass(ArrayList<HashMap<String,String>> D){
        String CLASS = D.get(0).get("class");
        for(int i = 1; i<D.size(); i++){
            if(!D.get(i).get("class").equals(CLASS)){
                return false;
            }
        }
        return true;
    }
    public String majortiyClass(ArrayList<HashMap<String,String>> D){
        HashMap<String, Integer> counts = new HashMap<>();
        for(HashMap<String, String> tuple : D){
            String CLASS = tuple.get("class");
            if (!counts.containsKey(CLASS)){
                counts.put(CLASS, 1);
                
            }
            else{
                counts.put(CLASS, counts.get(CLASS)+1);
            }
        }
        ArrayList<String> classes = new ArrayList<String>(counts.keySet());
        Collections.sort(classes, new Comparator(){

            @Override
            public int compare(Object o1, Object o2) {
//                throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
                return (counts.get(o2)-counts.get(o1));
            }
            
        });
        return classes.get(0);
    }
    public void genNode(ArrayList<HashMap<String,String>> D, ArrayList<String> attributes, node n){
        if (allTuplesOneClass(D)){
            n.tag = 1;
            n.setAttr(majortiyClass(D));
//            n.setValue(majortiyClass(D));
            return;
        }
        if(attributes.size() == 0){
            n.tag = 1;
            n.setAttr(majortiyClass(D));
//            n.setValue(majortiyClass(D));
            return;
        }
        gain obj = new gain(D,attributes);
        String attr = obj.getBestSplittingAttribute();
        n.setAttr(attr);
        ArrayList<String>  attrValues =obj.getAllUniqueValues(attr);
        ArrayList<String> new_attributes = new ArrayList<String>(attributes);
        new_attributes.remove(attr);
        for(String value: attrValues){
            ArrayList<HashMap<String,String>> newD = obj.getPartition(attr, value);
            if(newD.size() == 0){
                node temp = new node(value);
                temp.setAttr("class");
                n.children.add(temp);   
            }
            else{
                node temp = new node("");
                temp.setValue(value);
                n.children.add(temp);
                genNode(newD, new_attributes, temp);
            }
        }
        
    }
    public void printtree() {
        print(root,"", true);
    }

    private void print(node t,String prefix, boolean isTail) {
        System.out.println(prefix + (isTail ? "└── " : "├── ") + t);
        for (int i = 0; i < t.children.size() - 1; i++) {
            print(t.children.get(i),prefix + (isTail ? "    " : "│   "), false);
        }
        if (t.children.size() > 0) {
            print(t.children.get(t.children.size() - 1),prefix + (isTail ?"    " : "│   "), true);
        }
    }
    public void printTree(node n){
        System.out.println(""+n);
        for(node child : n.children){
            printTree(child);
        }
    }
    
    public String predict(HashMap<String, String> tuple, node n){
        if(n.tag == 1){
            System.out.print("Class predicted is "+n.attr);
            return n.attr;
        }
        int index = n.getIndexToTraverse(tuple.get(n.attr));
        return predict(tuple,n.children.get(index));
    }
    public double checkTrainingAccuracy(){
        double total = (double)D.size();
        double correct = 0;
        for (HashMap tuple : D){
            String predictedCLASS = predict(tuple, root);
            System.out.println(":"+predictedCLASS);
            if (predictedCLASS.equals(tuple.get("class"))){
                correct++;
            }
        }
        System.out.println("correct "+correct+" total "+ total);
        return ((double)correct/(double)total)*100.0;
        
    }
}
