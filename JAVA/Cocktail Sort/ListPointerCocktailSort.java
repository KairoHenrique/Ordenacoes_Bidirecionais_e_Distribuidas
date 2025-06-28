import java.io.*;

public class ListPointerCocktailSort
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

    public void add(Rating r)
    {
        Node n = new Node(r);
        if (head == null) head = tail = n;
        else { tail.next = n; n.prev = tail; tail = n; }
        size++;
    }

    public void cocktailSort()
    {
        if (head == null) return;
        boolean swapped = true;
        Node start = head, end = tail;

        while (swapped)
        {
            swapped = false;
            for (Node cur = start; cur != end; cur = cur.next)
            {
                if (cur.r.compareTo(cur.next.r) > 0) {
                    Rating temp = cur.r; cur.r = cur.next.r; cur.next.r = temp;
                    swapped = true;
                }
            }

            if (!swapped) break;
            swapped = false;
            end = end.prev;

            for (Node cur = end; cur != start; cur = cur.prev)
            {
                if (cur.prev.r.compareTo(cur.r) > 0)
                {
                    Rating temp = cur.r; cur.r = cur.prev.r; cur.prev.r = temp;
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
            System.out.println("Uso: java ListPointerCocktailSort <ratings.csv> <N>");
            return;
        }

        String path = args[0];
        int N = Integer.parseInt(args[1]);
        ListPointerCocktailSort lista = new ListPointerCocktailSort();

        try (BufferedReader br = new BufferedReader(new FileReader(path))) 
        {
            String line = br.readLine();
            int count = 0;

            while ((line = br.readLine()) != null && count < N) 
            {
                String[] f = line.split(",");
                lista.add(new Rating(Integer.parseInt(f[0]), Integer.parseInt(f[1]),
                                     Float.parseFloat(f[2]), Long.parseLong(f[3])));
                count++;
            }
        }

        long t0 = System.nanoTime();
        lista.cocktailSort();
        long t1 = System.nanoTime();

        System.out.printf("Ordenado %d registros em %.2f ms\n", N, (t1 - t0) / 1e6);
        Node cur = lista.head;

        for (int i = 0; i < 100 && cur != null; i++, cur = cur.next) 
        {
            System.out.println(cur.r);
        }
    }
}
