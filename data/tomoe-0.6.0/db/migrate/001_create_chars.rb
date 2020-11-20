class CreateChars < ActiveRecord::Migration
  def self.up
    create_table(:chars) do |t|
      t.column :utf8, :string
      t.column :code_point, :integer
      t.column :n_strokes, :integer
      t.column :variant, :string
    end
    add_index :chars, :utf8, :unique => true
  end

  def self.down
    drop_table :chars
  end
end
