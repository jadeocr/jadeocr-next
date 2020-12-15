var mongoose = require('mongoose')

var deckSchema = new mongoose.Schema({
    title: String,
    description: String,
    characters: [{
        char: String,
        pinyin: String,
        definition: String,
        id: String
    }],
    creator: String,
    creatorFirst: String,
    creatorLast: String,
    createdDate: Number,
    access: {
        isPublic: Boolean,
        users: Object,
        classes: Object,
    },
})

module.exports = new mongoose.model("decks", deckSchema)