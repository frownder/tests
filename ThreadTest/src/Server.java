import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.Collections;
import java.util.Date;
import java.util.LinkedList;
import java.util.Set;
import java.util.TreeSet;


public class Server extends Thread{

	/* io Simple version : many Client Thread 
	public void run(){
		
		try{
			ServerSocket s = new ServerSocket(1990);
			
			//////better: while(true) need
			Socket c = s.accept();  //blocking. so need to create Thread for each Client.
			
			//////better: CleintReadThread need
			byte[] data = new byte[10];
			DataInputStream din = new DataInputStream(c.getInputStream());
			din.readFully(data);
			
			System.out.println( "Server:"+new String(data));
			
		}catch (IOException ioe){
			ioe.printStackTrace();
		}		
	}
	*/
	

	//nio version
	public void run(){
		ServerSocketChannel ssc=null;
		
		try{
			//for SSC
			InetSocketAddress addr = new InetSocketAddress(1990);
			
			ssc = ServerSocketChannel.open();
			ssc.socket().bind(addr);
			
			//for Selector
			Selector accSelector=Selector.open();
			
			ssc.configureBlocking(false); //make nonBlocking accept()
			ssc.register(accSelector, SelectionKey.OP_ACCEPT);
			
			
			while (true){ //select for accept While
				
				int numKeys = accSelector.select(); //select is non Blocking.
				
				if (numKeys > 0){
					System.out.println(" accept numKeys:" + numKeys);
				
					Set<SelectionKey> keySet = accSelector.keys();	
					
					for( SelectionKey key: keySet){
						
						SocketChannel csc = ((ServerSocketChannel) key.channel()).accept();
						System.out.println(" Client Connected ------");
					
						PerClient pClient = new PerClient(csc);
						pClient.start();
					}
				}
				Thread.sleep(10);
			}
			
			
		}catch (Exception e){
			e.printStackTrace();
		}finally{
			try{ if(ssc!=null) ssc.close(); 
			}catch (IOException e){}
		}
		
		
	}
	
	
	class PerClient extends Thread{
		
		SocketChannel csc; //Client Socket Channel
		public PerClient(SocketChannel csc){
			this.csc = csc;
		}
		
		public void run(){
			try{
				Selector readSelector = Selector.open();
				
				csc.configureBlocking(false);
				csc.register(readSelector,SelectionKey.OP_READ);
				
				while (true){
					int numKeys = readSelector.select();//select is non blocking
					
					if(numKeys > 0){
						System.out.println("    ----read numKeys:" + numKeys);
						
						ByteBuffer buff = ByteBuffer.allocateDirect(10);
						int len;
						
						Set<SelectionKey> keySet = readSelector.keys();
						for( SelectionKey key: keySet){
							
							if (key.isReadable())
							//csc is... SocketChannel sc=(SocketChannel)key.channel();
							while ( (len=csc.read(buff))!=-1 ){
								buff.flip();
								
								///WRITE ByteBffer============================================
								System.out.println(  new String( buff.array()) ); 
								buff.clear();
							}
						}
					}//if
					Thread.sleep(10);
				}//while select
				
			}catch(Exception e){
				e.printStackTrace();
			}finally{
				try{ csc.close(); 
				}catch (IOException e){}
			}
			
		
		}
	}
	
	public static void main(String args[]){
	
		Server server = new Server();
		server.start();
		
		Client client = new Client();
		client.start();
		
		Client client2 = new Client();
		client2.start();
		
		//Date////////////////////////////
		Date d = new Date();
		System.out.println( d.getDate() +":" + (d.getMonth()+1) +":" + (d.getYear()+1900) );
		
		
		//sort Comparator/////////  http://promc.tistory.com/11
		LinkedList<Fluit> list = new LinkedList<Fluit>();
		
		
		list.add(new Fluit("apple"));
		list.add(new Fluit("orange"));
		list.add(new Fluit("pear"));
		list.add(new Fluit("bear"));
		list.add(new Fluit("cinomon"));
		
		Collections.sort(list);/////////////GOOD.
		
		
		//auto sort-TreeSet
		TreeSet<Fluit> set = new TreeSet<Fluit>(); 
		set.add(new Fluit("apple"));
		set.add(new Fluit("orange"));
		set.add(new Fluit("pear"));
		set.add(new Fluit("bear"));
		set.add(new Fluit("cinomon"));
		
		for( Fluit f:set){
			//System.out.println(f.getName());
		}
	}
	
	
}
