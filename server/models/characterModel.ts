var mongoose = require('mongoose')

var characterSchema = new mongoose.Schema({
  character: String,
  definition: String,
  pinyin: Array,
  decomposition: String,
  radical: String,
  matches: Array
}, {collection: "characters"})

module.exports = mongoose.model('charcters', characterSchema)
