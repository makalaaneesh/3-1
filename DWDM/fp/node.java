/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package fp;

import java.util.ArrayList;

/**
 *
 * @author makala
 */
public class node {
    String item;
    int supportCount;
    ArrayList<node> children;
    
    public node(String item){
        this.item = item;
        supportCount = 1;
        children = new ArrayList<>();
    }
    public int getSupportCount(){
        return supportCount;
    }
    public boolean ItemEquals(String item){
        return this.item.equals(item);
    }
    public boolean isLeaf(){
        if(children.size()== 0)
            return true;
        else
            return false;
    }
    public void incrementSupportCount(){
        supportCount ++;
    }
    
    @Override
    public String toString(){
        return item +" :"+supportCount;
    }
}
