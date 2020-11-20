class Char < ActiveRecord::Base
  has_many :readings, :foreign_key => "utf8"
  has_many :radicals, :foreign_key => "utf8"
  has_many :meta_data, :foreign_key => "utf8"
end
