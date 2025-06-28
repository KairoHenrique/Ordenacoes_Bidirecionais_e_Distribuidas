import java.io.*;

public class StackArrayCocktailSort
{
    static class Rating
    {
        int userId, movieId;
        float rating;
        long timestamp;

        Rating(int u, int m, float r, long t)
        {
            userId = u; movieId = m; rating = r; timestamp = t;
        }

        int compareTo(Rating o)
        {
            return Float.compare(this.rating, o.rating);
        }

        public String toString()
        {
            return userId + "," + movieId + "," + rating + "," + timestamp;
        }
    }

    Rating[] stack;
    int top = -1;

    StackArrayCocktailSort(int capacity)
    {
        stack = new Rating[capacity];
    }

    void push(Rating r)
    {
        stack[++top] = r;
    }

    void cocktailSort()
    {
        boolean swapped = true;
        int start = 0, end = top;

        while (swapped)
        {
            swapped = false;
            for (int i = start; i < end; i++)
            {
                if (stack[i].compareTo(stack[i + 1]) > 0) 
                {
                    Rating temp = stack[i];
                    stack[i] = stack[i + 1];
                    stack[i + 1] = temp;
                    swapped = true;
                }
            }

            if (!swapped) break;
            swapped = false;
            end--;

            for (int i = end; i > start; i--)
            {
                if (stack[i - 1].compareTo(stack[i]) > 0)
                {
                    Rating temp = stack[i];
                    stack[i] = stack[i - 1];
                    stack[i - 1] = temp;
                    swapped = true;
                }
            }
            start++;
        }
    }

    public static void main(String[] args)
    throws Exception
    {
        if (args.length < 2)
        {
            System.out.println("Uso: java StackArrayCocktailSort <ratings.csv> <N>");
            return;
        }

        String path = args[0];
        int N = Integer.parseInt(args[1]);
        StackArrayCocktailSort pilha = new StackArrayCocktailSort(N);

        try (BufferedReader br = new BufferedReader(new FileReader(path)))
        {
            String line = br.readLine(); int count = 0;
            while ((line = br.readLine()) != null && count < N)
            {
                String[] f = line.split(",");
                pilha.push(new Rating(Integer.parseInt(f[0]), Integer.parseInt(f[1]),
                                      Float.parseFloat(f[2]), Long.parseLong(f[3])));
                count++;
            }
        }

        long t0 = System.nanoTime();
        pilha.cocktailSort();
        long t1 = System.nanoTime();

        System.out.printf("Ordenado %d registros em %.2f ms\n", N, (t1 - t0) / 1e6);
        for (int i = 0; i <= Math.min(100, pilha.top); i++)
        {
            System.out.println(pilha.stack[i]);
        }
    }
}
