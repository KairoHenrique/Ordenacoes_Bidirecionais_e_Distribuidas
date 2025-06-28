import java.io.*;

public class ListPointerPigeonholeSort
{

    static class Rating
    {
        int u,m;float r;long t;
        Rating(int u,int m,float r,long t){this.u=u;this.m=m;this.r=r;this.t=t;}
        public String toString(){return u+","+m+","+r+","+t;}
    }

    static class Node{Rating v;Node nx,pr;Node(Rating v){this.v=v;}}

    Node head,tail; int size=0;

    void add(Rating r)
    {
        Node n=new Node(r); if(head==null)head=tail=n;
        else{tail.nx=n;n.pr=tail;tail=n;} size++;
    }

    void pigeonholeSort()
    {
        final int HOLES=10;
        @SuppressWarnings("unchecked") java.util.ArrayDeque<Node>[] holes=new java.util.ArrayDeque[HOLES];
        for(int i=0;i<HOLES;i++) holes[i]=new java.util.ArrayDeque<>();

        for(Node p=head;p!=null;p=p.nx)
        {
            int h=(int)Math.round(p.v.r*2)-1; holes[h].addLast(p);
        }

        head=tail=null; size=0;

        for(int h=0;h<HOLES;h++)
            while(!holes[h].isEmpty())
            {
                Node n=holes[h].pollFirst(); n.nx=n.pr=null;
                if(head==null) head=tail=n; 
                else{tail.nx=n;n.pr=tail;tail=n;} 
                size++;
            }
    }

    public static void main(String[]a)
    throws Exception
    {
        if(a.length<2)
        {
            System.out.println("Uso: java ListPointerPigeonholeSort <ratings.csv> <N>");
            return;
        }

        int N=Integer.parseInt(a[1]); ListPointerPigeonholeSort L=new ListPointerPigeonholeSort();

        try(BufferedReader br=new BufferedReader(new FileReader(a[0])))
        {
            String ln=br.readLine();int c=0;
            while((ln=br.readLine())!=null&&c<N)
            {
                String[] f=ln.split(",");
                L.add(new Rating(Integer.parseInt(f[0]),Integer.parseInt(f[1]),
                                 Float.parseFloat(f[2]),Long.parseLong(f[3])));
                c++;
            }
        }

        long t0=System.nanoTime();L.pigeonholeSort();long t1=System.nanoTime();
        System.out.printf("lista ponteiro %d regs %.2f ms%n",N,(t1-t0)/1e6);
        Node p=L.head;for(int i=0;i<20&&p!=null;i++,p=p.nx)System.out.println(p.v);
    }
}
