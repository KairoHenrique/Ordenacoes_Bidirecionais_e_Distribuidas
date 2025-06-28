import java.io.*;

public class QueueArrayPigeonholeSort
{
    static class Rating
    {
        int u,m;float r;long t;
        Rating(int u,int m,float r,long t){this.u=u;this.m=m;this.r=r;this.t=t;}
        public String toString(){return u+","+m+","+r+","+t;}
    }

    Rating[] q; int size=0;
    QueueArrayPigeonholeSort(int cap){q=new Rating[cap];}
    void enqueue(Rating r){q[size++]=r;}

    void pigeonholeSort()
    {
        final int HOLES=10;
        @SuppressWarnings("unchecked") java.util.ArrayDeque<Rating>[] holes=new java.util.ArrayDeque[HOLES];
        for(int i=0;i<HOLES;i++) holes[i] = new java.util.ArrayDeque<>();

        for(int i=0;i<size;i++) holes[(int)Math.round(q[i].r*2)-1].addLast(q[i]);
        int k=0;
        for(int h=0;h<HOLES;h++) while(!holes[h].isEmpty()) q[k++] = holes[h].pollFirst();
    } 

    public static void main(String[]a)
    throws Exception
    {
        if(a.length<2)
        {
            System.out.println("Uso: java QueueArrayPigeonholeSort <ratings.csv> <N>");
            return;
        }

        int N=Integer.parseInt(a[1]); QueueArrayPigeonholeSort Q=new QueueArrayPigeonholeSort(N);

        try(BufferedReader br=new BufferedReader(new FileReader(a[0])))
        {
            String ln=br.readLine();int c=0;
            while((ln=br.readLine())!=null&&c<N)
            {
                String[] f=ln.split(",");
                Q.enqueue(new Rating(Integer.parseInt(f[0]),Integer.parseInt(f[1]),
                                     Float.parseFloat(f[2]),Long.parseLong(f[3])));
                c++;
            }
        }
        
        long t0=System.nanoTime();Q.pigeonholeSort();long t1=System.nanoTime();
        System.out.printf("fila vetor %d regs %.2f ms%n",N,(t1-t0)/1e6);
        for(int i=0;i<Math.min(20,Q.size);i++)System.out.println(Q.q[i]);
    }
}
