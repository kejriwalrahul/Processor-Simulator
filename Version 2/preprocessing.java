/*
	Parse Input assembly into binary IR for loading

	Abinash Patra
	CS14B032
*/

import java.util.*;
import java.io.*;

class PreProcess{
	static HashMap<String, Integer> labels 		= new HashMap<String, Integer>();
	static HashMap<String, String> registers 	= new HashMap<String, String>();
	static HashMap<String, String> memory 		= new HashMap<String, String>();
	static HashMap<String, String> init 		= new HashMap<String,String>();
	static String input = "input";
	static String output = "temp";

	static String twosComplement(Integer bits, Integer val){   //For finding 2's complement incase imm is -ve.
		String temp =  Integer.toBinaryString(val);
		Integer lenDiff = - temp.length() + bits;
		if (lenDiff>0) {
			String padZero = "";
			for (Integer i = lenDiff;i>0 ; i--) {
				padZero += "0";
			}
			return padZero + temp;
		}else if (lenDiff < 0) {
			lenDiff = -lenDiff;
			temp = temp.substring(lenDiff);
		}
		return temp;
	}

	static String instPrint(String inst){ //For printing inst no.
		Integer lenDiff = - inst.length() + 3;
		if (lenDiff == 1) {
			return "0"+inst;
		}else if (lenDiff == 2) {
			return "00"+inst;
		}else{
			return inst;
		}
	}

	static void loadLabels(){ //For loading labels
		try{
			RandomAccessFile raf = new RandomAccessFile(input,"r");
			String temp = ""; Integer inst = 0;
			while(raf.getFilePointer()<raf.length()){
				temp = raf.readLine().trim();
				if (temp.contains(":")) {
					labels.put(temp.split(":")[0],inst);
				}
				if (!temp.contains("##")&&!temp.contains("$")&&temp.length()!=0) {
					inst += 2;
				}
			}
			raf.close();
		}catch(Exception e){e.printStackTrace();}
	}

	static void init(){ //For creating the binary mapping
		for (Integer i=0;i < 16 ;i++ ) {
			init.put("R"+Integer.toString(i),twosComplement(4,i));
			registers.put("R"+Integer.toString(i),"0");
		}
		init.put("ADD","0000");
		init.put("ADDI","0001");
		init.put("SUB","0010");
		init.put("SUBI","0011");
		init.put("MUL","0110");
		init.put("MULI","0111");	
		init.put("LD","1000");
		init.put("SD","1010");
		init.put("JMP","1100");
		init.put("BEQZ","1101");
		init.put("HLT","1110");
	}

	static void translate(){ //For translation from assembly to binary
		try{
			RandomAccessFile raf = new RandomAccessFile(input,"r");
			RandomAccessFile rout = new RandomAccessFile(output,"rw");
			String temp = ""; Integer inst = 0; String tbw = ""; Integer toPrint=1;
			while(raf.getFilePointer()<raf.length()){
				temp = raf.readLine().trim(); tbw = "";
				if (temp.contains(":")) {
					if (!temp.startsWith("##")&&!temp.startsWith("//")) {
						temp = temp.split(":")[1].trim();
					}
				}
				if (temp.contains("##")||temp.contains("$")||temp.length()==0) {
					toPrint = 0;
					if (temp.contains("$") && !temp.contains("##") && temp.length()!=0) {
						String[] tokens = temp.split("\\$");
						if (tokens[2].contains("//")) {
							tokens[2] = tokens[2].split("//")[0].trim();
						}
						if (tokens[1].contains("R")) {
							registers.put(tokens[1],tokens[2]);
						}else{
							memory.put(tokens[1],tokens[2]);
						}
					}
				}else if (temp.startsWith("//")) {
					toPrint = 0;
				}else if (temp.contains("#")){
					String[] tokens = temp.split("\\s+");
					tbw += init.get(tokens[0]+"I");
					tbw += init.get(tokens[1]);					
					tbw += init.get(tokens[2]);
					tbw += twosComplement(4,Integer.valueOf(tokens[3].split("#")[1]));
				}else if (temp.contains("ADD") || temp.contains("SUB") || temp.contains("MUL") ) {
					String[] tokens = temp.split("\\s+");
					tbw += init.get(tokens[0]);
					for(Integer i =1 ; i < 4; i++){
						tbw += init.get(tokens[i]);
					}
				}else if (temp.contains("JMP")) {
					String[] tokens = temp.split("\\s+");
					tbw += init.get(tokens[0]);
					tbw += "0000";
					tbw += String.valueOf(twosComplement(8,(labels.get(tokens[1]))-inst));
				}else if (temp.contains("BEQZ")) {
					String[] tokens = temp.split("\\s+");
					tbw += init.get(tokens[0]);
					tbw += init.get(tokens[1].split("\\(")[1].split("\\)")[0]);
					// System.out.println(tbw);
					// System.out.println(labels.get(tokens[2]));
					tbw += String.valueOf(twosComplement(8,(labels.get(tokens[2]))-inst));
					// System.out.println(tbw);
				}else if (temp.contains("LD")) {
					String[] tokens = temp.split("\\s+");
					tbw += init.get(tokens[0]);
					tbw += init.get(tokens[1]);
					tbw += init.get(tokens[2].split("\\[")[0]);
					tbw += init.get(tokens[2].split("\\[")[1].split("\\]")[0]);
					// System.out.println(tbw);
				}else if (temp.contains("SD")) {
					String[] tokens = temp.split("\\s+");
					tbw += init.get(tokens[0]);
					tbw += init.get(tokens[1].split("\\[")[0]);
					tbw += init.get(tokens[1].split("\\[")[1].split("\\]")[0]);
					tbw += init.get(tokens[2]);
				}else if (temp.contains("HLT")) {
					tbw = init.get("HLT")+"0000"+"0000"+"0000";
				}
				if (toPrint==1) {
					// System.out.println(tbw.length());
					if(tbw.length()==16){
						tbw = "$"+instPrint(String.valueOf(inst))+"$"+tbw+'\n';
						inst += 2;
						rout.writeChars(tbw);	
					}
				}else{
					toPrint = 1;
				}
			}
			raf.close();
			rout.close();
		}catch(Exception e){e.printStackTrace();}
	}

	static void intermediateGen(){ //generate intermediate print it out on screen
		try{
			String toPrint = "";
			for (int i=0; i< 16; i++) {
				toPrint += registers.get("R"+Integer.toString(i)) + " ";
			}
			System.out.println(toPrint);
			for (String key: memory.keySet() ) {
				System.out.println(key+" "+memory.get(key));
			}
			System.out.println("-1 -1");
			RandomAccessFile raf = new RandomAccessFile(output,"rw");
			while(raf.getFilePointer()<raf.length()){
				String tbw = "";
				String tokens = raf.readLine().substring(10);
				for (Integer i=1;i <= 4;i++ ) {
					char[] charArray= tokens.substring(8*(i-1),(8*i)).toCharArray();
					String temp= "";
					for (Integer j=0;j<charArray.length;j++ ){
						if (j%2!=0)						
							temp += charArray[j];
					}
					if (i<=3)				
						tbw += String.valueOf(Integer.parseInt(temp,2))+" ";
					else
						tbw += String.valueOf(Integer.parseInt(temp,2));
				}
				System.out.println(tbw);
			}
			raf.close();
			File file = new File(output);
			file.delete();
		}catch(Exception e){e.printStackTrace();}		
	}


	public static void main(String []args) {
		if (args.length == 1) {
			input = args[0];
			// System.out.println(input);
			// output = args[1];
		}
		loadLabels();
		init();
		translate();
		intermediateGen();
		System.out.println("-1 -1 -1 -1");
   }
}
