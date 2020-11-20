class CreateMetaData < ActiveRecord::Migration
  def self.up
    create_table(:meta_data) do |t|
      t.column :utf8, :string, :null => false
      t.column :name, :string, :null => false
      t.column :value, :text, :null => false
    end
    add_index :meta_data, :utf8
  end

  def self.down
    drop_table :meta_data
  end
end
