require 'rubygems'
require 'raop'
require 'icanhasaudio'
require 'socket'

rd, wr = IO.pipe
puts Audio::MPEG::Decoder.lame_version

decoder = Thread.new(rd, wr) { |read, write|
  reader = Audio::MPEG::Decoder.new
  socket = TCPSocket.new('kexp-mp3-128k.cac.washington.edu', '8000')
  socket.puts("GET / HTTP/1.0\r\n\r\n")
  until(socket.readline == "\r\n"); end
  reader.decode(socket, write);
  write.close
}

sleep 2

raop = Net::RAOP::Client.new('10.0.1.1')
raop.connect
raop.play rd
raop.disconnect
rd.close

decoder.join

