require 'benchmark'
require 'objspace'

class Rating
  attr_accessor :user_id, :movie_id, :rating, :timestamp
  def initialize(user_id, movie_id, rating, timestamp)
    @user_id = user_id
    @movie_id = movie_id
    @rating = rating
    @timestamp = timestamp
  end
  def <=>(other)
    @rating <=> other.rating
  end
  def to_s
    "#{@user_id},#{@movie_id},#{@rating},#{@timestamp}"
  end
end

class QueueArrayCocktailSort
  attr_accessor :queue, :size
  def initialize(capacity)
    @queue = []
    @size = 0
  end
  def enqueue(rating)
    @queue << rating
    @size += 1
  end
  def cocktail_sort
    return if @size <= 1
    swapped = true
    start_i = 0
    end_i = @size - 1
    while swapped
      swapped = false
      (start_i...end_i).each do |i|
        if @queue[i] > @queue[i + 1]
          @queue[i], @queue[i + 1] = @queue[i + 1], @queue[i]
          swapped = true
        end
      end
      break unless swapped
      swapped = false
      end_i -= 1
      end_i.downto(start_i + 1) do |i|
        if @queue[i - 1] > @queue[i]
          @queue[i], @queue[i - 1] = @queue[i - 1], @queue[i]
          swapped = true
        end
      end
      start_i += 1
    end
  end
end

if ARGV.size < 2
  puts "Uso: ruby queue_array_cocktail_sort.rb <arquivo.csv> <N>"
  exit
end

path = ARGV[0]
n = ARGV[1].to_i
fila = QueueArrayCocktailSort.new(n)

File.open(path) do |f|
  f.readline
  f.each_line.take(n).each do |line|
    user_id, movie_id, rating, timestamp = line.strip.split(',')
    fila.enqueue(Rating.new(user_id.to_i, movie_id.to_i, rating.to_f, timestamp.to_i))
  end
end

mem_before = ObjectSpace.memsize_of_all
time = Benchmark.realtime { fila.cocktail_sort }
mem_after = ObjectSpace.memsize_of_all

puts "Ordenado #{n} registros em #{(time * 1000).round(2)}极 ms"
puts "Memória usada: #{((mem_after - mem_before) / 1024.0).round(2)} KB"
puts "Primeiros 100 registros ordenados:"
fila.queue.first(100).each { |r| puts r }

puts "Últimos 100 registros ordenados:"
fila.queue.last(100).each { |r| puts r }