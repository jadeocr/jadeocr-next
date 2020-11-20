require 'rubygems'
require 'active_record'

require 'yaml'

base = File.expand_path(File.dirname(__FILE__))
top = File.expand_path(File.join(base, ".."))

$LOAD_PATH.unshift(File.join(top, "test"))
$LOAD_PATH.unshift(File.join(top, "ext", "ruby", ".libs"))
$LOAD_PATH.unshift(File.join(top, "ext", "ruby"))

require 'tomoe-test-utils'

config = TomoeTestUtils::Config.db_config_for_active_record
ActiveRecord::Base.establish_connection(config)

logger = Logger.new($stdout)
logger.level = Logger::DEBUG
ActiveRecord::Base.logger = logger
