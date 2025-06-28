import java.io.*;
import java.util.ArrayList;

public class ListArrayBucketSort
{
    static class Rating
    {
        int userId, movieId; float rating; long ts;
        Rating(int u,int m,float r,long t){userId=u;movieId=m;rating=r;ts=t;}
        public String toString(){return userId+","+movieId+","+rating+","+ts;}
    }

    Rating[] data; int size=0;
    ListArrayBucketSort(int cap){data=new Rating[cap];}
    void add(Rating r){data[size++]=r;}

    void bucketSort()
    {
        @SuppressWarnings("unchecked")
        ArrayList<Rating>[] buckets=new ArrayList[11];
        for(int i=0;i<buckets.length;i++) buckets[i]=new ArrayList<>();

        for(int i=0;i<size;i++)
        {
            int idx=(int)Math.floor(data[i].rating*2);
            buckets[idx].add(data[i]);
        }

        int k=0;
        for(ArrayList<Rating> b: buckets)
            for(Rating r:b) data[k++]=r;
    }

    public static void main(String[]a)
    throws Exception
    {
        if(a.length<2)
        {
            System.out.println("Uso: java ListArrayBucketSort <ratings.csv> <N>");
            return;
        }

        int N=Integer.parseInt(a[1]); ListArrayBucketSort list=new ListArrayBucketSort(N);

        try(BufferedReader br=new BufferedReader(new FileReader(a[0])))
        {
            String line=br.readLine(); int c=0;
            while((line=br.readLine())!=null&&c<N)
            {
                String[] f=line.split(",");
                list.add(new Rating(Integer.parseInt(f[0]),Integer.parseInt(f[1]),
                                    Float.parseFloat(f[2]),Long.parseLong(f[3])));
                c++;
            }
        }
        
        long t0=System.nanoTime(); list.bucketSort(); long t1=System.nanoTime();
        System.out.printf("Bucket sort lista vetor %d regs: %.2f ms%n",N,(t1-t0)/1e6);
        for(int i=0;i<Math.min(20,list.size);i++) System.out.println(list.data[i]);
    }
}
