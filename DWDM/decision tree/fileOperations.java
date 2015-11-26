/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package decision.tree;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.StringTokenizer;

/**
 *
 * @author c137104
 */
public class fileOperations {
    String filepath;
    ArrayList<String> attributes;
    public fileOperations(String path, ArrayList<String> attributes){
        filepath = path;
        this.attributes = attributes;
    }
    public ArrayList<HashMap<String,String>> readFromFile() throws FileNotFoundException, IOException{
        ArrayList<HashMap<String,String>> a = new ArrayList<HashMap<String,String>>();
         BufferedReader r = new BufferedReader(new FileReader(filepath));
            while(r.ready()){
                HashMap<String,String> tuple = new HashMap<String,String>();
                StringTokenizer items = new StringTokenizer(r.readLine()," ");
                int attr = 0;
//                System.out.println("attributes are "+ attributes);
                while(items.hasMoreTokens()){
                    String attr_value = items.nextToken();
//                    System.out.println("attr value for "+ attributes.get(attr)+" is"+ attr_value);
                    tuple.put(attributes.get(attr), attr_value);
                    attr++;
//                    System.out.println(""+attr);
                }
                a.add(tuple);
            }
//        System.out.println(""+ a);
        return a;
    }
}
