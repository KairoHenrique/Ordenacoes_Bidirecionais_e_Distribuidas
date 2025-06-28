import java.io.*;
import java.util.ArrayList;

public class QueueArrayBucketSort
{
    static class Rating
    {
        int u,m;float r;long t;
        Rating(int u,int m,float r,long t){this.u=u;this.m=m;this.r=r;this.t=t;}
        public String toString(){return u+","+m+","+r+","+t;}
    }

    Rating[] q; int front=0,rear=0,size=0;
    QueueArrayBucketSort(int cap){q=new Rating[cap];}
    void enqueue(Rating r){q[rear++]=r;size++;}

    void bucketSort()
    {
        @SuppressWarnings("unchecked") ArrayList<Rating>[] buckets=new ArrayList[11];
        for(int i=0;i<11;i++) buckets[i]=new ArrayList<>();
        for(int i=0;i<size;i++) buckets[(int)Math.floor(q[i].r*2)].add(q[i]);

        int k=0;
        for(ArrayList<Rating>b: buckets)for(Rating r:b) q[k++]=r;
        front=0; rear=size;
    }

    public static void main(String[]a)
    throws Exception
    {
        if(a.length<2)
        {
            System.out.println("Uso: java QueueArrayBucketSort <ratings.csv> <N>");
            return;
        }

        int N=Integer.parseInt(a[1]); QueueArrayBucketSort Q=new QueueArrayBucketSort(N);

        try(BufferedReader br=new BufferedReader(new FileReader(a[0])))
        {
            String line=br.readLine();int c=0;
            while((line=br.readLine())!=null&&c<N)
            {
                String[] f=line.split(",");
                Q.enqueue(new Rating(Integer.parseInt(f[0]),Integer.parseInt(f[1]),
                                     Float.parseFloat(f[2]),Long.parseLong(f[3])));
                c++;
            }
        }

        long t0=System.nanoTime();Q.bucketSort();long t1=System.nanoTime();
        System.out.printf("Bucket sort fila vetor %d regs: %.2f ms%n",N,(t1-t0)/1e6);
        for(int i=0;i<20&&i<Q.size;i++)System.out.println(Q.q[i]);
    }
}
