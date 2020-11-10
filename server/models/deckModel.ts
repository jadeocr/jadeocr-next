var mongoose = require('mongoose')

var deckSchema = new mongoose.Schema({
    title: String,
    description: String,
    characters: Array,
    creator: String,
    isPublic: Boolean,
})

module.exports = new mongoose.model("decks", deckSchema)