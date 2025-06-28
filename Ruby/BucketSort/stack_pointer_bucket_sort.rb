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

class StackPointerBucketSort
  attr_accessor :top, :size
  def initialize
    @top = nil
    @size = 0
  end

  def push(rating)
    node = Node.new(rating)
    if @top.nil?
      @top = node
    else
      node.prev = @top
      @top.next = node
      @top = node
    end
    @size += 1
  end

  def bucket_sort
    base = @top
    base = base.prev while base&.prev

    buckets = Array.new(11) { [] }
    p = base
    while p
      idx = [(p.rating.rating * 2).floor, 10].min
      buckets[idx] << p
      p = p.next
    end

    # Reorganiza os nós
    new_top = nil
    prev = nil
    buckets.each do |bucket|
      bucket.each do |node|
        node.prev = prev
        node.next = nil
        prev.next = node if prev
        prev = node
        new_top = node # último da nova lista
      end
    end

    @top = new_top
  end
end

# --- Execução principal ---

if ARGV.size < 2
  puts "Uso: ruby stack_pointer_bucket_sort.rb <arquivo.csv> <N>"
  exit
end

path = ARGV[0]
n = ARGV[1].to_i
pilha = StackPointerBucketSort.new

File.open(path) do |f|
  f.readline
  f.each_line.take(n).each do |line|
    user_id, movie_id, rating, timestamp = line.strip.split(',')
    pilha.push(Rating.new(user_id.to_i, movie_id.to_i, rating.to_f, timestamp.to_i))
  end
end

mem_before = ObjectSpace.memsize_of_all
time = Benchmark.realtime { pilha.bucket_sort }
mem_after = ObjectSpace.memsize_of_all

puts "Ordenado #{n} registros em #{(time * 1000).round(2)} ms"
puts "Memória usada: #{((mem_after - mem_before) / 1024.0).round(2)} KB"

puts "Primeiros 100 registros ordenados:"
cur = pilha.top
cur = cur.prev while cur&.prev
100.times do
  break if cur.nil?
  puts cur.rating
  cur = cur.next
end

puts "Últimos 100 registros ordenados:"
cur = pilha.top
100.times do
  break if cur.nil?
  puts cur.rating
  cur = cur.prev
end

puts "Tamanho da pilha: #{pilha.size}"