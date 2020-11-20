#!/usr/bin/env ruby

require 'tomoe-test-utils'

require 'uconv'
require 'suikyo/suikyo'

unihan_txt = ARGV.shift
kanjidic2_xml = ARGV.shift

DO_NOT_EDIT_HEADER = <<-EOH
/*
  DO NOT EDIT!
  THIS FILE IS GENERATED FROM Unihan.txt:
    ftp://ftp.unicode.org/Public/UNIDATA/Unihan.zip
*/
EOH

@romaji_to_hiragana = Suikyo.new("romaji-kana")
@hiragana_to_katakana = Suikyo.new("hiragana-katakana")

def kcode(code)
  old_kcode = $KCODE
  $KCODE = code
  yield
ensure
  $KCODE = old_kcode
end

def euc_to_utf8(euc)
  Uconv.euctou8(euc)
end

def romaji_to_hiragana(romaji)
  kcode("e") do
    euc_to_utf8(@romaji_to_hiragana.convert("#{romaji} "))
  end
end

def romaji_to_katakana(romaji)
  kcode("e") do
    hiragana = @romaji_to_hiragana.convert("#{romaji} ")
    euc_to_utf8(@hiragana_to_katakana.convert("#{hiragana} "))
  end
end

def ucs4_to_utf8(ucs4)
  TomoeTestUtils::Unicode.ucs4_to_utf8(Integer("0x#{ucs4}"))
end

def utf8_to_ucs4(utf8)
  "%X" % TomoeTestUtils::Unicode.utf8_to_ucs4(utf8)
end

def cache(filename)
  cache = "#{filename}.cache"
  if File.exists?(cache) and (File.mtime(cache) > File.mtime(filename))
    begin
      return Marshal.load(File.read(cache))
    rescue ArgumentError
    end
  end

  result = yield
  File.open(cache, "wb") {|f| f.print(Marshal.dump(result))}
  result
end

def parse_unihan_entry(key, value)
  case key
  when "kCompatibilityVariant"
    [:variants, value.split.collect {|v| ucs4_to_utf8(v.sub(/^U\+2?/, ''))}]
  when "kJapaneseKun"
    [:ja_kuns, value.split.collect {|k| romaji_to_hiragana(k)}]
  when "kJapaneseOn"
    [:ja_ons, value.split.collect {|o| romaji_to_katakana(o)}]
  when "kTotalStrokes"
    [:n_strokes, value]
  else
    [key, value]
  end
end

def parse_unihan_txt(unihan_txt)
  cache(unihan_txt) do
    infos = {}
    File.open(unihan_txt).each do |line|
      case line
      when /^#/ #
        next
      when /^U\+([\da-fA-F]+)\s+([a-zA-Z_]+)\s*(.*)\s*$/u
        ucs4 = $1.upcase
        key = $2
        value = $3

        infos[ucs4] ||= {}
        key, value = parse_unihan_entry(key, value)
        infos[ucs4][key] = value
      else
        STDERR.puts "Unknown line: #{line}"
      end
    end
    infos
  end
end

def merge_kanjidic2_xml(kanjidic2_xml, infos)
  dict = Tomoe::DictXML.new("filename" => kanjidic2_xml, "editable" => false)
  dict.search(Tomoe::Query.new).each do |cand|
    char = cand.char
    ucs4 = utf8_to_ucs4(char.utf8)
    info = infos[ucs4] || {}

    info[:n_strokes] ||= char.n_strokes

    info[:ja_ons] ||= []
    info[:ja_kuns] ||= []
    char.readings.each do |reading|
      case reading.type
      when Tomoe::Reading::JA_ON
        info[:ja_ons] << reading.reading
      when Tomoe::Reading::JA_KUN
        info[:ja_kuns] << reading.reading
      end
    end
    info[:ja_ons].uniq!
    info[:ja_kuns].uniq!

    info[:radicals] ||= []
    info[:radicals].concat(char.radicals)
    info[:radicals].uniq!

    if char.variant
      info[:variants] ||= []
      info[:variants] << char.variant
      info[:variants].uniq!
    end

    info[:meta_data] ||= {}
    char.each do |key, value|
      info[:meta_data][key] = value
    end

    infos[ucs4] = info
  end
  infos
end

def generate_header(infos)
  prefix = "tomoe_unihan_"

  puts <<-EOH
/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
#{DO_NOT_EDIT_HEADER}

#include "tomoe-unihan.h"

#include <tomoe-char.h>

typedef struct _TomoeUnihanReading TomoeUnihanReading;
typedef struct _TomoeUnihanMetaData TomoeUnihanMetaData;
typedef struct _TomoeUnihanInfo TomoeUnihanInfo;

struct _TomoeUnihanReading {
    TomoeReadingType  type;
    gchar            *reading;
};

struct _TomoeUnihanMetaData {
    gchar *key;
    gchar *value;
};

struct _TomoeUnihanInfo {
    gchar                *utf8;
    gint                  n_strokes;
    TomoeUnihanReading   *readings;
    gint                  readings_size;
    gchar               **radicals;
    gint                  radicals_size;
    gchar               **variants;
    gint                  variants_size;
    TomoeUnihanMetaData  *meta_data;
    gint                  meta_data_size;
};

EOH

  infos.each_with_index do |(ucs4, info), i|
    readings = []
    kuns = info[:ja_kuns]
    ons = info[:ja_ons]
    if kuns
      readings.concat(kuns.collect {|x| ["TOMOE_READING_JA_KUN", x]})
    end
    if ons
      readings.concat(ons.collect {|x| ["TOMOE_READING_JA_ON", x]})
    end

    unless readings.empty?
      info[:have_readings] = true
      puts("static TomoeUnihanReading #{prefix}readings_#{ucs4}[] = {")
      readings.each do |type, reading|
        puts("    {#{type}, \"#{reading}\"},")
      end
      puts("};")
    end

    radicals = info[:radicals] || []
    unless radicals.empty?
      info[:have_radicals] = true
      puts("static gchar *#{prefix}radicals_#{ucs4}[] = {")
      radicals.each do |radical|
        puts("    \"#{radical}\",")
      end
      puts("};");
    end

    variants = info[:variants] || []
    unless variants.empty?
      info[:have_variants] = true
      puts("static gchar *#{prefix}variants_#{ucs4}[] = {")
      variants.each do |variant|
        puts("    \"#{variant}\",")
      end
      puts("};");
    end

    meta_data = info[:meta_data] || []
    unless meta_data.empty?
      info[:have_meta_data] = true
      puts("static TomoeUnihanMetaData #{prefix}meta_data_#{ucs4}[] = {")
      meta_data.each do |key, value|
        puts("    {\"#{key}\", \"#{value.gsub(/\"/, '\"')}\"},")
      end
      puts("};")
    end
  end

  puts("static TomoeUnihanInfo #{prefix}infos[] = {")
  infos.each_with_index do |(ucs4, info), i|
    utf8 = ucs4_to_utf8(ucs4)
    n_strokes = info[:n_strokes] || -1
    readings = radicals = variants = meta_data = "NULL"
    readings_size = radicals_size = variants_size = meta_data_size = "0"
    if info[:have_readings]
      readings = "#{prefix}readings_#{ucs4}"
      readings_size = "G_N_ELEMENTS(#{readings})"
    end
    if info[:have_radicals]
      radicals =  "#{prefix}radicals_#{ucs4}"
      radicals_size = "G_N_ELEMENTS(#{radicals})"
    end
    if info[:have_variants]
      variants =  "#{prefix}variants_#{ucs4}"
      variants_size = "G_N_ELEMENTS(#{variants})"
    end
    if info[:have_meta_data]
      meta_data =  "#{prefix}meta_data_#{ucs4}"
      meta_data_size = "G_N_ELEMENTS(#{meta_data})"
    end

    puts("    {\"#{utf8}\", #{n_strokes}, #{readings}, #{readings_size},")
    puts("     #{radicals}, #{radicals_size}, #{variants}, #{variants_size},")
    puts("     #{meta_data}, #{meta_data_size}},")
  end
  puts("};")
end

infos = parse_unihan_txt(unihan_txt)
infos = merge_kanjidic2_xml(kanjidic2_xml, infos).collect do |ucs4, info|
  [ucs4, info]
end.sort_by do |ucs4, info|
  ucs4
end
generate_header(infos)
