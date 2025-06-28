import java.io.*;

public class StackPointerPigeonholeSort
{
    static class Rating
    {
        int u,m;float r;long t;
        Rating(int u,int m,float r,long t){this.u=u;this.m=m;this.r=r;this.t=t;}
        public String toString(){return u+","+m+","+r+","+t;}
    }

    static class Node{Rating v;Node nx,pr;Node(Rating v){this.v=v;}}

    Node top=null; int size=0;

    void push(Rating r)
    {
        Node n=new Node(r);
        if(top!=null){n.pr=top;top.nx=n;} 
        top=n; 
        size++;
    }

    void pigeonholeSort()
    {
        final int HOLES=10;

        Node base=top; while(base!=null&&base.pr!=null) base=base.pr;

        @SuppressWarnings("unchecked") java.util.ArrayDeque<Node>[] holes=new java.util.ArrayDeque[HOLES];
        for(int i=0;i<HOLES;i++) holes[i]=new java.util.ArrayDeque<>();

        for(Node p=base;p!=null;p=p.nx) holes[(int)Math.round(p.v.r*2)-1].addLast(p);

        top=null; size=0; Node prev=null;
        for(int h=0;h<HOLES;h++) while(!holes[h].isEmpty())
        {
            Node n=holes[h].pollFirst(); n.nx=null; n.pr=prev;
            if(prev!=null) prev.nx=n;
            prev=n; if(top==null) top=n; size++;
        }

        while(top.nx!=null) top=top.nx;
    }

    public static void main(String[]a)
    throws Exception
    {
        if(a.length<2)
        {
            System.out.println("Uso: java StackPointerPigeonholeSort <ratings.csv> <N>");
            return;
        }

        int N=Integer.parseInt(a[1]); StackPointerPigeonholeSort S=new StackPointerPigeonholeSort();

        try(BufferedReader br=new BufferedReader(new FileReader(a[0])))
        {
            String ln=br.readLine();int c=0;
            while((ln=br.readLine())!=null&&c<N)
            {
                String[] f=ln.split(",");
                S.push(new Rating(Integer.parseInt(f[0]),Integer.parseInt(f[1]),
                                  Float.parseFloat(f[2]),Long.parseLong(f[3])));
                c++;
            }
        }

        long t0=System.nanoTime();S.pigeonholeSort();long t1=System.nanoTime();
        System.out.printf("pilha ponteiro %d regs %.2f ms%n",N,(t1-t0)/1e6);

        Node b=S.top; while(b.pr!=null) b=b.pr;
        for(int i=0;i<20&&b!=null;i++,b=b.nx)System.out.println(b.v);
    }
}
