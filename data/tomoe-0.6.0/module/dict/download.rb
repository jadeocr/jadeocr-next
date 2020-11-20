#!/usr/bin/env ruby

require 'uri'

if ARGV.size < 1
  puts "Usage: #{$0} URI"
  exit 1
end

uri = URI.parse(ARGV.first)

if uri.is_a?(URI::FTP)
  require 'net/ftp'
  Net::FTP.open(uri.host) do |ftp|
    ftp.passive = true
    ftp.login
    ftp.passive
    dir, file = File.split(uri.path)
    ftp.chdir(dir)
    ftp.getbinaryfile(file)
  end
else
  require 'open-uri'
  File.open(File.basename(uri.path), "wb") do |output|
    output.print uri.read
  end
end
