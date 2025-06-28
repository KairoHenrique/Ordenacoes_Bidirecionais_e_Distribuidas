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

class ListPointerPigeonholeSort
  attr_accessor :head, :tail, :size

  def initialize
    @head = @tail = nil
    @size = 0
  end

  def add(rating)
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

  def pigeonhole_sort
    holes = Array.new(10) { [] }
    cur = @head
    while cur
      idx = [(cur.rating.rating * 2).round - 1, 9].min
      idx = [idx, 0].max  # Garante que idx esteja entre 0 e 9
      holes[idx] << cur
      cur = cur.next
    end

    @head = @tail = nil
    @size = 0

    holes.each do |hole|
      hole.each do |node|
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

# ------------------- EXECUÇÃO PRINCIPAL -------------------

if ARGV.size < 2
  puts "Uso: ruby #{__FILE__} <arquivo.csv> <N>"
  exit
end

path = ARGV[0]
n = ARGV[1].to_i
lista = ListPointerPigeonholeSort.new

File.open(path) do |f|
  f.readline # Ignora o cabeçalho
  f.each_line.take(n).each do |line|
    user_id, movie_id, rating, timestamp = line.strip.split(',')
    lista.add(Rating.new(user_id.to_i, movie_id.to_i, rating.to_f, timestamp.to_i))
  end
end

mem_before = ObjectSpace.memsize_of_all
time = Benchmark.realtime { lista.pigeonhole_sort }
mem_after = ObjectSpace.memsize_of_all

puts "Ordenado #{n} registros em #{(time * 1000).round(2)} ms"
puts "Memória usada: #{((mem_after - mem_before) / 1024.0).round(2)} KB"

puts "\nPrimeiros 100 registros ordenados:"
cur = lista.head
100.times do
  break if cur.nil?
  puts cur.rating
  cur = cur.next
end

puts "\nÚltimos 100 registros ordenados:"
cur = lista.tail
100.times do
  break if cur.nil?
  puts cur.rating
  cur = cur.prev
end

puts "\nTotal de registros: #{lista.size}"