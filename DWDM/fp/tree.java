/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package fp;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.Iterator;

/**
 *
 * @author makala
 */
public class tree {
    node root;
    HashMap<String, Integer> supportCounts;
    int minimumSupport;
    ArrayList<ArrayList<node>> allPaths;
    String prefix;
    ArrayList<ArrayList<String>> freqItemSets;
    
    public class itemComparator implements Comparator<String>
    {
        @Override
        public int compare(String o1, String o2) {
//            throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
            if(!supportCounts.containsKey(o1)){
                return 1;
            }
            else if (!supportCounts.containsKey(o2)){
                return -1;
            }
            else{                
                return supportCounts.get(o2) - supportCounts.get(o1);
            }
        }
    }
     public tree(ArrayList<ArrayList<String>> all, int minsup,String prefix){
         System.out.println("__________________________");
        minimumSupport = minsup;
        this.prefix = prefix;
        root = new node("");
        supportCounts = new HashMap<String, Integer>();
        allPaths = new ArrayList<>();
        freqItemSets = new ArrayList<>();
        processSupportCounts(all);
        System.out.println("all items above minsup are "+ supportCounts.keySet());
        for (ArrayList<String> transaction: all){
            add(transaction);
        }
//        add(all.get(0));
//        add(all.get(1));
//        add(all.get(2));
        
        ArrayList<node> path = new ArrayList(Collections.nCopies(100, new node("")));
        genpaths(root, path, 0);
    }
    
    
    public void processSupportCounts(ArrayList<ArrayList<String>> all){
        for (ArrayList<String> transaction : all){
            for (String item : transaction){
                if(supportCounts.containsKey(item)){
                    int oldCount = supportCounts.get(item);
                    supportCounts.put(item, oldCount+1);
                }
                else{
                    supportCounts.put(item, 1);
                }
            }
        }
        Iterator<String> it = supportCounts.keySet().iterator();
        while(it.hasNext()){
            String key = it.next();
            if (supportCounts.get(key) < minimumSupport){
                it.remove();
            }
        }
//        for (String key:supportCounts.keySet()){
//            if (supportCounts.get(key) < minimumSupport){
//                supportCounts.remove(key);
//            }
//        }
    }
    
    public ArrayList<String> sortBySupportCount(ArrayList<String> transaction){
        ArrayList<String> temp = new ArrayList<>(transaction);
        Collections.sort(temp, new itemComparator());
        Iterator<String> it = temp.iterator();
        while (it.hasNext()) {
            String item = it.next();
            if(!supportCounts.containsKey(item)){
                it.remove();
                
            }
           
        }
        return temp;
    }
    
    
    public void add(ArrayList<String> transaction){
        ArrayList<String> sortedTransaction = sortBySupportCount(transaction);
        System.out.println(""+sortedTransaction);
        addItem(root, sortedTransaction, 0);
    }
    
    public int childExists(node t, String item){
        for(node child:t.children){
            if(child.ItemEquals(item)){
                return t.children.indexOf(child);
            }
        }
        return -1;
    }
    
    public void addItem(node t, ArrayList<String> transaction, int index){
        if (index >= transaction.size())
            return;
        String item = transaction.get(index);
        int childIndex;
        if ((childIndex = childExists(t,item)) >=0){
//            System.out.println("childindex for"+item +"in "+t+ " is" + childIndex);
            t.children.get(childIndex).incrementSupportCount();
            addItem(t.children.get(childIndex), transaction, index+1);
        }
        else{
//            System.out.println("childindex for"+item +"in "+t+ " is" + childIndex);
            node temp  = new node(item);
            t.children.add(temp);
            addItem(temp,transaction,index+1);
        }
        
    }
     public void printTree() {
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
    
    public void addpath(ArrayList<node> path, int index){
        ArrayList<node> temp = new ArrayList(path.subList(0, index));
//        System.out.println(""+ temp);
        allPaths.add(temp);
    }
    public void genpaths(node t, ArrayList<node> path, int index){
        for(node child: t.children){
            path.set(index, child);
            genpaths(child, path,index+1);
        }
        if(t.isLeaf()){
            addpath(path, index);
        }
    }
    public int contains(ArrayList<node> path, String item){
        for(node t:path){
            if(t.ItemEquals(item))
                return path.indexOf(t);
        }
        return -1;
    }
    
    public ArrayList<ArrayList<String>> allPathsEndingIn(String item){
        ArrayList<ArrayList<String>> newpaths = new ArrayList<>();
        for(ArrayList<node> path: allPaths){
//            if(path.size() == 1 && (path.get(0).equals(item))){
//                ArrayList<String> temp = new ArrayList<>();
//                newpaths.add(temp);
//            }
//            else if(path.get(path.size()-1).equals(item)){
//                ArrayList<String> temp = new ArrayList(path.subList(0, path.size()-1));
//                newpaths.add(temp);
//            }
            if(contains(path,item) !=-1){
                ArrayList<node> temp = new ArrayList<>(path.subList(0, contains(path,item)));
                ArrayList<String> temp2 = new ArrayList<>();
                for(node t: temp){
                    temp2.add(t.item);
                }
                for(int i = 0; i<path.get(contains(path,item)).supportCount; i++){
                    ArrayList<String> tempToAdd = new ArrayList<>(temp2);
                    if(tempToAdd.size()>0)
                        newpaths.add(tempToAdd);
                }
                
            }
        }
        return newpaths;
    }
    public ArrayList<ArrayList<node>> getPaths(){
        
        return allPaths;
    }
    public ArrayList<ArrayList<String>> getFreqItemSets(){
        return freqItemSets;
    }
    public void algo(){
        ArrayList<String> items = new ArrayList(supportCounts.keySet());
        items = sortBySupportCount(items);
        
        for(int i = items.size()-1; i>=0;  i--){
            System.out.println("Tree for ending in "+items.get(i)+" with prefix "+prefix);
            //generating prefix
            String pre = items.get(i)+ prefix;
            ArrayList<String> temp = new ArrayList<>();
            temp.add(pre);
            freqItemSets.add(temp);
            System.out.println("all paths for tree with prefix "+prefix+" ending in  "+items.get(i)+" are "+allPathsEndingIn(items.get(i)));
            FP endingIn = new FP(allPathsEndingIn(items.get(i)),pre);
            ArrayList<ArrayList<String>> e = endingIn.getFIS();
            freqItemSets.addAll(e);
            
            
            
        }
        
    }
}
