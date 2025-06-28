import java.io.*;

public class StackArrayPigeonholeSort
{
    static class Rating
    {
        int u,m;float r;long t;
        Rating(int u,int m,float r,long t){this.u=u;this.m=m;this.r=r;this.t=t;}
        public String toString(){return u+","+m+","+r+","+t;}
    }

    Rating[] s; int top=-1;
    StackArrayPigeonholeSort(int cap){s=new Rating[cap];}
    void push(Rating r){s[++top]=r;}

    void pigeonholeSort()
    {
        final int HOLES=10;

        @SuppressWarnings("unchecked") java.util.ArrayDeque<Rating>[] holes=new java.util.ArrayDeque[HOLES];
        for(int i=0;i<HOLES;i++) holes[i]=new java.util.ArrayDeque<>();
        for(int i=0;i<=top;i++) holes[(int)Math.round(s[i].r*2)-1].addLast(s[i]);

        int k=0;
        for(int h=0;h<HOLES;h++) while(!holes[h].isEmpty()) s[k++]=holes[h].pollFirst();
        top=k-1;
    }

    public static void main(String[]a)
    throws Exception
    {
        if(a.length<2)
        {
            System.out.println("Uso: java StackArrayPigeonholeSort <ratings.csv> <N>");
            return;
        }

        int N=Integer.parseInt(a[1]); StackArrayPigeonholeSort S=new StackArrayPigeonholeSort(N);

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
        System.out.printf("pilha vetor %d regs %.2f ms%n",N,(t1-t0)/1e6);
        for(int i=0;i<Math.min(20,S.top+1);i++)System.out.println(S.s[i]);
    }
}
