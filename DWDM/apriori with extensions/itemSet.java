/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package completedapriori;

import java.util.ArrayList;

/**
 *
 * @author makala
 */
public class itemSet {
    ArrayList<String> items;
    int startedAt;
    int count;
    
    public itemSet(ArrayList<String> items, int startedAt){
        this.items = items;
        this.startedAt = startedAt;
        this.count = 0;
    }
    @Override
    public String toString(){
        return "("+items.toString()+" count: "+count+")";
    }
}
