import java.io.*;
import java.util.ArrayList;

public class ListPointerBucketSort
{
    static class Rating
    {
        int u,m;float r;long t;
        Rating(int u,int m,float r,long t){this.u=u;this.m=m;this.r=r;this.t=t;}
        public String toString(){return u+","+m+","+r+","+t;}
    }

    static class Node{Rating val;Node nxt,prv;Node(Rating v){val=v;}}

    Node head,tail; int size=0;

    void add(Rating r)
    {
        Node n=new Node(r);if(head==null)head=tail=n;
        else{tail.nxt=n;n.prv=tail;tail=n;}size++;
    }

    void bucketSort()
    {
        @SuppressWarnings("unchecked")
        ArrayList<Node>[] buckets=new ArrayList[11];
        for(int i=0;i<buckets.length;i++) buckets[i]=new ArrayList<>();

        for(Node p=head;p!=null;p=p.nxt)
        {
            int idx=(int)Math.floor(p.val.r*2);
            buckets[idx].add(p);
        }

        head=tail=null; size=0;

        for(ArrayList<Node>b:buckets)
            for(Node n:b)
            {
                n.nxt=n.prv=null;
                if(head==null)head=tail=n;else{tail.nxt=n;n.prv=tail;tail=n;}
                size++;
            }
    }

    public static void main(String[]a)
    throws Exception
    {
        if(a.length<2)
        {
            System.out.println("Uso: java ListPointerBucketSort <ratings.csv> <N>");
            return;
        }

        int N=Integer.parseInt(a[1]); ListPointerBucketSort L=new ListPointerBucketSort();

        try(BufferedReader br=new BufferedReader(new FileReader(a[0])))
        {
            String line=br.readLine();int c=0;
            while((line=br.readLine())!=null&&c<N)
            {
                String[] f=line.split(",");
                L.add(new Rating(Integer.parseInt(f[0]),Integer.parseInt(f[1]),
                                 Float.parseFloat(f[2]),Long.parseLong(f[3])));
                c++;
            }
        }
        
        long t0=System.nanoTime();L.bucketSort();long t1=System.nanoTime();
        System.out.printf("Bucket sort lista ponteiro %d regs: %.2f ms%n",N,(t1-t0)/1e6);
        Node p=L.head;for(int i=0;i<20&&p!=null;i++,p=p.nxt)System.out.println(p.val);
    }
}
