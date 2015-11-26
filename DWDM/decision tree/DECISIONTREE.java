/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package decision.tree;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;

/**
 *
 * @author c137104
 */
public class DECISIONTREE {
    String filepath = "dataset.txt";
    ArrayList<String> attributes;
    ArrayList<HashMap<String,String>> D;
    /**
     * @param args the command line arguments
     */
    public DECISIONTREE(){
        attributes = new ArrayList<String>();
        attributes.add("age");
        attributes.add("income");
        attributes.add("student");
        attributes.add("credit_rating");
        attributes.add("class");
    }
    public void run() throws FileNotFoundException, IOException{
        fileOperations blackops = new fileOperations(filepath, attributes);
        D = blackops.readFromFile();
        gain gainObj = new gain(D, attributes);
//        System.out.println(""+ D);
//        System.out.println(""+ gainObj.getProbablity("yes"));
//        System.out.println(""+ gainObj.getProbablity("no"));
//        System.out.println(""+ (gainObj.getProbablity("no")+ gainObj.getProbablity("yes")));
//        System.out.println(""+gainObj.getInfo());
        System.out.println(""+gainObj.getBestSplittingAttribute());
        tree Tree = new tree(D, attributes);
        Tree.printtree();
//        Tree.predict(D.get(0), Tree.root);
        System.out.println(""+Tree.checkTrainingAccuracy());
    }
    public static void main(String[] args) throws FileNotFoundException, IOException {
        // TODO code application logic here
        
        DECISIONTREE obj = new DECISIONTREE();
        obj.run();
        
    }
}
