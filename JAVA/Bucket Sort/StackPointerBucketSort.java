import java.io.*;
import java.util.ArrayList;

public class StackPointerBucketSort
{
    static class Rating
    {
        int u,m;float r;long t;
        Rating(int u,int m,float r,long t){this.u=u;this.m=m;this.r=r;this.t=t;}
        public String toString(){return u+","+m+","+r+","+t;}
    }

    static class Node{Rating v;Node nxt,prv;Node(Rating v){this.v=v;}}

    Node top=null; int size=0;
    void push(Rating v){Node n=new Node(v);if(top!=null){n.prv=top;top.nxt=n;}top=n;size++;}

    void bucketSort()
    {
        Node base=top; while(base!=null&&base.prv!=null) base=base.prv;

        @SuppressWarnings("unchecked") ArrayList<Node>[] buckets=new ArrayList[11];
        for(int i=0;i<11;i++) buckets[i]=new ArrayList<>();
        for(Node p=base;p!=null;p=p.nxt) buckets[(int)Math.floor(p.v.r*2)].add(p);

        top=null; size=0;

        Node prev=null;
        for(ArrayList<Node>b:buckets)for(Node n:b)
        {
            n.nxt=null; n.prv=prev;
            if(prev!=null) prev.nxt=n;
            prev=n; if(top==null) top=n; size++;
        }

        top=prev;
    }

    public static void main(String[]a)
    throws Exception
    {
        if(a.length<2)
        {
            System.out.println("Uso: java StackPointerBucketSort <ratings.csv> <N>");
            return;
        }

        int N=Integer.parseInt(a[1]); StackPointerBucketSort S=new StackPointerBucketSort();

        try(BufferedReader br=new BufferedReader(new FileReader(a[0])))
        {
            String line=br.readLine();int c=0;
            while((line=br.readLine())!=null&&c<N)
            {
                String[] f=line.split(",");
                S.push(new Rating(Integer.parseInt(f[0]),Integer.parseInt(f[1]),
                                  Float.parseFloat(f[2]),Long.parseLong(f[3])));c++;
            }
        }

        long t0=System.nanoTime();S.bucketSort();long t1=System.nanoTime();
        System.out.printf("Bucket sort pilha ponteiro %d regs: %.2f ms%n",N,(t1-t0)/1e6);

        Node b=S.top; while(b!=null&&b.prv!=null) b=b.prv;
        for(int i=0;i<20&&b!=null;i++,b=b.nxt)System.out.println(b.v);
    }
}
