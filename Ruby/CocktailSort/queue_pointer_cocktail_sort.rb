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

class Node
  attr_accessor :rating, :next, :prev
  def initialize(rating)
    @rating = rating
    @next = nil
    @prev = nil
  end
end

class QueuePointerCocktailSort
  attr_accessor :head, :tail, :size
  def initialize
    @head = @tail = nil
    @size = 0
  end
  def enqueue(rating)
    node = Node.new(rating)
    if @head.nil?
      @head = @tail = node
    else
      @tail.next = node
      node.prev = @tail
      @tail = node
    end
    @size += 1
  end
  def cocktail_sort
    swapped = true
    start = @head
    end_node = @tail
    while swapped
      swapped = false
      cur = start
      while cur != end_node
        if cur.rating > cur.next.rating
          cur.rating, cur.next.rating = cur.next.rating, cur.rating
          swapped = true
        end
        cur = cur.next
      end
      break unless swapped
      swapped = false
      end_node = end_node.prev
      cur = end_node
      while cur != start
        if cur.prev.rating > cur.rating
          cur.rating, cur.prev.rating = cur.prev.rating, cur.rating
          swapped = true
        end
        cur = cur.prev
      end
      start = start.next
    end
  end
end

if ARGV.size < 2
  puts "Uso: ruby queue_pointer_cocktail_sort.rb <arquivo.csv> <N>"
  exit
end

path = ARGV[0]
n = ARGV[1].to_i
fila = QueuePointerCocktailSort.new

File.open(path) do |f|
  f.readline
  f.each_line.take(n).each do |line|
    user_id, movie_id, rating, timestamp = line.strip.split(',')
    fila.enqueue(Rating.new(user_id.to_i, movie_id.to_i, rating.to_f, timestamp.to_i))
  end
end

mem_before = ObjectSpace.memsize_of_all
time = Benchmark.realtime { fila.cocktail极 }
mem_after = ObjectSpace.memsize_of_all

puts "Ordenado #{n} registros em #{(time * 1000).round(2)} ms"
puts "Memória usada: #{((mem_after - mem_before) / 1024.0).round(2)} KB"
puts "Primeiros 100 registros ordenados:"
cur = fila.head
100.times do
  break if cur.nil?
  puts cur.rating
  cur = cur.next
end

puts "Ultimos 100 registros ordenados:"
cur = fila.tail
100.times do
  break if cur.nil?
  puts cur.rating
  cur = cur.prev
end