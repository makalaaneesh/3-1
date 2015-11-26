/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package decision.tree;

import com.sun.xml.internal.bind.v2.runtime.RuntimeUtil;
import java.util.ArrayList;

/**
 *
 * @author c137104
 */
public class node {
    String attr;
    String value;
    int tag;
    ArrayList<node> children;
    public node(String value){
        this.value= value;
        children = new ArrayList<node>();
        tag = 0;
    }
    public void setAttr(String attr){
        this.attr = attr;
    }
    public void setValue(String val){
        value = val;
    }
    
    public int getIndexToTraverse(String value){
        for(node t: children){
            if (t.value.equals(value)){
                return children.indexOf(t);
            }
        }
        return -1;
    }
    @Override
    public String toString(){
        return (value+"->"+attr);
    }
}
