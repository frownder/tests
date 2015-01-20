


public class Fluit implements Comparable<Fluit>{
	
	private String name;
	
	public Fluit(String name){
		this.name = name;
	}

	public String getName(){
		return name;
	}
	
	
	public void setName(String name) {
		this.name = name;
	}

	@Override
	public int compareTo(Fluit o) {
		// TODO Auto-generated method stub
		
		return this.name.compareTo( o.name  );
		
	}

}
