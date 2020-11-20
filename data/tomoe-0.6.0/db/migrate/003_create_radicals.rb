class CreateRadicals < ActiveRecord::Migration
  def self.up
    create_table(:radicals) do |t|
      t.column :utf8, :string, :null => false
      t.column :radical_utf8, :string, :null => false
    end
    add_index :radicals, :utf8
  end

  def self.down
    drop_table :radicals
  end
end
