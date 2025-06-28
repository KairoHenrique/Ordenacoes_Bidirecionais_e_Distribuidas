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

class QueuePointerBucketSort
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
  def bucket_sort
    buckets = Array.new(11) { [] }
    cur = @head
    while cur
      idx = (cur.rating.rating * 2).floor
      buckets[idx] << cur
      cur = cur.next
    end
    @head = @tail = nil
    @size = 0
    buckets.each do |bucket|
      bucket.each do |node|
        node.next = node.prev = nil
        if @head.nil?
          @head = @tail = node
        else
          @tail.next = node
          node.prev = @tail
          @tail = node
        end
        @size += 1
      end
    end
  end
end

if ARGV.size < 2
  puts "Uso: ruby queue_pointer_bucket_sort.rb <arquivo.csv> <N>"
  exit
end

path = ARGV[0]
n = ARGV[1].to_i
fila = QueuePointerBucketSort.new

File.open(path) do |f|
  f.readline
  f.each_line.take(n).each do |line|
    user_id, movie_id, rating, timestamp = line.strip.split(',')
    fila.enqueue(Rating.new(user_id.to_i, movie_id.to_i, rating.to_f, timestamp.to_i))
  end
end

mem_before = ObjectSpace.memsize_of_all
time = Benchmark.realtime { fila.bucket_sort }
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

puts "Últimos 100 registros ordenados:"
cur = fila.tail
100.times do
  break if cur.nil?
  puts cur.rating
  cur = cur.prev
end