import java.io.*;

public class StackPointerCocktailSort
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
        Node prev, next;

        Node(Rating r) { this.r = r; }
    }

    Node top = null;
    int size = 0;

    void push(Rating r)
    {
        Node n = new Node(r);
        if (top != null)
        {
            top.next = n;
            n.prev = top;
        }
        top = n;
        size++;
    }

    void cocktailSort()
    {
        if (size <= 1) return;

        Node base = top;
        while (base.prev != null) base = base.prev;

        boolean swapped = true;
        Node start = base, end = top;

        while (swapped)
        {
            swapped = false;
            for (Node c = start; c != end; c = c.next)
            {
                if (c.r.compareTo(c.next.r) > 0)
                {
                    Rating tmp = c.r;
                    c.r = c.next.r;
                    c.next.r = tmp;
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
                    Rating tmp = c.r;
                    c.r = c.prev.r;
                    c.prev.r = tmp;
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
            System.out.println("Uso: java StackPointerCocktailSort <ratings.csv> <N>");
            return;
        }

        String path = args[0];
        int N = Integer.parseInt(args[1]);
        StackPointerCocktailSort pilha = new StackPointerCocktailSort();

        try (BufferedReader br = new BufferedReader(new FileReader(path)))
        {
            String line = br.readLine();
            int count = 0;
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

        Node cur = pilha.top;
        while (cur.prev != null) cur = cur.prev;

        for (int i = 0; i < 100 && cur != null; i++, cur = cur.next)
        {
            System.out.println(cur.r);
        }
    }
}
