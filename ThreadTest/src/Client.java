import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;


public class Client extends Thread{

	private Socket socket;
	private String sending = "1234567890";
	
	/* PlainSocket
	public void run(){
		try{
			socket = new Socket("127.0.0.1",1990);
			
			DataOutputStream dout = new DataOutputStream(socket.getOutputStream());
			
			for(int i=0;i<10;i++)
				dout.write( sending.getBytes() );
			System.out.println("Client: sent");
			
		}catch (IOException ioe){
			ioe.printStackTrace();
		}
	}*/
	
	//nio Socket
	public void run(){
		
		SocketChannel csc=null;
		try{
			Thread.sleep(1000);//for late connect
			
			//for csc
			InetSocketAddress addr = new InetSocketAddress("localhost", 1990);
			
			csc = SocketChannel.open(addr);
			
			//ByteBuffer buff = ByteBuffer.allocateDirect(10);
			//buff.put(sending.getBytes());
			
			ByteBuffer buff = ByteBuffer.wrap(sending.getBytes());
			csc.write(buff);
			
			
			
		}catch(Exception e){
			e.printStackTrace();
		}finally{
			try{ if(csc!=null)csc.close(); 
			}catch (IOException e){}
		}
		
	}
}
