require 'icanhasaudio'

include Audio::MPEG

writer = Encoder.new

### NOTE:  All of these settings are optional ###
# The default setup is 128kbps minimum plus VBR.

# Set up the encoder to be 128kbps, NON-VBR
writer.bitrate  = 128
writer.vbr_type = Encoder::VBR_OFF

# Set the ID3 tags
writer.title  = 'tenderlovemaking.com'
writer.artist = 'Aaron Patterson'
writer.album  = 'ICANHASAUDIO'
writer.year   = 2008
writer.track  = 1
writer.genre  = 'Porn Groove'

# Make sure to open your outfile as read and write.  Writing VBR tags requires
# seeking through the file.
File.open(ARGV[1], 'wb+') { |outfile|
  File.open(ARGV[0], 'rb') { |infile|
    writer.encode(infile, outfile)
  }
}
