import java.io.*;

public class QueueArrayCocktailSort
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

    Rating[] queue;
    int front = 0, rear = 0, size = 0;

    QueueArrayCocktailSort(int capacity)
    {
        queue = new Rating[capacity];
    }

    void enqueue(Rating r)
    {
        queue[rear++] = r;
        size++;
    }

    void cocktailSort()
    {
        if (size <= 1) return;
        boolean swapped = true;
        int start = 0, end = size - 1;

        while (swapped)
        {
            swapped = false;
            for (int i = start; i < end; i++)
            {
                if (queue[i].compareTo(queue[i + 1]) > 0)
                {
                    Rating temp = queue[i];
                    queue[i] = queue[i + 1];
                    queue[i + 1] = temp;
                    swapped = true;
                }
            }

            if (!swapped) break;
            swapped = false;
            end--;

            for (int i = end; i > start; i--)
            {
                if (queue[i - 1].compareTo(queue[i]) > 0)
                {
                    Rating temp = queue[i];
                    queue[i] = queue[i - 1];
                    queue[i - 1] = temp;
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
            System.out.println("Uso: java QueueArrayCocktailSort <ratings.csv> <N>");
            return;
        }

        String path = args[0];
        int N = Integer.parseInt(args[1]);
        QueueArrayCocktailSort fila = new QueueArrayCocktailSort(N);

        try (BufferedReader br = new BufferedReader(new FileReader(path)))
        {
            String line = br.readLine(); int count = 0;
            while ((line = br.readLine()) != null && count < N)
            {
                String[] f = line.split(",");
                fila.enqueue(new Rating(Integer.parseInt(f[0]), Integer.parseInt(f[1]),
                                        Float.parseFloat(f[2]), Long.parseLong(f[3])));
                count++;
            }
        }

        long t0 = System.nanoTime();
        fila.cocktailSort();
        long t1 = System.nanoTime();

        System.out.printf("Ordenado %d registros em %.2f ms\n", N, (t1 - t0) / 1e6);
        for (int i = 0; i < 100 && i < fila.size; i++)
        {
            System.out.println(fila.queue[i]);
        }
    }
}
