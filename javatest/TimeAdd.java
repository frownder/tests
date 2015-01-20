import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.HashMap;

import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathConstants;
import javax.xml.xpath.XPathFactory;

import org.w3c.dom.Document;
import org.xml.sax.InputSource;


public class TimeAdd {

	private static SimpleDateFormat sFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
	
	
	public static String timeAdd(String origin, String offSecond){
		
		try{
			Date oDate = sFormat.parse(origin);
			
			Date rDate = new Date(oDate.getTime() + Integer.parseInt(offSecond)*1000 ); //ms
			
			//cal.setTime( date ), cal.getTime( date) 
			//cal.add(Calendar.SECOND, Integer.parseInt(offSec));
			
			return sFormat.format(rDate.getTime()); 
			
		}catch (Exception e){
			return " input time Format Error";
		}
		
	}
	
	public static boolean useJson = true;
	/**
	 * add local time to the input timeSring data.
	 * @param originTime
	 * @return localTime added.
	 * @reutn "Err+" when error occurs.: for notice purpose
	 */
	public static String getTimeDB(String originTime){
		
		String[] timeCords = originTime.split(",");
		
		if (timeCords.length !=3)
			return "Err: not well-fomred data";
			
		
		try {
			URL apiUrl = new URL(baseUrl+"lat="+timeCords[1]+"&lng="+timeCords[2]);
			if(useJson)
				apiUrl = new URL( apiUrl.toString() + "&format=json");
			//System.out.println(apiUrl);
			
			try {
				HttpURLConnection con = (HttpURLConnection)apiUrl.openConnection();
				con.setRequestMethod("GET");
				
				String zone, time;
				
				
				if(!useJson){ //using XML
					
					//using XPath
					InputSource is = new InputSource( new InputStreamReader(con.getInputStream()));
					
					Document doc = DocumentBuilderFactory.newInstance()
										.newDocumentBuilder().parse(is);
					
					XPath xpath = XPathFactory.newInstance().newXPath();
					zone = (String)xpath.evaluate("/result/zoneName", doc, XPathConstants.STRING);
					
					String offTime = (String)xpath.evaluate("/result/gmtOffset", doc, XPathConstants.STRING);			
					time = timeAdd(timeCords[0], offTime);
					
				}
				else{  /////using JSON ? with noLib ////////////////////////////
					
					BufferedReader br = new BufferedReader(new InputStreamReader(con.getInputStream()));
					
					String line;
					StringBuffer allLine= new StringBuffer("");
					
					while ( (line=br.readLine())!=null){
						allLine.append(line);
					}
					MyJson mj = new MyJson(allLine.toString());
						
					zone=mj.get("zoneName");
					
					String offTime = mj.get("gmtOffset");
					time = timeAdd(timeCords[0], offTime);
				}
				/*
				String line;
				String zone="";
				String offTime="";
				String time="";
				
				BufferedReader br= new BufferedReader( 
						new InputStreamReader( con.getInputStream() ).);
				
				while((line = br.readLine())!=null){
					
					if( line.contains("<zoneName>")){
						zone = line.substring( line.indexOf(">")+1, line.lastIndexOf("<") );
					}
					if( line.contains("<gmtOffset>")){
						offTime = line.substring( line.indexOf(">")+1, line.lastIndexOf("<") );
						time = timeAdd(timeCords[0], offTime);
						//System.out.println("in:"+timeCords[0] + " out:"+time + " offTime:" +offTime);
					}
					
				}
				br.close();
				*/
				
				String result = timeCords[0] + "," +zone + ","+ time; //or line + zone + time
				System.out.println("  writing:" + result);
				return result; 
				
			} catch (Exception e) {
				return "Err:"+ e.toString();
			}
			
			
		} catch (MalformedURLException e) {
			return "Err:"+e.toString();
		}
		
		
		//return "Err:nothing done";
	}
	
	//private static String url="http://www.earthtools.org/timezone/";
	private static String baseUrl="http://api.timezonedb.com/?key=7JCJXBNELG14&"; //howTo- http://timezonedb.com/api 
	
	
	
	
	public static void main(String[] args){
		
		String outFileName = "output.txt";
		
		if(args.length < 1){
			System.out.println("USAGE: java TimeAdd inputfile [outfilename]");
			return;
		}
		if (args.length==2)
			outFileName = args[1];
		
		
		//Scanner s=null;
		BufferedReader br=null;
		PrintWriter pw=null;
		
		try{
			
			//s = new Scanner(new BufferedReader(new FileReader(args[0])));
			//s.useDelimiter(",\\s*");
			//while (s.hasNext()){
			
			br = new BufferedReader(new FileReader(args[0]));
			pw = new PrintWriter(new FileWriter(outFileName));
			
			String line;
			while ( (line=br.readLine()) !=null) {
				
				System.out.println("processing:" + line);	
				pw.println( getTimeDB(line) );
			}
			
			br.close();
		}catch (FileNotFoundException fe){
			fe.printStackTrace();
		}catch (IOException ioe){
			ioe.printStackTrace();
		}finally{
			//if(br!=null)
			//	br.close();
			if(pw!=null)
				pw.close();
		}
		
	}//main
		
}


class MyJson{
	
	private HashMap<String, String> hm = new HashMap<String,String>();
	
	public MyJson(String json){
		String normal = json.replaceAll("[{\"}]", ""); //remove {,", }
		
		String[] kvset = normal.split(",\\s*"); 
		
		for( String s : kvset){
			
			String kv[] = s.split(":");
			
			if( kv.length==2)
				hm.put(kv[0], kv[1]);
			else{
				System.err.println("Json Parse err: "+s);
			}
		}
	}
	
	public String get(String key){
		return hm.get(key);
	}	
	
}