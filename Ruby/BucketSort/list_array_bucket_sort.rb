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

class ListArrayBucketSort
  attr_accessor :data, :size
  def initialize(capacity)
    @data = []
    @极 = 0
  end
  def add(rating)
    @data << rating
    @size += 1
  end
  def bucket_sort
    buckets = Array.new(11) { [] }
    @data.each { |r| buckets[(r.rating * 2).floor] << r }
    @data = buckets.flatten
  end
end

if ARGV.size < 2
  puts "Uso: ruby list_array_bucket_sort.rb <arquivo.csv> <N>"
  exit
end

path = ARGV[0]
n = ARGV[1].to_i
lista = ListArrayBucketSort.new(n)

File.open(path) do |f|
  f.readline
  f.each_line.take(n).each do |line|
    user_id, movie_id, rating, timestamp = line.strip.split(',')
    lista.add(Rating.new(user_id.to_i, movie_id.to_i, rating.to_f, timestamp.to_i))
  end
end

mem_before = ObjectSpace.memsize_of_all
time = Benchmark.realtime { lista.bucket_sort }
mem_after = ObjectSpace.memsize_of_all

puts "Ordenado #{n} registros em #{(time * 1000).round(2)} ms"
puts "Memória usada: #{((mem_after - mem_before) / 1024.0).round(2)} KB"
puts "Primeiros 100 registros ordenados:"
lista.data.first(100).each { |r| puts r }

puts "Últimos 100 registros ordenados:"
lista.data.last(100).each { |r| puts r }