require 'glib2'
require 'tomoe.so'

module Tomoe
  LOG_DOMAIN = "Tomoe"

  class << self
    def const_missing(name)
      @missing ||= false
      super if @missing
      begin
        missing = @missing
        @missing = true
        case name.to_s
        when /^Dict(.+)$/
          return const_get(name) if Dict.load($1)
        when /^Recognizer(.+)$/
          return const_get(name) if Recognizer.load($1)
        end
        super
      ensure
        @missing = missing
      end
    end
  end
end

GLib::Log.set_log_domain(Tomoe::LOG_DOMAIN)
