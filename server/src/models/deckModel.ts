var mongoose = require('mongoose')

var deckSchema = new mongoose.Schema({
    title: String,
    description: String,
    characters: [{
        char: String,
        id: String
    }],
    creator: String,
    creatorFirst: String,
    creatorLast: String,
    isPublic: Boolean,
})

module.exports = new mongoose.model("decks", deckSchema)