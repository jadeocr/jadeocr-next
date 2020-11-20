class CreateReadings < ActiveRecord::Migration
  def self.up
    create_table(:readings) do |t|
      t.column :utf8, :string, :null => false
      t.column :reading_type, :integer
      t.column :reading, :string, :null => false
    end
    add_index :readings, :utf8
  end

  def self.down
    drop_table :readings
  end
end
