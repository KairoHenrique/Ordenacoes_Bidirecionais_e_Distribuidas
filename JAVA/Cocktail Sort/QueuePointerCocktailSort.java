import java.io.*;

public class QueuePointerCocktailSort
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

    static class Node
    {
        Rating r;
        Node next, prev;

        Node(Rating r) { this.r = r; }
    }

    Node head, tail;
    int size = 0;

    void enqueue(Rating r)
    {
        Node n = new Node(r);
        if (head == null) head = tail = n;
        else { tail.next = n; n.prev = tail; tail = n; }
        size++;
    }

    void cocktailSort()
    {
        boolean swapped = true;
        Node start = head, end = tail;

        while (swapped)
        {
            swapped = false;
            for (Node c = start; c != end; c = c.next)
            {
                if (c.r.compareTo(c.next.r) > 0)
                {
                    Rating temp = c.r; c.r = c.next.r; c.next.r = temp;
                    swapped = true;
                }
            }

            if (!swapped) break;
            swapped = false;
            end = end.prev;

            for (Node c = end; c != start; c = c.prev)
            {
                if (c.prev.r.compareTo(c.r) > 0)
                {
                    Rating temp = c.r; c.r = c.prev.r; c.prev.r = temp;
                    swapped = true;
                }
            }
            start = start.next;
        }
    }

    public static void main(String[] args)
    throws Exception
    {
        if (args.length < 2)
        {
            System.out.println("Uso: java QueuePointerCocktailSort <ratings.csv> <N>");
            return;
        }

        String path = args[0];
        int N = Integer.parseInt(args[1]);
        QueuePointerCocktailSort fila = new QueuePointerCocktailSort();

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
        
        Node c = fila.head;
        for (int i = 0; i < 100 && c != null; i++, c = c.next)
        {
            System.out.println(c.r);
        }
    }
}
