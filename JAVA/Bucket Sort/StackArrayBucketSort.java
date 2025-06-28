import java.io.*;
import java.util.ArrayList;

public class StackArrayBucketSort
{
    static class Rating
    {
        int u,m;float r;long t;
        Rating(int u,int m,float r,long t){this.u=u;this.m=m;this.r=r;this.t=t;}
        public String toString(){return u+","+m+","+r+","+t;}
    }

    Rating[] s; int top=-1;
    StackArrayBucketSort(int cap){s=new Rating[cap];}
    void push(Rating r){s[++top]=r;}

    void bucketSort()
    {
        @SuppressWarnings("unchecked") ArrayList<Rating>[] buckets=new ArrayList[11];
        for(int i=0;i<11;i++) buckets[i]=new ArrayList<>();
        for(int i=0;i<=top;i++) buckets[(int)Math.floor(s[i].r*2)].add(s[i]);

        int k=0;
        for(ArrayList<Rating>b:buckets)for(Rating r:b) s[k++]=r;
        top=k-1;
    }

    public static void main(String[]a)
    throws Exception
    {
        if(a.length<2)
        {
            System.out.println("Uso: java StackArrayBucketSort <ratings.csv> <N>")
            ;return;
        }

        int N=Integer.parseInt(a[1]); StackArrayBucketSort S=new StackArrayBucketSort(N);

        try(BufferedReader br=new BufferedReader(new FileReader(a[0])))
        {
            String line=br.readLine();int c=0;
            while((line=br.readLine())!=null&&c<N)
            {
                String[] f=line.split(",");
                S.push(new Rating(Integer.parseInt(f[0]),Integer.parseInt(f[1]),
                                  Float.parseFloat(f[2]),Long.parseLong(f[3])));
                c++;
            }
        }

        long t0=System.nanoTime();S.bucketSort();long t1=System.nanoTime();
        System.out.printf("Bucket sort pilha vetor %d regs: %.2f ms%n",N,(t1-t0)/1e6);
        for(int i=0;i<20&&i<=S.top;i++)System.out.println(S.s[i]);
    }
}
