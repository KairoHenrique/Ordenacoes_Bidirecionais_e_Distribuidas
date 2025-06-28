import java.io.*;

public class ListArrayPigeonholeSort
{
    static class Rating
    {
        int userId, movieId; float rating; long ts;
        Rating(int u,int m,float r,long t){userId=u;movieId=m;rating=r;ts=t;}
        public String toString(){return userId+","+movieId+","+rating+","+ts;}
    }

    Rating[] data; int size=0;
    ListArrayPigeonholeSort(int cap){data=new Rating[cap];}
    void add(Rating r){data[size++]=r;}

    void pigeonholeSort()
    {
        final int HOLES = 10;

        @SuppressWarnings("unchecked")
        java.util.ArrayDeque<Rating>[] holes = new java.util.ArrayDeque[HOLES];
        for(int i=0;i<HOLES;i++) holes[i]=new java.util.ArrayDeque<>();

        for(int i=0;i<size;i++)
        {
            int h=(int)Math.round(data[i].rating*2)-1;
            holes[h].addLast(data[i]);
        }

        int k=0;
        for(int h=0;h<HOLES;h++)
            while(!holes[h].isEmpty()) data[k++]=holes[h].pollFirst();
    }

    public static void main(String[]a)
    throws Exception
    {
        if(a.length<2){System.out.println("Uso: java ListArrayPigeonholeSort <ratings.csv> <N>");return;}
        int N=Integer.parseInt(a[1]);
        ListArrayPigeonholeSort L=new ListArrayPigeonholeSort(N);

        try(BufferedReader br=new BufferedReader(new FileReader(a[0])))
        {
            String ln=br.readLine(); int c=0;
            while((ln=br.readLine())!=null&&c<N)
            {
                String[] f=ln.split(",");
                L.add(new Rating(Integer.parseInt(f[0]),Integer.parseInt(f[1]),
                                 Float.parseFloat(f[2]),Long.parseLong(f[3])));
                c++;
            }
        }

        long t0=System.nanoTime(); L.pigeonholeSort(); long t1=System.nanoTime();
        System.out.printf("lista vetor %d regs %.2f ms%n",N,(t1-t0)/1e6);
        for(int i=0;i<Math.min(20,L.size);i++) System.out.println(L.data[i]);
    }
}
