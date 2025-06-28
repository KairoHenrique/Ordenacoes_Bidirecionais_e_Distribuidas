import java.io.*;

public class ListArrayCocktailSort
{
    static class Rating
    {
        int userId, movieId;
        float rating;
        long timestamp;

        Rating(int userId, int movieId, float rating, long timestamp)
        {
            this.userId = userId;
            this.movieId = movieId;
            this.rating = rating;
            this.timestamp = timestamp;
        }

        int compareTo(Rating other)
        {
            return Float.compare(this.rating, other.rating);
        }

        public String toString()
        {
            return userId + "," + movieId + "," + rating + "," + timestamp;
        }
    }

    Rating[] data;
    int size;

    public ListArrayCocktailSort(int capacity)
    {
        data = new Rating[capacity];
        size = 0;
    }

    public void add(Rating r)
    {
        data[size++] = r;
    }

    public void cocktailSort()
    {
        boolean swapped = true;
        int start = 0, end = size - 1;

        while (swapped)
        {
            swapped = false;
            for (int i = start; i < end; i++)
            {
                if (data[i].compareTo(data[i + 1]) > 0)
                {
                    swap(i, i + 1);
                    swapped = true;
                }
            }

            if (!swapped) break;
            swapped = false;
            end--;

            for (int i = end; i > start; i--)
            {
                if (data[i - 1].compareTo(data[i]) > 0)
                {
                    swap(i - 1, i);
                    swapped = true;
                }
            }
            start++;
        }
    }

    private void swap(int i, int j)
    {
        Rating temp = data[i];
        data[i] = data[j];
        data[j] = temp;
    }

    public static void main(String[] args) 
    throws Exception
    {
        if (args.length < 2)
        {
            System.out.println("Uso: java ListArrayCocktailSort <ratings.csv> <N>");
            return;
        }

        String path = args[0];
        int N = Integer.parseInt(args[1]);

        ListArrayCocktailSort lista = new ListArrayCocktailSort(N);

        try (BufferedReader br = new BufferedReader(new FileReader(path)))
        {
            String line = br.readLine();
            int count = 0;

            while ((line = br.readLine()) != null && count < N)
            {
                String[] parts = line.split(",");
                int userId = Integer.parseInt(parts[0]);
                int movieId = Integer.parseInt(parts[1]);
                float rating = Float.parseFloat(parts[2]);
                long timestamp = Long.parseLong(parts[3]);
                lista.add(new Rating(userId, movieId, rating, timestamp));
                count++;
            }
        }

        long startTime = System.nanoTime();
        lista.cocktailSort();
        long endTime = System.nanoTime();

        System.out.printf("Ordenado %d registros em %.2f ms%n", N, (endTime - startTime) / 1e6);
        System.out.println("Primeiros 20 registros ordenados:");
        
        for (int i = 0; i < Math.min(100, lista.size); i++)
        {
            System.out.println(lista.data[i]);
        }
    }
}

